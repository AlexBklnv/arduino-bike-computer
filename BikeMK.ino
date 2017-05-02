#include <SPI.h>
#include <Bounce2.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <iarduino_RTC.h>
#include <EEPROM.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "SdFat.h"
//TODO: Я ИЗ БУДУЩЕГО!!! ЗАПОМНИ! ПРЕЖДЕ ЧЕМ ЗАГРУЖАТЬ НА ПЛАТУ ВЫЗОВИ ФУНКЦИЮ СОХРАНЕНИЯ ЗНАЧЕНИЙ В EEPROM ИЛИ ВСЕ ПОЙДЕТ ПО ЕЛАДКУ!

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
int weight = 50;
// характеристики текущего пути в интервале  1 минуты
// требуемые для записи на SD карту
/* E = 0.014*G*t*(0.12*f-7)
  где  E – энергозатраты в килокалориях,
      G – масса тела в килограммах,
      t – время в минутах,
      f – число сердечных сокращений (ЧСС) в минуту.
*/
unsigned long sendDynDataTimeStamp = 0;
bool isSendDynData = false;
unsigned long dynBurned = 0;                                                      // количество сожженых ккал
unsigned long dynDist = 0;                                              // пройденная дистанция
float dynSpeed = 0.0;                                                   // средняя скорость
unsigned long countDynAvgSpeed = 1;                                     // счетчик регистраций скорости (для расчета средней скорости)
unsigned long dynHR = 0;                                                // среднее сердцебиение
unsigned long countDynAvgHR = 1;                                        // счетчик регистраций сердцебиения (для расчета среднего сердцебиения)

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
byte longValueCurPos = 0;                                             // позиция разряда длины колеса. Используется при более удобной настройки
bool isSettingsMenuActive = false;                                      // находимся ли в меню настроек

// переменные отвечающие за запись
// данных в EEPROM в нужное время
bool saveData = false;                                                  // требуется ли сохранить данные в EEPROM
unsigned long saveStartTimeStamp = 0;                                   // начало периода отсчета сохранения данных в EEPROM
unsigned long saveStopTimeStamp = 0;                                    // конец периода отсчета сохранения данных в EEPROM
bool saveSdData = false;
unsigned long saveSdStarTmStmp = 0;
unsigned long saveSdStopTmStmp = 0;


void setup() {
  //  initStartEEPROM();
  // pinMode(4, OUTPUT);                                                // временная настройка пина на время тестов
  Serial.begin(115200);                                                 // настраиваем сериал
  readDataFromEEPROM();                                                 // инициализируем начальное значение переменных сохраннеых в EEPROM
  initLCD();                                                            // инициализируем основные настрйоки экрана
  time.begin();                                                         // запуск работы с часами
  initButtons();                                                        // инициализируем обработчик кнопок
  initRadio();                                                          // инициализируем радио передатчик
  initSDCard();                                                         // инициализируем внешнюю карту памяти
  resetDynCharSD();                                                     // инициализация переменных ответственных за корерктные данные пути на SD карте
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
      resetTravelDynChar();                                             // сбрасываем динамические параметры
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
    if (isSendDynData) {
      if (sendDynDataTimeStamp - millis() >= 1000){
        isSendDynData = false;
        sendDynData();
      }
    }
  } else {
    if (stopHandler) {                                                  // после остановки если нет движения
      if (millis() - stopTime >= 900000) {                              // 15 минут
        saveDataAtEEPROM();                                             // сохраняем данные
        resetTravelChar();                                              // обнуляем динамические параметры второго типа
      }
    }
  }

  if (saveSdData) {
    if (saveSdStarTmStmp == 0) {
      saveSdStarTmStmp = millis();
    } else if (saveSdStopTmStmp - saveSdStarTmStmp >= 60 * 1000) {
      if (initSDCard()) {
        writeDynDataToSD();
        saveSdData = false;
        saveSdStopTmStmp = 0;
        saveSdStarTmStmp = 0;
      }
    }
    saveSdStopTmStmp = millis();
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
      if ((menuPosition == 3 && isSettingsMenuActive)                   // если в меню настроек 2
          || (menuPosition == 1 && !isSettingsMenuActive) ) {               // или в меню отображения данных в меню с часами(для обновления часов)
        redrawValues = true;                                            // требуется обновить экран по времени для мерцания значения часов
      } else {
        redrawValues = false;                                           // иначе не требуется
      }
    }
    lifeCycleTime = millis();                                           // фиксируем время для следующего обновления экрана
  }
}
