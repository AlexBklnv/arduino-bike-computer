// TODO: PressedLongTheFirstButton 3 вариант обнуление еепром

// установка пинов кнопок
const byte button1Pin = 5;
const byte button2Pin = 6;
const byte button3Pin = 7;

// анти дребезг контактов на кнопках
Bounce debounceButton1 = Bounce();
Bounce debounceButton2 = Bounce();

// временные штампы нажатия на кнопку
unsigned long button1PressTimeStamp;
unsigned long button2PressTimeStamp;

void initButtons() {
  // настройка пинов кнопок и подстрокйка подтягивающего резистора
  pinMode(button1Pin, INPUT);
  digitalWrite(button1Pin, HIGH);
  pinMode(button2Pin, INPUT);
  digitalWrite(button2Pin, HIGH);

  // привязка объектов антидребезка к кнопкам
  debounceButton1.attach(button1Pin);
  debounceButton1.interval(5);
  debounceButton2.attach(button2Pin);
  debounceButton2.interval(5);
}

void buttonsHandler() {
  if (debounceButton1.update())  {
    if (debounceButton1.read() == 1) {
      button1PressTimeStamp =  millis();
    } else {
      if (millis() - button1PressTimeStamp >= 2000 ) {
        PressedLongTheFirstButton();
      } else {
        PressedTheFirstButton();
      }
    }
  } else if (debounceButton2.update())  {
    if (debounceButton2.read() == 1) {
      button2PressTimeStamp =  millis();
    } else {
      if (millis() - button2PressTimeStamp >= 2000 ) {
        PressedLongTheSecondButton();
      } else {
        PressedTheSecondButton();
      }
    }
  }
}


void PressedTheFirstButton() {
  if (isSettingsMenuActive) {
    switch (settingPosition) {
      case 0:
        byte powRate;
        int tmpPoweredScope;
        tmpPoweredScope = 1;
        switch (settingsCursorPosition) {
          case 0: powRate = 3; break;
          case 1: powRate = 2; break;
          case 2: powRate = 1; break;
          case 3: powRate = 0; break;
        }
        for (byte i = 0; i < powRate; i++)
          tmpPoweredScope *= 10;
        if (getDigitFromCycleLengtValue(settingsCursorPosition) == 9)
          if (settingsCursorPosition == 0)
            cycleLengthValue -= 8 * tmpPoweredScope;
          else
            cycleLengthValue -= 9 * tmpPoweredScope;
        else
          cycleLengthValue += 1 * tmpPoweredScope;
          redrawValues = true;
        break;
      case 1:
        switch (timeModeSet) {
          case 2: time.settime(-1, (time.minutes == 59 ? 0 : time.minutes + 1), -1, -1, -1, -1, -1); break;
          case 3: time.settime(-1, -1, (time.Hours == 23 ? 0 : time.Hours + 1), -1, -1, -1, -1); break;
          case 4: time.settime(-1, -1, -1, (time.day == 31 ? 1 : time.day + 1), -1, -1, -1); break;
          case 5: time.settime(-1, -1, -1, -1, (time.month == 12 ? 1 : time.month + 1), -1, -1); break;
          case 6: time.settime(-1, -1, -1, -1, -1, (time.year == 99 ? 0 : time.year + 1), -1); break;
        }
        break;
      case 2:
        // ничего не нужно
        break;
    }
  }
}

void PressedLongTheFirstButton() {
  if (isSettingsMenuActive) {
    switch (settingPosition) {
      case 0:
        if (settingsCursorPosition == 3)
          settingsCursorPosition = 0;
        else
          settingsCursorPosition++;
        lcd.setCursor(settingsCursorPosition, 1);
        break;
      case 1:
        // выбор позиции настройки времени -минуты-дни -дни
        if (timeModeSet == 6)
          timeModeSet = 2;
        else
          timeModeSet++;
        break;
      case 2:
        // обнулить EEPROM b jи очистка карточки
        break;
    }
  }
}

void PressedTheSecondButton() {
  if (!isSettingsMenuActive) {
    menuPosition++;
    if (menuPosition == 5)
      menuPosition = 0;
    redrawValues = true;
  } else {
    settingPosition++;
    if (settingPosition == 3)
      settingPosition = 0;
    redrawValues = true;
    if (settingPosition == 1) {
      lcd.noBlink();
    }
  }
  redrawScreen = true;
}

void PressedLongTheSecondButton() {
  if (isSettingsMenuActive) {
    lcd.noBlink();
    isSettingsMenuActive = false;
    menuPosition = 0;
    settingPosition = 0;
    printCurrentScreenTittles();
    printCurrnetScreenValues();
    redrawScreen = false;
  } else {
    isSettingsMenuActive = true;
    settingPosition = 0;
    menuPosition = 0;
    settingsCursorPosition = 0;
    printCurrentScreenSettingsTittles();
    printCurrentScreenSettingsValues();
    redrawScreen = false;
    redrawValues = false;
  }
}




