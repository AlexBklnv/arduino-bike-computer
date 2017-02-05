// пины кнопок
#define BUTTON_1_PIN 5                   // пин радиомодуля Chip Enable
#define BUTTON_2_PIN 6                    // пин радиомодуля Chip Select

// анти дребезг контактов на кнопках
Bounce debounceButton1 = Bounce();
Bounce debounceButton2 = Bounce();

// временные штампы нажатия на кнопку
unsigned long buttonPressTimeStamp;

void initButtons() {
  // настройка пинов кнопок и подстрокйка подтягивающего резистора
  digitalWrite(BUTTON_1_PIN, HIGH);
  digitalWrite(BUTTON_2_PIN, HIGH);

  // привязка объектов антидребезка к кнопкам
  debounceButton1.attach(BUTTON_1_PIN, INPUT);
  debounceButton1.interval(5);
  debounceButton2.attach(BUTTON_2_PIN, INPUT);
  debounceButton2.interval(5);
}

void buttonsHandler() {
  if (debounceButton1.update())  {
    if (debounceButton1.read() == 1) {
      buttonPressTimeStamp =  millis();
    } else {
      if (millis() - buttonPressTimeStamp >= 2000) {
        PressedLongTheFirstButton();
      } else {
        PressedTheFirstButton();
      }
    }
  } else if (debounceButton2.update())  {
    if (debounceButton2.read() == 1) {
      buttonPressTimeStamp =  millis();
    } else {
      if (millis() - buttonPressTimeStamp >= 2000) {
        PressedLongTheSecondButton();
      } else {
        PressedTheSecondButton();
      }
    }
  }
}

void PressedTheFirstButton() {
  if (isSettingsMenuActive) {
    switch (menuPosition) {
      case 0:
        brightness = brightness == 255 ? 31 : brightness += 32;
        setBrightness(brightness);
        setBrightnessLCD();
        break;
      case 1:
        int tmpPoweredScope;
        byte digit;
        tmpPoweredScope = 1;
        for (byte i = 0; i < (3 - settingsCursorPosition); i++)
          tmpPoweredScope *= 10;

        if (settingsCursorPosition == 0)                                                // поразрядно слева направо
          digit = (byte)(cycleLengthValueMM / 1000);
        else if (settingsCursorPosition == 1)
          digit = (byte)((cycleLengthValueMM / 100) % 10);
        else if (settingsCursorPosition == 2)
          digit = (byte)((cycleLengthValueMM % 100) / 10);
        else if (settingsCursorPosition == 3)
          digit = (byte)(cycleLengthValueMM % 10);

        if (digit == 9)
          if (settingsCursorPosition == 0)
            cycleLengthValueMM -= 8 * tmpPoweredScope;
          else
            cycleLengthValueMM -= 9 * tmpPoweredScope;
        else
          cycleLengthValueMM += 1 * tmpPoweredScope;

        cycleLengthValue = cycleLengthValueMM / 1000.0;
        setCycleLenght(cycleLengthValueMM);
        calculateMaxMinTimeForSpeedReg();
        break;
      case 2:
        if (timeModeSet == 2)
          time.settime(-1, (time.minutes == 59 ? 0 : time.minutes + 1), -1, -1, -1, -1, -1);
        else if (timeModeSet == 3)
          time.settime(-1, -1, (time.Hours == 23 ? 0 : time.Hours + 1), -1, -1, -1, -1);
        else if (timeModeSet == 4)
          time.settime(-1, -1, -1, (time.day == 31 ? 1 : time.day + 1), -1, -1, -1);
        else if (timeModeSet == 5)
          time.settime(-1, -1, -1, -1, (time.month == 12 ? 1 : time.month + 1), -1, -1);
        else if (timeModeSet == 6)
          time.settime(-1, -1, -1, -1, -1, (time.year == 99 ? 0 : time.year + 1), -1);
        break;
      default:
        break;
    }
    redrawValues = true;
  }

}

void PressedLongTheFirstButton() {
  if (isSettingsMenuActive) {
    switch (menuPosition) {
      case 1:
        settingsCursorPosition = settingsCursorPosition == 3 ? 0 : settingsCursorPosition + 1;
        lcdSetCursor(settingsCursorPosition, 1);
        break;
      case 2:
        // выбор позиции настройки времени -минуты-дни -дни
        timeModeSet = timeModeSet == 6 ? 2 : timeModeSet + 1;
        break;
      case 3:
        resetAchievements();
        lcdSetCursor(14, 1);
        lcd.print(F("W8"));
        eraseSD();
        lcdSetCursor(14, 1);
        lcd.print(F("OK"));
        redrawScreen = true;
        break;
      default:
        break;
    }
  }
}

void PressedTheSecondButton() {
  if (!isSettingsMenuActive) {
    menuPosition == 4 ? menuPosition = 0  : menuPosition ++;
  } else {
    menuPosition == 3 ? menuPosition = 0  : menuPosition ++;
  }
  redrawValues = true;
  redrawScreen = true;
}


void PressedLongTheSecondButton() {
  menuPosition = 0;
  if (isSettingsMenuActive) {
    detachInt();	                                 // отключаем прерывание геркона
    isSettingsMenuActive = false;
    lcd.noBlink();
    printCurrentScreenTittles();
    printCurrnetScreenValues();
  } else {
    attachInt();
    isSettingsMenuActive = true;
    printCurrentScreenSettingsTittles();
    printCurrentScreenSettingsValues();
  }

}

