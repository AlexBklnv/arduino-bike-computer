//герокон
volatile bool isInterruptGerconRunning = false;                           // запущенно ли прерывание геркона
bool isFirstTurn = true;
const byte gerconPin = 2;                                                 // пин геркона

void initSpeedRegistarator() {
  pinMode(2, INPUT);	                                                  // установка пина на вход для геркона
  attachInterrupt(0, SpeedRegistrator, RISING);	                          // установка прерывания на смены 0-1
}

// регистратор скорости
void SpeedRegistrator() {
  noInterrupts();                                                       // запрещаем прерывания

  if (isFirstTurn) {
    isFirstTurn = false;
    lastCycleTurnTime = millis();                                         // время последнего оборота колеса
  } else {
    unsigned long timeInterval = millis() - lastCycleTurnTime;        // интервал между оборотом колеса, для расчета скорости
    if (timeInterval < maxTimeIntrvl                                  // если время меньше чем мин допустимого для расчета от 3км/ч
        && timeInterval > minTimeIntrvl) {                                                // небольшая защита от шума, если прерывание произошло тут же то пропустить
      curSpeed = cycleLengthValue / ((float)timeInterval / 1000.0) * 3.6; // длину колеса умножаем на время между оборотами
      // делим на отрвыок времени за который пройдено
      // сие растояние и переводим в км/ч из м/с
      if (curSpeed > MaxSpeed) {                                          // если текущая скорость окозалась больше максимальной
        MaxSpeed = curSpeed;                                              // то переопределяем максимальную скорость
      }
      Serial.print("V=");
      Serial.print(curSpeed, 1);
      Serial.println();
      Serial.print("T=");
      Serial.print(timeInterval);
      Serial.println();
      // так же записываем важные параметры
      travelDistance += cycleLengthValueMM;                               // текущий путь в мм для точности
      totalDistanceMM += cycleLengthValueMM;                              // глобальный путь в мм для точности
      travelTime += timeInterval;                                         // текущее время пути
      totalTime += timeInterval;                                          // глобальное время пути

      lastCycleTurnTime = millis();                                       // время последнего оборота колеса
      redrawValues = true;                                                // разрешаем перерисовать значения
      isMovement = true;                                                  // переназначаем значение флага
    }
  }
  interrupts();                                                         // рарешаем прерывание
}

// сброс динамических параметров
void SpeedReset() {
  noInterrupts();                                                         // запрещаем прерывания тк если вдруг начнется прерывание то значения сбросятся
  isMovement = false;                                                     // уже не движемся
  stopTime = lastCycleTurnTime;
  stopHandler = true;
  isFirstTurn = true;
  lastCycleTurnTime = 0;                                                  // время последнего оборота
  curSpeed = 0;                                                           // текущая скорость
  BPM = 0;                                                                // текущий пульс
  redrawValues = true;                                                    // разрешаем перерисовать значения
  interrupts();                                                           // рарешаем прерывание
}

void ResetTravelChar() {
  stopHandler = false;
  travelTime = 0;
  travelDistance = 0;
}

void calculateMaxTimeForSpeedRegistration() {
  maxTimeIntrvl = cycleLengthValue * 1200;                                          // 1200 = 3,6(перевод с метрво в км/ч )/3 (км/ч) * 1000 мс
}

void calculateMinTimeForSpeedRegistration() {
  minTimeIntrvl = cycleLengthValue * 18;                                            // 18 = 3,6(перевод с метрво в км/ч )/200 (км/ч) * 1000 мс
}

