// инициализация экрана
void initLCD() {
  lcd.init();                                                        // инициализация дисплея
  lcd.backlight();                                                   // Включаем подсветку дисплея
  setCursorLCD(0, 0);
  sendDataToLCD("Loading...");
  initCustomChar();                                                  // инициализация собственных символов
}

// вывод на экран длины колеса
void printCycleLengthLCD() {
  for (int i = 0; i < 4; i++) {
    lcd.setCursor(i, 1);
    lcd.print(getDigitFromCycleLengtValue(i));
  }
}


void printCurrentScreenTittles() {
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (menuPosition) {
    // экран текущей скорости, дистанции и времени пути.
    case 0:
      lcd.print("S=");                                         // заголовок скорости
      lcd.setCursor(8, 0);                                     // смещение на позицию для печати дистанции
      lcd.print("D=");                                         // заголовок дистанции
      lcd.setCursor(0, 1);                                     // перенос строки для печати времени
      lcd.print("T=");                                         // текущее время пути
      break;
    // экран текущей скорости, дистанции и времени (по настроеной дате в часовом модуле)
    case 1:
      lcd.print("S=");                                         // заголовок скорости
      lcd.setCursor(8, 0);                                     // смещение на позицию для печати дистанции
      lcd.print("D=");                                         // заголовок дистанции
      lcd.setCursor
      lcd.write(clockIndex);                                   // время по текущей дате
      break;
    case 2:
      lcd.print("S=");                                         // заголовок скорости
      lcd.setCursor(8, 0);                                     // смещение на позицию для печати пульса
      lcd.write(heartIndex);                                   // знак сердеечка
      lcd.setCursor(0, 1);
      lcd.print("Cal.=");                                      // калории
      break;
    case 3:
      lcd.print("S=");
      lcd.setCursor(0, 1);
      lcd.print("MaxS=");                                      // максимальная скорость
      break;
    case 4:
      lcd.print("TD=");                                        // дистанция за все время total distance
      lcd.setCursor(0, 1);
      lcd.print("TT=");                                        // время пути за все время использования total time
      break;
    case 5:
      lcd.print("Set cycle length");
      lcd.setCursor(0, 1);
      printCycleLengthLCD();
      break;
    case 6:
      lcd.print(time.gettime("H:i"));
      lcd.setCursor(0, 1);
      lcd.print(time.gettime("d-m-Y"));
      break;
    case 7:
      lcd.print("Reset your ");
      lcd.write(achievementIndex);
      lcd.setCursor(0, 1);
      lcd.print("yes(1)");
      break;
    case 8:
      lcd.print("Forget ");
      lcd.write(bluetoothIndex)
      lcd.setCursor(0, 1);
      lcd.print("device? yes(1)");
      break;
  }
}

void printCurrnetScreenValues() {

}
