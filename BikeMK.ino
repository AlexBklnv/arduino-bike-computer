#include <Bounce2.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <iarduino_RTC.h>
// интервал сброса
iarduino_RTC time(RTC_DS3231);                           // инициализация работы со временем
LiquidCrystal_I2C lcd(0x27, 16, 2);                      // устанавливаем дисплей

volatile unsigned long lastCycleTurnTime = 0;	        // время от посленей фиксации оборота колеса
volatile float curSpeed = 0.0;                          // текущая скорость
volatile float MaxSpeed = 0.0;                          // максимальаня скорость

const unsigned long dynCharRefreshRate = 5000;          // значение интервала для сброса динамических параметров как текущая скорость
const unsigned long screenRefreshRate = 600;           // частота обновления экрана
const unsigned long millisecondsOf24Hours = 86400000;   // 24 часа в формате миллисекунд
unsigned long lifeCycleTime = 0;                        // переменная для отсчета времени необходимости обновления экрана
bool isMovement = false;                                // находится ли велосипедист в движении

int cycleLengthValue = 1234;                            // длина окружности колеса

byte timeModeSet = 2;	                            // позиция при настройке времени (начальная минуты)
byte menuPosition = 0;                              // пункт меню
byte settingPosition = 0;
byte settingsCursorPosition = 0;
bool isSettingsMenuActive = false;

unsigned long  travelTime = 0;	                        // время движения
volatile unsigned long travelDistance = 0;              // текущий путь в мм
unsigned long curCal = 0;                               // текущее количество сожженных ккал
int BPM = 0;                                            // текущее сердцебиение

unsigned long totalDistance = 0;	                // общий путь в км
word totalDays = 0;                                     // общее время движения, часть дней
word totalTime = 0;                                     // общее время движения, часть минут и часов

int avgBPM = 0;                                         // среднее значение пульса за период
float avgSpeed = 0.0;                                   // среднее значение скорости за период

bool redrawScreen = true;
bool redrawValues = true;

void setup() {
  initLCD();
  time.begin();                                         // запуск работы с часами
  initButtons();
  initSpeedRegistarator();
  lcd.clear();
}

void loop() {
  buttonsHandler();
  /*if (isMovement) {
    if (lifeCycleTime == 0) {
      lifeCycleTime = millis();
    }
    if (millis() - lastCycleTurnTime >= dynCharRefreshRate) {
        // если движемся и больше 5 секунд стоим то
    }

    if (millis() - lifeCycleTime >= screenRefreshRate) {
      travelTime += 1;
      totalTime += 1;
      if (totalTime >= millisecondsOf24Hours) {
        totalDays++;
        totalTime = 0;
      }
      lifeCycleTime = millis();
    }
  } else {
    // если нет движения 15 минут то обнулить параметры
  }*/
  if (millis() - lifeCycleTime >= screenRefreshRate) {
    if (!isSettingsMenuActive) {
      if (redrawScreen) {
        printCurrentScreenTittles();
        redrawScreen = false;
      }
      if (redrawValues) {
        printCurrnetScreenValues();
        redrawValues = false;
      }
    } else {
      if (redrawScreen) {
        printCurrentScreenSettingsTittles();
        redrawScreen = false;
      }
      if (settingPosition == 1){
        redrawValues = true;
      }
      if (redrawValues){
        printCurrentScreenSettingsValues();
        redrawValues = false;   
      }
    }
    lifeCycleTime = millis();
  }


}
