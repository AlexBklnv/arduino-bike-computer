//герокон
volatile bool isInterruptGerconRunning = false;     // запущенно ли прерывание геркона

const byte gerconPin = 2;                            // пин геркона

void initSpeedRegistarator(){
  pinMode(2, INPUT);	                              // установка пина на вход для геркона
  attachInterrupt(0, SpeedRegistrator, RISING);	      // установка прерывания на смены 0-1
}

// регистратор скорости
void SpeedRegistrator() {
  if (isInterruptGerconRunning == false) {                     // если прерывание не запущено то выполнить
    isInterruptGerconRunning = true;
    unsigned long timeInterval = millis() - lastCycleTurnTime + 5;
    if (timeInterval > 0) {                                    // небольшая защита от шума, если прерывание произошло тут же то пропустить
      //  переводим длину колеса в метры и умножаем на количество оборотов что бы знать пройденный путь
      // делим на отрвыок времени за который пройдено сие растояние и переводим в км/ч из м/с
      curSpeed = cycleLengthValue / ((float)timeInterval / 1000.0) * 3.6;
      if (curSpeed > MaxSpeed) {
        MaxSpeed = curSpeed;
      }
      travelDistance += cycleLengthValue;
      if (totalDistance >= 999999000)
        totalDistance = 0;
      totalDistance += cycleLengthValue / 1000;

      lastCycleTurnTime = millis();
      isMovement = true;
    }
    delay(5);
  }
}

// сброс динамических параметров
void SpeedReset() {
  lastCycleTurnTime = 0;                                             // время последнего оборота
  curSpeed = 0;                                                      // текущая скорость
  BPM = 0;                                                           // текущий пульс
}
