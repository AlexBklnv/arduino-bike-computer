#include <SPI.h>
#include <Bounce2.h>
#include <Wire.h>
#include <SD.h>
#include <LiquidCrystal_I2C.h>
#include <iarduino_RTC.h>
#include <EEPROM.h>
#include <nRF24L01.h>                                     // Подключаем файл настроек из библиотеки RF24
#include <RF24.h>

iarduino_RTC time(RTC_DS3231);                          // инициализация работы со временем
LiquidCrystal_I2C lcd(0x27, 16, 2);                     // устанавливаем дисплей

byte brightness = 127;                                  // яркость экрана
const unsigned long millisecondsOf24Hours = 86400000;   // 24 часа

volatile unsigned long totalDistanceMM = 0;             // общий путь в миллиметрах для точности
volatile unsigned long travelDistance = 0;              // текущий путь в мм (динамический параметр)
volatile unsigned long travelTime = 0;	                // время движения (динамический параметр)
volatile unsigned long totalTime = 0;                   // общее время движения, часть минут и часов
volatile unsigned long lastCycleTurnTime = 0;	        // время от посленей фиксации оборота колеса
volatile unsigned long stopTime = 0;                    // штамп времени остановки
volatile float curSpeed = 0.0;                          // текущая скорость
volatile float maxSpeed = 0.0;                             // максимальня скорость округленная

unsigned long lifeCycleTime = 0;                        // переменная для отсчета времени необходимости обновления экрана
bool isMovement = false;                                // находится ли велосипедист в движении
bool stopHandler = false;                               // штам остановки активирован

float cycleLengthValue = 1.000;                         // длина окружности колеса в метрах
int cycleLengthValueMM = 1000;                          // длина окружности колеса в миллиметрах

// переменные связанные с отображением на экранах
byte timeModeSet = 2;	                                // позиция при настройке времени (начальная минуты)
byte menuPosition = 0;                                  // пункт меню
byte settingPosition = 0;
byte settingsCursorPosition = 0;
bool isSettingsMenuActive = false;

unsigned long curCal = 0;                               // текущее количество сожженных ккал
int BPM = 0;                                            // текущее сердцебиение

unsigned long totalDistance = 0;	                // общий путь в км
int totalDays = 0;                                      // общее время движения, часть дней

bool redrawScreen = true;                               // требуется ли обновить экран заголовков
bool redrawValues = true;                               // требуется ли обновить экран значений

bool saveData = false;                                  // требуется ли сохранить данные в EEPROM
unsigned long saveStartTimeStamp = 0;                   // начало периода отсчета сохранения данных в EEPROM
unsigned long saveStopTimeStamp = 0;                    // конец периода отсчета сохранения данных в EEPROM

unsigned long dynBurned = 0;
unsigned long dynDist = 0;
float dynSpeed = 0.0;
int countDynAvgSpeed = 0;
int dynHR = 0;
int countDynAvgHR = 0;

void setup() {
  initLCD();
  readDataFromEEPROM();
  calculateMaxMinTimeForSpeedReg();
  time.begin();                                         // запуск работы с часами
  initButtons();
  initSpeedRegistarator();
  lcd.clear();
}

void loop() {
  buttonsHandler();                                                     // проверяем есть ли события на кнопке
  if (isMovement) {                                                     // если мы движемся
    if (millis() - lastCycleTurnTime >= 5000) {                         // значение интервала для сброса динамических параметров как текущая скорость 5сек
      travelDynCharReset();
    }
    if (totalDistanceMM >= 1000000) {                                   // елси путь стал = 1км
      totalDistance++;                                                  // глоабльная дистанция в км увеличивается
      if (totalDistance >= 999999000)                                   // если пересекли рубеж в 999999 км то обнуляем
        totalDistance = 0;
      totalDistanceMM = totalDistanceMM - 1000000;                      // находим новое значение дистанции
    }
    if (totalTime >= millisecondsOf24Hours) {                           // если время стало больше 24 часов
      totalDays++;                                                      // увеличиваем количество дней
      if (totalDays > 999) {                                            // если дней больше 999 то обнуляем
        totalDays = 0;
      }
      totalTime = totalTime - millisecondsOf24Hours;                    // зануляем время и прибовляем хвост
    }
    if (travelTime >= millisecondsOf24Hours) {                          // если текущее время дистанции больше 24 часов
      travelTime = travelTime - millisecondsOf24Hours;                  // зануляем время и прибовляем хвост
    }
    if (saveStartTimeStamp == 0) {
      saveStartTimeStamp = millis();
    } else if (saveStopTimeStamp - saveStartTimeStamp >= 300000) {      // 5 минут
      saveDataAtEEPROM();
      saveStartTimeStamp = 0;
    }
    saveStopTimeStamp = millis();
  } else {
    if (stopHandler) {
      if (millis() - stopTime >= 900000) {                              // 15 минут
        saveDataAtEEPROM();
        resetTravelChar();
      }
    }
  }

  if (millis() - lifeCycleTime >= 600) {                                // если прошло больше чем 1 секунда  то обновить значения
    if (redrawScreen) {                                                 // требуется обновить экран
      if (!isSettingsMenuActive) {                                      // если не в меню настроек
        printCurrentScreenTittles();                                    // обновляем заголовки экрана параметров
      } else {                                                          // если же в меню настроек
        printCurrentScreenSettingsTittles();                            // обновляем заголовки экрана настроек
      }
      redrawScreen = false;                                             // обновление не требуется
    }
    if (redrawValues) {                                                 // требуется обновить динамические параметры
      if (!isSettingsMenuActive) {                                      // если не в экране настроек
        printCurrnetScreenValues();                                     // обновляем параметры тренировки
      } else {                                                          // если на экране настроек
        printCurrentScreenSettingsValues();                             // обновляем значения
      }
      if (settingPosition == 2 && isSettingsMenuActive) {              // если в меню настроек 2
        redrawValues = true;                                           // требуется обновить экран по времени для мерцания значения часов
      } else {
        redrawValues = false;                                         // иначе не требуется
      }
    }
    lifeCycleTime = millis();
  }
}

