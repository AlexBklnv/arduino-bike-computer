bool isFirstTurn = true;                                                  // Отвечает за трек первого оборота
const byte gerconPin = 2;                                                 // пин геркона

unsigned long maxTimeIntrvl;                                              // верхняя граница времени порога срабатывания геркона (при минимальнйо скорости)
unsigned long minTimeIntrvl;                                              // нижняя граница времени порога срабатывания геркона  (при максимальнйо скорости)

void attachInt(){
  attachInterrupt(0, travelDynCharRegistrator, RISING);	                  // установка прерывания на смены 0-1
}

void detachInt(){
    detachInterrupt(0);	                  
}

void initSpeedRegistarator() {                                            // инициализация при запуске
  pinMode(2, INPUT);	                                                  // установка пина на вход для геркона
  attachInt();
}

void travelDynCharRegistrator() {                                         // регистратор скорости, пути и времени пути
  noInterrupts();                                                         // запрещаем прерывания

  if (isFirstTurn) {                                                      // если первый оборот колеса за 5сек то пропускаем его только фиксируя время
    isFirstTurn = false;
    isMovement = true;                                                    // в движении
    lastCycleTurnTime = millis();                                         // время последнего оборота колеса
  } else {
    unsigned long timeInterval = millis() - lastCycleTurnTime;            // интервал между оборотом колеса, для расчета скорости
    if (timeInterval <= maxTimeIntrvl                                     // если время меньше чем мин допустимого для расчета от 3км/ч
        && timeInterval >= minTimeIntrvl) {                               // небольшая защита от шума, если прерывание произошло тут же то пропустить

      // длину колеса умножаем на время между оборотами
      // делим на отрвыок времени за который пройдено
      // сие растояние и переводим в км/ч из м/с
      curSpeed = cycleLengthValue / ((float)timeInterval / 1000.0) * 3.6; // текущая скорость

      if (curSpeed > (float)maxSpeed) {                                          // если текущая скорость окозалась больше максимальной округленной 
        if (curSpeed > getMaxSpeed())                                       // то берем релаьное значение из памяти и сверяем
          setMaxSpeed(curSpeed);                                          // то переопределяем максимальную скорость
      }

      // так же записываем важные параметры
      travelDistance += cycleLengthValueMM;                               // текущий путь в мм для точности
      totalDistanceMM += cycleLengthValueMM;                              // глобальный путь в мм для точности
      travelTime += timeInterval;                                         // текущее время пути
      totalTime += timeInterval;                                          // глобальное время пути

      lastCycleTurnTime = millis();                                       // время последнего оборота колеса
      redrawValues = true;                                                // разрешаем перерисовать значения
      isMovement = true;
    }
  }
  interrupts();                                                           // рарешаем прерывание
}

// сброс динамических параметров
void travelDynCharReset() {
  noInterrupts();                                                         // запрещаем прерывания тк если вдруг начнется прерывание то значения сбросятся
  isMovement = false;                                                     // уже не движемся
  stopTime = millis();                                                    // пишем время остановки
  stopHandler = true;                                                     // разрешаем проверять критерий полной остановки 15 минут
  isFirstTurn = true;                                                     // обнуляем значение параметра первого оборота
  lastCycleTurnTime = 0;                                                  // время последнего оборота
  curSpeed = 0;                                                           // текущая скорость
  BPM = 0;                                                                // текущий пульс
  redrawValues = true;                                                    // разрешаем перерисовать значения
  saveStopTimeStamp = 0;
  interrupts();                                                           // рарешаем прерывание
}

void resetTravelChar() {
  stopHandler = false;                                                    // остановка в 15 минут обработана и больше не требует проверки
  travelTime = 0;                                                         // сброс времени пути
  travelDistance = 0;                                                     // сброс пройденой дистанции
  redrawValues = true;                                                    // разрешаем перерисовать значения
}
