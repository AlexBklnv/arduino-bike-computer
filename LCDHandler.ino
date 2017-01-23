// инициализация экрана
void initLCD() {
  lcd.init();                                                        // инициализация дисплея
  lcd.backlight();                                                   // Включаем подсветку дисплея
  lcd.setCursor(0, 0);
  lcd.print("Loading...");
  initCustomChar();                                                  // инициализация собственных символов
}

// вывод на экран длины колеса
void printCycleLengthLCD() {
  for (int i = 0; i < 4; i++) {
    lcd.setCursor(i, 1);
    lcd.print(getDigitFromCycleLengtValue(i));
  }
}

// очистка поля значения от старых данных
void clearValue(byte col, byte row, byte rowLength) {
  lcd.setCursor(col, row);
  for (int i = 0; i < rowLength; i++) {
    lcd.print(" ");
  }
  lcd.setCursor(col, row);
}

// вывод текущей скорости
void printCurSpeed() {
  clearValue(0, 0, 5);
  lcd.print(curSpeed, 1);
}

// вывод текущей дистанции
void printTravelDistance() {
  clearValue(4, 1, 6);
  lcd.print(round(travelDistance / 1000));
}

// вывод времени
void printTime(unsigned long time) {
  if (time / 60 / 60 < 10)                                      // если часов меньше 10 то пишем 0
    lcd.print("0");
  lcd.print(int(time / 60 / 60));                               // пишем колчичество часов
  lcd.print(":");
  if (time / 60 % 60 < 10)                                      // вычисляем минуты
    lcd.print("0");
  lcd.print((int)(time / 60 % 60));
}

void printCurSpeedTittle() {
  lcd.setCursor(6, 0);                                     // после значения километража пишем измерение
  lcd.print("KMH");
}

void printCurDistanceTittle() {
  lcd.setCursor(0, 1);
  lcd.print("DST ");                                         // текущая дистанция
  lcd.setCursor(11, 1);
  lcd.print("m.");                                         // измерение дистанции
}

// вывод экранов без значений
void printCurrentScreenTittles() {
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (menuPosition) {
    // экран текущей скорости, дистанции и времени пути.
    case 0:
      printCurSpeedTittle();
      lcd.setCursor(10, 0);
      lcd.write(travelClockIndex);                                         // текущее время пути
      printCurDistanceTittle();
      break;
    // экран текущей скорости, дистанции и времени (по настроеной дате в часовом модуле)
    case 1:
      printCurSpeedTittle();
      lcd.setCursor(10, 0);
      lcd.write(clockIndex);                                   // время по текущей дате
      printCurDistanceTittle();
      break;
    case 2:
      printCurSpeedTittle();
      lcd.setCursor(11, 0);
      lcd.write(heartIndex);                                  // знак сердеечка
      lcd.setCursor(7, 1);
      lcd.print("cal.");                                      // калории
      break;
    case 3:
      printCurSpeedTittle();
      lcd.setCursor(0, 1);
      lcd.print("Max V ");                                      // максимальная скорость
      lcd.setCursor(11, 1);
      lcd.print("KMH");
      break;
    case 4:
      lcd.print("TD=");                                        // дистанция за все время total distance
      lcd.setCursor(14, 0);
      lcd.print("KM");
      lcd.setCursor(0, 1);
      lcd.print("TT=");                                        // время пути за все время использования total time
      break;
  }
}

void printCurrnetScreenValues() {
  switch (menuPosition) {
    // экран текущей скорости, дистанции и времени пути.
    case 0:
      printCurSpeed();                                       // печать значения текущей скорости
      lcd.setCursor(11, 0);
      printTime(travelTime);                                 // печать значения времени
      printTravelDistance();                                 // печать значение дистанции
      break;
    // экран текущей скорости, дистанции и времени (по настроеной дате в часовом модуле)
    case 1:
      printCurSpeed();
      lcd.setCursor(11, 0);
      lcd.print(time.gettime("H:i"));                        // печать текущего времени
      printTravelDistance();
      break;
    case 2:
      printCurSpeed();
      clearValue(13, 0, 3);
      lcd.print(BPM);
      clearValue(0, 1, 6);
      lcd.print(curCal);                                      // калории
      break;
    case 3:
      printCurSpeed();                                        // максимальная скорость
      clearValue(6, 1, 5);
      lcd.print(MaxSpeed, 1);
      break;
    case 4:
      clearValue(3, 0, 6);
      lcd.print(totalDistance / 1000);
      clearValue(3, 1, 9);
      if (totalDays < 100)
        lcd.print("0");
      if (totalDays < 10)
        lcd.print("0");
      lcd.print(totalDays);
      lcd.print(":");
      printTime(totalTime);
      break;
  }
}

void printCurrentScreenSettingsTittles() {
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (settingPosition) {
    case 0:
      lcd.print("Set cycle length");
      lcd.setCursor(5,1);
      lcd.print("mm.");
      lcd.setCursor(settingsCursorPosition, 1);
      lcd.blink();
      break;
    case 1:
      printDateTimeSettings();
      break;
    case 2:
      lcd.print("Reset your ");
      lcd.write(achievementIndex);
      lcd.setCursor(0, 1);
      lcd.print("yes(1)");
      break;
  }
}

void printDateTimeSettings() {
  lcd.setCursor(0, 0);
  lcd.print(time.gettime("H:i"));
  lcd.setCursor(0, 1);
  lcd.print(time.gettime("d-m-Y"));
  time.blinktime(timeModeSet);
}

void printCurrentScreenSettingsValues() {
  switch (settingPosition) {
    case 0:
      printCycleLengthLCD();
      lcd.setCursor(settingsCursorPosition, 1);
      break;
    case 1:
      printDateTimeSettings();
      break;
    case 2:
      break;
  }
}
