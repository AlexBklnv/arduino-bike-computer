// регистратор скорости
void SpeedRegistrator() {
  if (isInterruptGerconRunning == false) {                     // если прерывание не запущено то выполнить                              
    isInterruptGerconRunning = true;
    unsigned long timeInterval = millis() - lastCycleTurnTime;
    if (timeInterval > 0) {                                    // небольшая защита от шума, если прерывание произошло тут же то пропустить
      countOfCycleTurns++;                                     // увеличиваем количество оборотов
      //  переводим длину колеса в метры и умножаем на количество оборотов что бы знать пройденный путь
      // делим на отрвыок времени за который пройдено сие растояние и переводим в км/ч из м/с
      curSpeed = cycleLengthValue / ((float)timeInterval / 1000.0) * 3.6;
      curDistance += cycleLengthValue;
      lastCycleTurnTime = millis();
    }
    delay(5);
  }
}

// сброс динамических параметров
void SpeedReset() {
  countOfCycleTurns = 0;                                             // количество оборотов
  lastCycleTurnTime = 0;                                             // время последнего оборота
  curSpeed = 0;                                                      // текущая скорость
  curPulse = 0;                                                      // текущий пульс
} 
