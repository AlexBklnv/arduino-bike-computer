#include <SPI.h>
#include <Bounce2.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <iarduino_RTC.h>
#include <EEPROM.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "SdFat.h"

// радио
#define CE_PIN 8                                                        // пин радиомодуля Chip Enable
#define CSN_PIN 9                                                       // пин радиомодуля Chip Select
RF24 radio(CE_PIN, CSN_PIN);
iarduino_RTC time(RTC_DS3231);                                          // инициализация работы со временем

// экран
LiquidCrystal_I2C lcd(0x27, 16, 2);                                     // устанавливаем дисплей
byte brightness = 127;                                                  // яркость экрана
bool redrawScreen = true;                                               // требуется ли обновить экран заголовков
volatile bool redrawValues = true;                                      // требуется ли обновить экран значений

// bluetooth
/*--statusBLE
  0 - ожидаем запроса подключения
  1 - ожидаем получение размера файла
  2 - ожидаем запрос на передачу динамических данных--*/
byte statusBLE = 0;
unsigned long requestResetTimeStamp = 0;

// переменные "общего" назначения
const unsigned long millisecondsOf24Hours = 86400000;                   // 24 часа в миллисекундах
volatile bool isMovement = false;                                       // находится ли велосипедист в движении
bool stopHandler = false;                                               // штам остановки активирован
unsigned long maxTimeIntrvl;                                            // верхняя граница времени порога срабатывания геркона (при минимальнйо скорости)
unsigned long minTimeIntrvl;                                            // нижняя граница времени порога срабатывания геркона  (при максимальнйо скорости)

// характеристики пути в течении всего времени
unsigned long totalDistance = 0;                                        // общий путь в км
int totalDays = 0;                                                      // общее время движения, часть дней

// характеристики пути
volatile unsigned long totalDistanceMM = 0;                             // общий путь в миллиметрах для точности
volatile unsigned long travelDistance = 0;                              // текущий путь в мм (динамический параметр)
volatile unsigned long travelTime = 0;	                                // время движения (динамический параметр)
volatile unsigned long totalTime = 0;                                   // общее время движения, часть минут и часов
volatile float curSpeed = 0.0;                                          // текущая скорость
volatile float maxSpeed = 0.0;                                          // максимальня скорость

// характеристики здоровья велосепидиста
unsigned long curCal = 0;                                               // текущее количество сожженных ккал
int BPM = 0;                                                            // текущее сердцебиение

// характеристики текущего пути в интервале  1 минуты
// требуемые для записи на SD карту
int dynBurned = 0;                                                      // количество сожженых ккал
unsigned long dynDist = 0;                                              // пройденная дистанция
float dynSpeed = 0.0;                                                   // средняя скорость
int countDynAvgSpeed = 0;                                               // счетчик регистраций скорости (для расчета средней скорости)
int dynHR = 0;                                                          // среднее сердцебиение
int countDynAvgHR = 0;                                                  // счетчик регистраций сердцебиения (для расчета среднего сердцебиения)

// временны'е переменные - штампы
volatile unsigned long lastCycleTurnTime = 0;	                          // время от посленей фиксации оборота колеса (для расчета скорости)
unsigned long stopTime = 0;                                             // штамп времени остановки
unsigned long lifeCycleTime = 0;                                        // переменная для отсчета времени необходимости обновления экрана

// характеристика колеса для расчета
float cycleLengthValue = 3.600;                                         // длина окружности колеса в метрах * 3.6 для вычисления скорости
int cycleLengthValueMM = 1000;                                          // длина окружности колеса в миллиметрах

// переменные связанные с отображением на экранах
byte timeModeSet = 2;	                                                  // позиция при настройке времени (начальная минуты)
byte menuPosition = 1;                                                  // пункт меню
byte cycleLengthCurPos = 0;                                             // позиция разряда длины колеса. Используется при более удобной настройки
bool isSettingsMenuActive = false;                                      // находимся ли в меню настроек

// переменные отвечающие за запись
// данных в EEPROM в нужное время
bool saveData = false;                                                  // требуется ли сохранить данные в EEPROM
unsigned long saveStartTimeStamp = 0;                                   // начало периода отсчета сохранения данных в EEPROM
unsigned long saveStopTimeStamp = 0;                                    // конец периода отсчета сохранения данных в EEPROM

void setup() {
  // pinMode(4, OUTPUT);                                                // временная настройка пина на время тестов
  Serial.begin(115200);                                                 // настраиваем сериал
  initLCD();                                                            // инициализируем основные настрйоки экрана
  readDataFromEEPROM();                                                 // инициализируем начальное значение переменных сохраннеых в EEPROM
  time.begin();                                                         // запуск работы с часами
  initButtons();                                                        // инициализируем обработчик кнопок
  initRadio();                                                          // инициализируем радио передатчик
  initSDCard();                                                         // инициализируем внешнюю карту памяти
  initSpeedRegistarator();                                              // инициализируем регистратор скорости
}

void loop() {
/* времененный генератор скорости
  writeDynDataToSD();
  digitalWrite(4, HIGH);
  digitalWrite(4, LOW);
  delay(100);*/
  buttonsHandler();                                                     // проверяем есть ли события на кнопке
  if (isMovement) {                                                     // если мы движемся
    writeDataToRadio(true);                                             // отправляем запрос пульсометру о начале считывания
    if (millis() - lastCycleTurnTime > maxTimeIntrvl) {                 // значение интервала для сброса при скорости ниже 3км/ч
      travelDynCharReset();                                             // сбрасываем динамические параметры
      writeDataToRadio(false);                                          // отправляем запрос пульсометру о конце считвания
    }
    if (totalDistanceMM >= 1000000) {                                   // елси путь стал = 1км
      totalDistance++;                                                  // глоабльная дистанция (в км) увеличивается
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
    // ни кто не едит сутки без единой остоновки
    if (travelTime >= millisecondsOf24Hours) {                          // если текущее время дистанции больше 24 часов
      travelTime = travelTime - millisecondsOf24Hours;                  // зануляем время и прибовляем хвост
    }
    if (saveStartTimeStamp == 0) {  //  обдумать как вынести за пределы функции для объединения со стопом + плюс добавить сейв карты памяти раз в минуту
      saveStartTimeStamp = millis();
    } else if (saveStopTimeStamp - saveStartTimeStamp >= 300000) {      // 5 минут
      saveDataAtEEPROM();                                               // сохраняем данные в постоянную память
      saveStartTimeStamp = 0;                                            
    }
    saveStopTimeStamp = millis();
    readDataFromRadio();                                                // читаем данные с пульсометра если они доступны
  } else {
    if (stopHandler) {                                                  // после остановки если нет движения 
      if (millis() - stopTime >= 900000) {                              // 15 минут
        saveDataAtEEPROM();                                             // сохраняем данные
        resetTravelChar();                                              // обнуляем динамические параметры второго типа
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
      if ((menuPosition == 2 && isSettingsMenuActive)                   // если в меню настроек 2
      ||(menuPosition == 1 && !isSettingsMenuActive) ) {                 // или в меню отображения данных в меню с часами(для обновления часов)
        redrawValues = true;                                            // требуется обновить экран по времени для мерцания значения часов
      } else {
        redrawValues = false;                                           // иначе не требуется
      }
    }
    if (statusBLE > 0) {                                                // если активен блюпуп
      if (statusBLE == 2)                                               // если режим передачи динамических данных
        sendDynDataToDevice();                                          // шлем данные
      if (millis() - requestResetTimeStamp > 20000)                     // интервал ожидания в 20 секунд
        statusBLE = 0;                                                  // обнуляем статус
    }
    lifeCycleTime = millis();                                           // фиксируем время для следующего обновления экрана
  }
}

