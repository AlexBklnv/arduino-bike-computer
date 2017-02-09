#define BRIGHTNESS_PIN 3                                             // пин регулировки яркости экрана = 3

void initLCD() {                                                     // инициализация экрана
  lcd.init();                                                        // инициализация дисплея
  setBrightnessLCD();                                                // установка яркости экрана
  lcd.backlight();                                                   // Включаем подсветку дисплея
  initCustomChar();                                                  // инициализация собственных символов
}

void setBrightnessLCD() {                                            // установка яркости экрана
  analogWrite(BRIGHTNESS_PIN, brightness);
}

void clearValue(byte col, byte row, byte rowLength) {                // очистка поля значения от старых данных
  lcdSetCursor(col, row);                                            // установка значения курсора в начальное положение
  for (byte i = 0; i < rowLength; i++) {                             // зачистка экрана в максимальную длину значения
    lcd.print(F(" "));
  }
  lcdSetCursor(col, row);                                            // установка положения курсора в нулевое положение значения
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
  time /= 60000;
  if (time / 60 < 10)                                                // если часов меньше 10 то пишем 0
    lcd.print(F("0"));
  lcd.print((byte)(time / 60));                                      // пишем колчичество часов
  if (time % 60 < 10)                                                // вычисляем минуты
    lcd.print(F(":0"));
  else
    lcd.print(F(":"));
  lcd.print((byte)(time % 60));
}

void printCurSpeedTittle() {                                         // вывод заголовка скорости на экран
  lcdSetCursor(6, 0);                                                // позиция после значения километража пишем измерение
  lcd.print(F("KMH"));
}

void printCurDistanceTittle() {                                      // вывод заголовка текущей дистанции на экран
  lcdSetCursor(0, 1);
  lcd.print(F("DST"));
  lcdSetCursor(11, 1);
  lcd.print(F("m."));
}

void lcdSetCursor(byte col, byte row) {                              // установка курсора в его позицию
  lcd.setCursor(col, row);
}

void lcdWrite(byte index) {                                          // вывод спец символов
  lcd.write(index);
}

void lcdClear() {                                                    // чистим экран
  lcd.clear();
}

void printCurrentScreenTittles() {                                   // вывод заголовков экранов
  lcdClear();
  lcdSetCursor(0, 0);
  if (menuPosition == 0) {                                           // экран текущей скорости, дистанции и времени пути.
    printCurSpeedTittle();                                            
    lcdSetCursor(10, 0);
    lcdWrite(travelClockIndex);
    printCurDistanceTittle();
  } else if (menuPosition == 1) {                                    // экран текущей скорости, дистанции и времени (по настроеной дате в часовом модуле)
    printCurSpeedTittle();
    lcdSetCursor(10, 0);
    lcdWrite(clockIndex);
    printCurDistanceTittle();
  } else if (menuPosition == 2) {                                    // экран скорости, сердцебиения и калорий
    printCurSpeedTittle();
    lcdSetCursor(11, 0);
    lcdWrite(heartIndex);
    lcdSetCursor(7, 1);
    lcd.print(F("cal."));
  } else if (menuPosition == 3) {                                    // экран текущей скорости и максимальной скорости
    printCurSpeedTittle();
    lcdSetCursor(0, 1);
    lcd.print(F("Max V"));
    lcdSetCursor(11, 1);
    lcd.print(F("KMH"));
  } else if (menuPosition == 4) {                                    // экран глоабльной дистанции и времени пути
    lcd.print(F("TD="));
    lcdSetCursor(14, 0);
    lcd.print(F("KM"));
    lcdSetCursor(0, 1);
    lcd.print(F("TT="));
  }
}

void printCurrnetScreenValues() {                                    // вывод значений динамический параметров в зависимости от экрана
  if (menuPosition == 0) {                                           // экран текущей скорости, дистанции и времени пути.
    printCurSpeed();
    lcdSetCursor(11, 0);
    printTime(travelTime);
    printTravelDistance();
  } else if (menuPosition == 1) {                                    // экран текущей скорости, дистанции и времени (по настроеной дате в часовом модуле)
    printCurSpeed();
    lcdSetCursor(11, 0);
    lcd.print(time.gettime("H:i"));
    printTravelDistance();
  } else if (menuPosition == 2) {                                    // экран скорости, сердцебиения и калорий
    printCurSpeed();
    clearValue(13, 0, 3);
    lcd.print(BPM);
    clearValue(0, 1, 6);
    lcd.print(curCal);
  } else if (menuPosition == 3) {                                    // экран текущей скорости и максимальной скорости
    printCurSpeed();
    clearValue(6, 1, 5);
    lcd.print(maxSpeed, 1);
  } else if (menuPosition == 4) {                                    // экран глоабльной дистанции и времени пути
    clearValue(3, 0, 6);
    lcd.print(totalDistance);
    clearValue(3, 1, 9);
    if (totalDays < 100)                                             // форматированный вывод DDD:HH:mm
      lcd.print(F("0"));
    if (totalDays < 10)
      lcd.print(F("0"));
    lcd.print(totalDays);
    lcd.print(F(":"));
    printTime(totalTime);
  }
}

void printCurrentScreenSettingsTittles() {                           // вывод заголовков настроек
  lcdClear();
  lcdSetCursor(0, 0);
  if (menuPosition == 0) {                                           // экран установки яркости
    lcd.print(F("Set brightness"));
    lcdSetCursor(3, 1);
    lcd.print(F("%"));
  } else if (menuPosition == 1) {                                    // экран установки длины окружности колеса
    lcd.print(F("Set cycle length"));
    lcdSetCursor(5, 1);
    lcd.print(F("mm."));
    cycleLengthCurPos = 0;
    lcdSetCursor(cycleLengthCurPos, 1);
    lcd.blink();
  } else if (menuPosition == 2) {                                    // экран настройки даты и времени
    lcd.noBlink();
    printDateTimeSettings();
  } else if (menuPosition == 3) {                                    // экран сброса
    lcd.print(F("Reset your "));
    lcdWrite(achievementIndex);
    lcdSetCursor(0, 1);
    lcd.print(F("yes(1)"));
  }
}

void printDateTimeSettings() {                                       // функция вывода времени для его настроки в модуле RTC DS3231
  lcdSetCursor(0, 0);
  lcd.print(time.gettime("H:i"));
  lcdSetCursor(0, 1);
  lcd.print(time.gettime("d-m-Y"));
  time.blinktime(timeModeSet);
}

void printCurrentScreenSettingsValues() {                            // вывод значений на экранах настроек
  if (menuPosition == 0) {
    clearValue(0, 1, 3);
    lcd.print((byte)round(brightness / 2.55));
  } else if (menuPosition == 1) {
    lcdSetCursor(0, 1);
    lcd.print(cycleLengthValueMM);
    lcdSetCursor(cycleLengthCurPos, 1);
  } else if (menuPosition == 2) {
    printDateTimeSettings();
  }
}

