#include <Bounce2.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <iarduino_RTC.h>
// интервал сброса
iarduino_RTC time(RTC_DS3231);                           // инициализация работы со временем
LiquidCrystal_I2C lcd(0x27, 16, 2);                      // устанавливаем дисплей
byte brightness = 127;

volatile unsigned long lastCycleTurnTime = 0;	        // время от посленей фиксации оборота колеса
volatile float curSpeed = 0.0;                          // текущая скорость
volatile float MaxSpeed = 0.0;                          // максимальаня скорость

volatile unsigned long stopTime = 0;
volatile bool stopHandler = false;

unsigned long maxTimeIntrvl = 0;
unsigned long minTimeIntrvl = 0;

const unsigned long dynCharRefreshRate = 5000;          // значение интервала для сброса динамических параметров как текущая скорость
const unsigned long screenRefreshRate = 600;            // частота обновления экрана
const unsigned long millisecondsOf24Hours = 86400000;   // 24 часа в формате миллисекунд
const unsigned long millisecondsOf15minutes = 900000;   // 15 минут без движения
unsigned long lifeCycleTime = 0;                        // переменная для отсчета времени необходимости обновления экрана
bool isMovement = false;                                // находится ли велосипедист в движении

float cycleLengthValue = 2.123;                         // длина окружности колеса в метрах
int cycleLengthValueMM = 2123;                          // длина окружности колеса в миллиметрах

byte timeModeSet = 2;	                                // позиция при настройке времени (начальная минуты)
byte menuPosition = 0;                                  // пункт меню
byte settingPosition = 0;
byte settingsCursorPosition = 0;
bool isSettingsMenuActive = false;

unsigned long  travelTime = 0;	                        // время движения
volatile unsigned long travelDistance = 0;              // текущий путь в мм
unsigned long curCal = 0;                               // текущее количество сожженных ккал
int BPM = 0;                                            // текущее сердцебиение

unsigned long totalDistanceMM = 0;
unsigned long totalDistance = 0;	                // общий путь в км
int totalDays = 0;                                      // общее время движения, часть дней
unsigned long totalTime = 0;                            // общее время движения, часть минут и часов

int avgBPM = 0;                                         // среднее значение пульса за период
float avgSpeed = 0.0;                                   // среднее значение скорости за период

bool redrawScreen = true;
bool redrawValues = true;

void setup() {
  initLCD();
  calculateMaxTimeForSpeedRegistration();       // временно
  calculateMinTimeForSpeedRegistration();
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
  } else {
    if (stopHandler) {
      if (millis() - stopTime >= millisecondsOf15minutes) {
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
