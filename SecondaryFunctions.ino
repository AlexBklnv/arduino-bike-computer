// функция упростит ввод длины окружности колеса
// ввод производится на каждом разряде, а не увеличение числа от 1000-9999
// изъят цифру из числа длины колеса
byte getDigitFromCycleLengtValue(byte numberOfDigit)
{
  byte tmp;
  switch (numberOfDigit) { // поразрядно слева направо
    case 0:
      tmp = (int)cycleLengthValue / 1000;
      break;
    case 1:
      tmp = (int)cycleLengthValue / 100;
      tmp = tmp % 10;
      break;
    case 2:
      tmp = cycleLengthValue % 100;
      tmp = (int)tmp / 10;
      break;
    case 3:
      tmp = cycleLengthValue % (int)10;
      break;
  }
  return tmp;
}
