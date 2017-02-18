#define GERCON_PIN 2// пин геркона = 2

void attachInt() {
  attachInterrupt(digitalPinToInterrupt(GERCON_PIN),
                  travelDynCharRegistrator, RISING);	                                  // установка прерывания на смены 0-1
}

void detachInt() {
  detachInterrupt(digitalPinToInterrupt(GERCON_PIN));
}

void initSpeedRegistarator() {                                            // инициализация при запуске
  calculateMaxMinTimeForSpeedReg();                                       // расчитываем пороги срабатывания геркона
  pinMode(GERCON_PIN, INPUT);	                                            // установка пина на вход для геркона
  attachInt();

}

void travelDynCharRegistrator() {                                         // регистратор скорости, пути и времени пути
  detachInt();                                                            // запрещаем прерывания
  if (!isMovement) {                                                      // если первый оборот колеса за 5сек то пропускаем его только фиксируя время
    isMovement = true;                                                    // в движении
    lastCycleTurnTime = millis();                                         // время последнего оборота колеса
  } else {
    unsigned long timeInterval = millis() - lastCycleTurnTime;            // интервал между оборотом колеса, для расчета скорости
    if (timeInterval <= maxTimeIntrvl                                     // если время меньше чем мин допустимого для расчета от 3км/ч
        && timeInterval >= minTimeIntrvl) {                               // небольшая защита от шума, если прерывание произошло тут же то пропустить
      // длину колеса умножаем на время между оборотами
      // делим на отрвыок времени за который пройдено
      // сие растояние и переводим в км/ч из м/с
      curSpeed = (float)cycleLengthValue / ((float)timeInterval / 1000.0);// текущая скорость

      if (curSpeed > maxSpeed) {                                          // если текущая скорость окозалась больше максимальной округленной
        maxSpeed = curSpeed;
      }

      dynDist  += cycleLengthValueMM;                                     // складируем путь для записи на карту
      dynSpeed += curSpeed;                                               // складируем скорость для записи на карту
      countDynAvgSpeed++;                                                 // увеличиваем количество замеров скорости для записи на карту
      
      // так же записываем важные параметры
      travelDistance += cycleLengthValueMM;                               // текущий путь в мм для точности
      totalDistanceMM += cycleLengthValueMM;                              // глобальный путь в мм для точности
      travelTime += timeInterval;                                         // текущее время пути
      totalTime += timeInterval;                                          // глобальное время пути

      lastCycleTurnTime = millis();                                       // время последнего оборота колеса
      isMovement = true;
      redrawValues = true;                                                // разрешаем перерисовать значения
      saveSdData = true;
    }
    Serial.print("out->");
    Serial.println(travelDistance);
  }
  attachInt();                                                            // рарешаем прерывание.
}

// сброс динамических параметров
void resetTravelDynChar() {
  detachInt();                                                            // запрещаем прерывания тк если вдруг начнется прерывание то значения сбросятся
  isMovement = false;                                                     // уже не движемся
  stopHandler = true;                                                     // разрешаем проверять критерий полной остановки 15 минут
  redrawValues = true;                                                    // разрешаем перерисовать значения
  stopTime = millis();                                                    // пишем время остановки
  lastCycleTurnTime = 0;                                                  // время последнего оборота
  curSpeed = 0.0;                                                         // текущая скорость
  BPM = 0;                                                                // текущий пульс
  saveStopTimeStamp = 0;
  attachInt();                                                            // рарешаем прерывание
}

void resetTravelChar() {
  stopHandler = false;                                                    // остановка в 15 минут обработана и больше не требует проверки
  redrawValues = true;                                                    // разрешаем перерисовать значения
  travelTime = 0;                                                         // сброс времени пути
  travelDistance = 0;                                                     // сброс пройденой дистанции
}

void resetDynCharSD() {
  dynBurned = 0;                                                          // количество сожженых ккал
  dynDist = 0;                                                            // пройденная дистанция
  dynSpeed = 0.0;                                                         // средняя скорость
  countDynAvgSpeed = 1;                                                   // счетчик регистраций скорости (для расчета средней скорости)
  dynHR = 0;                                                              // среднее сердцебиение
  countDynAvgHR = 1;
}

void calculateMaxMinTimeForSpeedReg() {
  maxTimeIntrvl = cycleLengthValue * 334;                                 // 334 = 1(3,6 внесено в радиус колеса)(перевод с метрво в км/ч )/3 (км/ч) * 1000 мс
  minTimeIntrvl = cycleLengthValue * 5;                                   // 5 = (3,6 внесено в радиус колеса)(перевод с метрво в км/ч )/200 (км/ч) * 1000 мс
}

void resetAchievements() {                                                // расстреливаем из квантовой пушки данные и зануляем их
  totalDays = 0;
  totalDistance = 0;
  totalDistanceMM = 0;
  totalTime = 0;
  saveDataAtEEPROM();
}
