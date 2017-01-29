// пины кнопок
const byte buttonPin[] PROGMEM = {
  5,                                              // button1Pin
  6,                                              // button2Pin
};

// анти дребезг контактов на кнопках
Bounce debounceButton1 = Bounce();
Bounce debounceButton2 = Bounce();

// временные штампы нажатия на кнопку
unsigned long button1PressTimeStamp;
unsigned long button2PressTimeStamp;

void initButtons() {
  // настройка пинов кнопок и подстрокйка подтягивающего резистора
  digitalWrite(pgm_read_byte(&(buttonPin[0])), HIGH);
  digitalWrite(pgm_read_byte(&(buttonPin[1])), HIGH);

  // привязка объектов антидребезка к кнопкам
  debounceButton1.attach(pgm_read_byte(&(buttonPin[0])), INPUT);
  debounceButton1.interval(5);
  debounceButton2.attach(pgm_read_byte(&(buttonPin[1])), INPUT);
  debounceButton2.interval(5);
}

void buttonsHandler() {
  if (debounceButton1.update())  {
    if (debounceButton1.read() == 1) {
      button1PressTimeStamp =  millis();
    } else {
      if (millis() - button1PressTimeStamp >= pgm_read_byte(&(timePoint[5])) ) {
        PressedLongTheFirstButton();
      } else {
        PressedTheFirstButton();
      }
    }
  } else if (debounceButton2.update())  {
    if (debounceButton2.read() == 1) {
      button2PressTimeStamp =  millis();
    } else {
      if (millis() - button2PressTimeStamp >= pgm_read_byte(&(timePoint[5])) ) {
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
        if (getBrightness() == 255)
          setBrightness(31);
        else
          setBrightness(getBrightness() + 32);
        setBrightnessLCD();
        redrawValues = true;
        break;
      case 1:
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
        if (settingsCursorPosition == 3)
          settingsCursorPosition = 0;
        else
          settingsCursorPosition++;
        lcd.setCursor(settingsCursorPosition, 1);
        break;
      case 2:
        // выбор позиции настройки времени -минуты-дни -дни
        if (timeModeSet == 6)
          timeModeSet = 2;
        else
          timeModeSet++;
        break;
      case 3:
        lcd.clear();
        printMSG(0, 0, F("Resetting"));
        printMSG(0, 1, F("achievements.."));
        //resetAchievements();
        delay(400);
        printMSG(14, 1, F("OK"));
        delay(600);
        lcd.clear();
        printMSG(0, 0, F("Resetting"));
        printMSG(0, 1, F("SD card..."));
        // почистить карту
        delay(400);
        printMSG(14, 1, F("OK"));
        delay(600);
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
    menuPosition = 0;
    settingPosition = 0;
    printCurrentScreenTittles();
    printCurrnetScreenValues();
    redrawScreen = false;
  } else {
    attachInt();
    isSettingsMenuActive = true;
    settingPosition = 0;
    menuPosition = 0;
    printCurrentScreenSettingsTittles();
    printCurrentScreenSettingsValues();
    redrawScreen = false;
    redrawValues = false;
  }
}
