#include <Bounce2.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <iarduino_RTC.h>
// интервал сброса
// короткое длинное нажатие кнопки
iarduino_RTC time(RTC_DS3231);                       // инициализация работы со временем
LiquidCrystal_I2C lcd(0x27, 16, 2);                  // устанавливаем дисплей

const byte gerconPin = 2;                            // пин геркона

// установка пинов кнопок
const byte button1Pin = 5;
const byte button2Pin = 6;
const byte button3Pin = 7;


const unsigned long dynCharRefreshRate = 5000;      // значение интервала для сброса динамических параметров как текущая скорость
const unsigned long screenRefreshRate = 1000;       // частота обновления экрана

// анти дребезг контактов на кнопках
Bounce debounceButton1 = Bounce();
Bounce debounceButton2 = Bounce();
Bounce debounceButton3 = Bounce();

// экран
byte settingsCursorPosition = 0;                    // позиция курсора на экране
byte timeModeSet = 2;	                            // позиция при настройке времени (начальная минуты)
int menuPosition = 0;                               // пункт меню

int cycleLenghtValue = 1234;                        // длина окружности колеса

//герокон
volatile long countOfCycleTurns = 0;                // количество оборотов колеса
volatile unsigned long lastCycleTurnTime = 0;	    // время от посленей фиксации оборота колеса
volatile bool isInterruptGerconRunning = false;     // запущенно ли прерывание геркона
volatile float curSpeed = 0.0;                      // текущая скорость
volatile float curDistance = 0.0;	            // текущий путь
volatile int curPulse = 0;                          // текущий пульс

float maxSpeed = 0.0;                               // максимальная скорость
unsigned long  travelTime = 0;	                    // время движения
float cycleLengthValue = 3.600;	                    // значение длины окружности колеса
float totalDistance = 0.0;	                    // общий путь

int curCal = 0;                                     // текущее количество сожженных ккал
int avgPulse = 0;                                   // среднее значение пульса за период
float avgSpeed = 0.0;                               // среднее значение скорости за период



void setup() {
  initLCD();
  time.begin();                                     // запуск работы с часами
  // настройка пинов кнопок и подстрокйка подтягивающего резистора
  pinMode(button1Pin, INPUT);
  digitalWrite(button1Pin, HIGH);
  pinMode(button2Pin, INPUT);
  digitalWrite(button2Pin, HIGH);
  pinMode(button3Pin, INPUT);
  digitalWrite(button3Pin, HIGH);

  // привязка объектов антидребезка к кнопкам
  debounceButton1.attach(button1Pin);
  debounceButton1.interval(5);
  debounceButton2.attach(button2Pin);
  debounceButton2.interval(5);
  debounceButton3.attach(button3Pin);
  debounceButton3.interval(5);

  pinMode(2, INPUT);	                              // установка пина на вход для геркона
  attachInterrupt(0, SpeedRegistrator, RISING);	      // установка прерывания на смены 0-1
  
  lcd.clear();
}

void loop() {



}
