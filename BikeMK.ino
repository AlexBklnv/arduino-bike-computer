#include <Bounce2.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <iarduino_RTC.h>

iarduino_RTC time(RTC_DS3231);                          // инициализация работы со временем
LiquidCrystal_I2C lcd(0x27, 16, 2);                     // устанавливаем дисплей
byte brightness = 255;                                  // яркость экрана

const unsigned long dynCharRefreshRate = 5000;          // значение интервала для сброса динамических параметров как текущая скорость 5сек
const unsigned long screenRefreshRate = 600;            // частота обновления экрана 600мс
const unsigned long millisecondsOf24Hours = 86400000;   // 24 часа
const unsigned long millisecondsOf15minutes = 900000;   // 15 минут
const unsigned long millisecondsOf5minutes = 300000;    // 5 минут

volatile unsigned long lastCycleTurnTime = 0;	        // время от посленей фиксации оборота колеса
volatile float curSpeed = 0.0;                          // текущая скорость
volatile unsigned long stopTime = 0;                    // штамп времени остановки
volatile bool stopHandler = false;                      // штам остановки активирован
volatile float MaxSpeed = 0.0;                          // максимальаня скорость

unsigned long lifeCycleTime = 0;                        // переменная для отсчета времени необходимости обновления экрана
bool isMovement = false;                                // находится ли велосипедист в движении

float cycleLengthValue = 1.000;                         // длина окружности колеса в метрах
int cycleLengthValueMM = 1000;                          // длина окружности колеса в миллиметрах

byte timeModeSet = 2;	                                // позиция при настройке времени (начальная минуты)
byte menuPosition = 0;                                  // пункт меню
byte settingPosition = 0;
byte settingsCursorPosition = 0;
bool isSettingsMenuActive = false;

unsigned long  travelTime = 0;	                        // время движения (динамический параметр)
volatile unsigned long travelDistance = 0;              // текущий путь в мм (динамический параметр)
unsigned long curCal = 0;                               // текущее количество сожженных ккал
int BPM = 0;                                            // текущее сердцебиение

unsigned long totalDistanceMM = 0;                      // общий путь в миллиметрах для точности
unsigned long totalDistance = 0;	                // общий путь в км
int totalDays = 0;                                      // общее время движения, часть дней
unsigned long totalTime = 0;                            // общее время движения, часть минут и часов

int avgBPM = 0;                                         // среднее значение пульса за период
float avgSpeed = 0.0;                                   // среднее значение скорости за период

bool redrawScreen = true;                               // требуется ли обновить экран заголовков
bool redrawValues = true;                               // требуется ли обновить экран значений
bool saveData = false;
unsigned long saveStartTimeStamp = 0;
unsigned long saveStopTimeStamp = 0;

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
    if (millis() - lastCycleTurnTime >= dynCharRefreshRate) {
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
    } else if (saveStopTimeStamp - saveStartTimeStamp >= 5000) {
      saveDataAtEEPROM();
      saveStartTimeStamp = 0;
    }
    saveStopTimeStamp = millis();
  } else {
    if (stopHandler) {
      if (millis() - stopTime >= millisecondsOf15minutes) {
        saveDataAtEEPROM();
        resetTravelChar();
      }
    }
  }

  if (millis() - lifeCycleTime >= screenRefreshRate) {                  // если прошло больше чем 1 секунда  то обновить значения
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
        redrawValues = true;                                          // требуется обновить экран по времени для мерцания значения часов
      } else {
        redrawValues = false;                                         // иначе не требуется
      }
    }
    lifeCycleTime = millis();
  }
}
