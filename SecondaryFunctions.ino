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

