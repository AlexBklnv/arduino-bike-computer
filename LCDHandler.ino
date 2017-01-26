byte pinLCDBrightness = 3;                                           // пин регулировки яркости экрана

void initLCD() {                                                     // инициализация экрана
  lcd.init();                                                        // инициализация дисплея
  setBrightness();                                                   // установка яркости экрана
  lcd.backlight();                                                   // Включаем подсветку дисплея
  lcd.setCursor(0, 0);
  lcd.print("Loading...");
  initCustomChar();                                                  // инициализация собственных символов
}

void clearValue(byte col, byte row, byte rowLength) {                // очистка поля значения от старых данных
  lcd.setCursor(col, row);                                           // установка значения курсора в начальное положение
  for (int i = 0; i < rowLength; i++) {                              // зачистка экрана в максимальную длину значения
    lcd.print(" ");
  }
  lcd.setCursor(col, row);                                           // установка положения курсора в нулевое положение значения
}

void printCurSpeed() {                                               // вывод значения текущей скорости
  clearValue(0, 0, 5);
  lcd.print(curSpeed, 1);
}

void printTravelDistance() {                                         // вывод значения текущей дистанции пути
  clearValue(4, 1, 6);
  lcd.print(round(travelDistance / 1000));                           // вывод в метрах
}

void printTime(unsigned long time) {                                 // вывод времени в формате HH:mm
  time /= 1000;
  if (time / 60 / 60 < 10)                                           // если часов меньше 10 то пишем 0
    lcd.print("0");
  lcd.print(int(time / 60 / 60));                                    // пишем колчичество часов
  lcd.print(":");
  if (time / 60 % 60 < 10)                                           // вычисляем минуты
    lcd.print("0");
  lcd.print((int)(time / 60 % 60));
}

void printCurSpeedTittle() {                                         // вывод заголовка скорости на экран
  lcd.setCursor(6, 0);                                               // позиция после значения километража пишем измерение
  lcd.print("KMH");
}

void printCurDistanceTittle() {                                      // вывод заголовка текущей дистанции на экран
  lcd.setCursor(0, 1);
  lcd.print("DST");
  lcd.setCursor(11, 1);
  lcd.print("m.");
}

void printCurrentScreenTittles() {                                   // вывод заголовков экранов
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (menuPosition) {
    case 0:                                                          // экран текущей скорости, дистанции и времени пути.
      printCurSpeedTittle();
      lcd.setCursor(10, 0);
      lcd.write(travelClockIndex);
      printCurDistanceTittle();
      break;
    case 1:                                                          // экран текущей скорости, дистанции и времени (по настроеной дате в часовом модуле)
      printCurSpeedTittle();
      lcd.setCursor(10, 0);
      lcd.write(clockIndex);
      printCurDistanceTittle();
      break;
    case 2:                                                          // экран скорости, сердцебиения и калорий
      printCurSpeedTittle();
      lcd.setCursor(11, 0);
      lcd.write(heartIndex);
      lcd.setCursor(7, 1);
      lcd.print("cal.");
      break;
    case 3:                                                         // экран текущей скорости и максимальной скорости
      printCurSpeedTittle();
      lcd.setCursor(0, 1);
      lcd.print("Max V");
      lcd.setCursor(11, 1);
      lcd.print("KMH");
      break;
    case 4:                                                         // экран глоабльной дистанции и времени пути
      lcd.print("TD=");
      lcd.setCursor(14, 0);
      lcd.print("KM");
      lcd.setCursor(0, 1);
      lcd.print("TT=");
      break;
  }
}

void printCurrnetScreenValues() {                                   // вывод значений динамический параметров в зависимости от экрана
  switch (menuPosition) {
    case 0:                                                         // экран текущей скорости, дистанции и времени пути.
      printCurSpeed();
      lcd.setCursor(11, 0);
      printTime(travelTime);
      printTravelDistance();
      break;
    case 1:                                                         // экран текущей скорости, дистанции и времени (по настроеной дате в часовом модуле)
      printCurSpeed();
      lcd.setCursor(11, 0);
      lcd.print(time.gettime("H:i"));
      printTravelDistance();
      break;
    case 2:                                                         // экран скорости, сердцебиения и калорий
      printCurSpeed();
      clearValue(13, 0, 3);
      lcd.print(BPM);
      clearValue(0, 1, 6);
      lcd.print(curCal);
      break;
    case 3:                                                         // экран текущей скорости и максимальной скорости
      printCurSpeed();
      clearValue(6, 1, 5);
      lcd.print(MaxSpeed, 1);
      break;
    case 4:                                                         // экран глоабльной дистанции и времени пути
      clearValue(3, 0, 6);
      lcd.print(totalDistance);
      clearValue(3, 1, 9);
      if (totalDays < 100)                                          // форматированный вывод DDD:HH:mm
        lcd.print("0");
      if (totalDays < 10)
        lcd.print("0");
      lcd.print(totalDays);
      lcd.print(":");
      printTime(totalTime);
      break;
  }
}

void setBrightness() {                                              // установка яркости экрана
  analogWrite(pinLCDBrightness, brightness);
}

void printCurrentScreenSettingsTittles() {                          // вывод заголовков настроек
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (settingPosition) {                                        // экран установки яркости
    case 0:
      lcd.print("Set brightness");
      lcd.setCursor(3, 1);
      lcd.print("%");
      break;
    case 1:                                                         // экран установки длины окружности колеса
      lcd.print("Set cycle length");
      lcd.setCursor(5, 1);
      lcd.print("mm.");
      settingsCursorPosition = 0;
      lcd.setCursor(settingsCursorPosition, 1);
      lcd.blink();
      break;
    case 2:                                                         // экран настройки даты и времени
      lcd.noBlink();
      printDateTimeSettings();
      break;
    case 3:                                                         // экран сброса
      lcd.print("Reset your ");
      lcd.write(achievementIndex);
      lcd.setCursor(0, 1);
      lcd.print("yes(1)");
      break;
  }
}

void printDateTimeSettings() {                                      // функция вывода времени для его настроки в модуле RTC DS3231
  lcd.setCursor(0, 0);
  lcd.print(time.gettime("H:i"));
  lcd.setCursor(0, 1);
  lcd.print(time.gettime("d-m-Y"));
  time.blinktime(timeModeSet);
}

void printCurrentScreenSettingsValues() {                           // вывод значений на экранах настроек
  switch (settingPosition) {
    case 0:                                                         // вывод яркости в процентах
      clearValue(0, 1, 3);
      lcd.print(round(brightness / 2.55));
      break;
    case 1:                                                       // вывод длины окружности колеса
      lcd.print(cycleLengthValueMM);
      lcd.setCursor(settingsCursorPosition, 1);
      break;
    case 2:                                                         // вывод даты
      printDateTimeSettings();
      break;
    case 3:
      break;
  }
}

void printMSG(byte col, byte row, String msg) {
  lcd.setCursor(col, row);
  lcd.print(msg);
}
