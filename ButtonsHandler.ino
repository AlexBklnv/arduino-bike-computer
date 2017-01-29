// пины кнопок
// button1Pin = 5
// button2Pin = 6


// анти дребезг контактов на кнопках
Bounce debounceButton1 = Bounce();
Bounce debounceButton2 = Bounce();

// временные штампы нажатия на кнопку
unsigned long button1PressTimeStamp;
unsigned long button2PressTimeStamp;

void initButtons() {
  // настройка пинов кнопок и подстрокйка подтягивающего резистора
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);

  // привязка объектов антидребезка к кнопкам
  debounceButton1.attach(5, INPUT);
  debounceButton1.interval(5);
  debounceButton2.attach(6, INPUT);
  debounceButton2.interval(5);
}

void buttonsHandler() {
  if (debounceButton1.update())  {
    if (debounceButton1.read() == 1) {
      button1PressTimeStamp =  millis();
    } else {
      if (millis() - button1PressTimeStamp >= 2000) {
        PressedLongTheFirstButton();
      } else {
        PressedTheFirstButton();
      }
    }
  } else if (debounceButton2.update())  {
    if (debounceButton2.read() == 1) {
      button2PressTimeStamp =  millis();
    } else {
      if (millis() - button2PressTimeStamp >= 2000) {
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
        if (brightness == 255)
          brightness = 31;
        else
          brightness += 32;
        setBrightness(brightness);
        setBrightnessLCD();
        redrawValues = true;
        break;
      case 1:
        int tmpPoweredScope;
        tmpPoweredScope = 10;
        pow(tmpPoweredScope, 3 - settingsCursorPosition);
        
        if (getDigitFromCycleLengtValue(settingsCursorPosition) == 9)
          if (settingsCursorPosition == 0)
            cycleLengthValueMM -= 8 * tmpPoweredScope;
          else
            cycleLengthValueMM -= 9 * tmpPoweredScope;
        else
          cycleLengthValueMM += 1 * tmpPoweredScope;
        redrawValues = true;
        cycleLengthValue = cycleLengthValueMM / 1000.0;
        setCycleLenght(cycleLengthValueMM);
        calculateMaxMinTimeForSpeedReg();
        break;
      case 2:
        switch (timeModeSet) {
          case 2: time.settime(-1, (time.minutes == 59 ? 0 : time.minutes + 1), -1, -1, -1, -1, -1); break;
          case 3: time.settime(-1, -1, (time.Hours == 23 ? 0 : time.Hours + 1), -1, -1, -1, -1); break;
          case 4: time.settime(-1, -1, -1, (time.day == 31 ? 1 : time.day + 1), -1, -1, -1); break;
          case 5: time.settime(-1, -1, -1, -1, (time.month == 12 ? 1 : time.month + 1), -1, -1); break;
          case 6: time.settime(-1, -1, -1, -1, -1, (time.year == 99 ? 0 : time.year + 1), -1); break;
        }
        break;
      case 3:
        // ничего не нужно
        break;
    }
  }
}

void PressedLongTheFirstButton() {
  if (isSettingsMenuActive) {
    switch (settingPosition) {
      case 0:
        break;
      case 1:
        settingsCursorPosition = settingsCursorPosition == 3 ? 0 : settingsCursorPosition++;
        lcd.setCursor(settingsCursorPosition, 1);
        break;
      case 2:
        // выбор позиции настройки времени -минуты-дни -дни
        timeModeSet = timeModeSet == 6 ? 2 : timeModeSet++;
        break;
      case 3:
        resetAchievements();
        eraseSD();
        lcd.setCursor(14, 1);
        lcd.print(F("OK"));
        redrawScreen = true;
        break;
    }
  }
}

void PressedTheSecondButton() {
  if (!isSettingsMenuActive) {
    menuPosition++;
    if (menuPosition == 5)
      menuPosition = 0;
  } else {
    settingPosition++;
    if (settingPosition == 4)
      settingPosition = 0;
  }
  redrawValues = true;
  redrawScreen = true;
}

void PressedLongTheSecondButton() {
  if (isSettingsMenuActive) {
    detachInt();
    isSettingsMenuActive = false;
    lcd.noBlink();
    settingPosition = 0;
    printCurrentScreenTittles();
    printCurrnetScreenValues();
  } else {
    attachInt();
    isSettingsMenuActive = true;
    menuPosition = 0;
    printCurrentScreenSettingsTittles();
    printCurrentScreenSettingsValues();
  }
}
