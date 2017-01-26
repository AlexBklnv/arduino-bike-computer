// функция упростит ввод длины окружности колеса
// ввод производится на каждом разряде, а не увеличение числа от 1000-9999
// изъят цифру из числа длины колеса
byte getDigitFromCycleLengtValue(byte posOfDigit)
{
  byte tmp;
  switch (posOfDigit) { // поразрядно слева направо
    case 0:
      tmp = (int)cycleLengthValueMM / 1000;
      break;
    case 1:
      tmp = (int)(cycleLengthValueMM / 100) % 10;
      break;
    case 2:
      tmp = (int)(cycleLengthValueMM % 100) / 10;
      break;
    case 3:
      tmp = (int)cycleLengthValueMM % 10;
      break;
  }
  return tmp;
}

void calculateMaxMinTimeForSpeedReg() {
  calculateMaxTimeForSpeedRegistration();
  calculateMinTimeForSpeedRegistration();
}

void calculateMaxTimeForSpeedRegistration() {                             // расчет времени при котором движение начинается от 3км/ч
  maxTimeIntrvl = cycleLengthValue * 1200;                                // 1200 = 3,6(перевод с метрво в км/ч )/3 (км/ч) * 1000 мс
}

void calculateMinTimeForSpeedRegistration() {                             // расчет времени при котором движение достигает 200км/ч максимально допустимая скорость
  minTimeIntrvl = cycleLengthValue * 18;                                  // 18 = 3,6(перевод с метрво в км/ч )/200 (км/ч) * 1000 мс
}

void resetAchievements() {
  totalDays = 0;
  MaxSpeed = 0.0;
  totalDistance = 0;
  totalDistanceMM = 0;
  totalTime = 0;
  saveDataAtEEPROM();
}
