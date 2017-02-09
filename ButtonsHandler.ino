// пины кнопок
#define BUTTON_1_PIN 5                                                // пин кнопки 1. // кнопка 1 действует только в меню настроек
#define BUTTON_2_PIN 6                                                // пин кнопки 2

// анти дребезг контактов на кнопках
Bounce debounceButton1 = Bounce();
Bounce debounceButton2 = Bounce();

unsigned long buttonPressTimeStamp;                                   // временный штампы нажатия на кнопку

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

// распознователь коротких и длинных нажатий на кнопку
void buttonsHandler() {
  if (debounceButton1.update())  {                                    // распознователь кнопки №1
    if (debounceButton1.read() == 1) {
      buttonPressTimeStamp =  millis();
    } else {
      if (millis() - buttonPressTimeStamp >= 2000) {
        PressedLongTheFirstButton();
      } else {
        PressedTheFirstButton();
      }
    }
  } else if (debounceButton2.update())  {                             // распознователь кнопки №2
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

void PressedTheFirstButton() {                                        // короткое нажатие кнопки 1
  if (isSettingsMenuActive) {
    switch (menuPosition) {                                           // меню настроек яркости
      case 0:
        brightness = brightness == 255 ? 31 : brightness += 32;       // инкремент яркости
        setBrightness(brightness);                                    // сохраняем яркость в EEPROM
        setBrightnessLCD();                                           // устанавливаем яркость экрана
        break;
      case 1:                                                         // меню настроек длины колеса
        int tmpPoweredScope;                                          // множитель для разряда
        byte digit;                                                   // рязряд
        tmpPoweredScope = 1;
        for (byte i = 0; i < (3 - cycleLengthCurPos); i++)            // в зависимости от разряда вычисляем множитель
          tmpPoweredScope *= 10;

        if (cycleLengthCurPos == 0)                                   // поразрядно слева направо получаем цифру
          digit = (byte)(cycleLengthValueMM / 1000);
        else if (cycleLengthCurPos == 1)
          digit = (byte)((cycleLengthValueMM / 100) % 10);
        else if (cycleLengthCurPos == 2)
          digit = (byte)((cycleLengthValueMM % 100) / 10);
        else if (cycleLengthCurPos == 3)
          digit = (byte)(cycleLengthValueMM % 10);

        if (digit == 9)                                                // полученную цифру инкриментируем по правилам
          if (cycleLengthCurPos == 0)
            cycleLengthValueMM -= 8 * tmpPoweredScope;
          else
            cycleLengthValueMM -= 9 * tmpPoweredScope;
        else
          cycleLengthValueMM += 1 * tmpPoweredScope;

        setCycleLenght(cycleLengthValueMM);                            // сохраняем значение длины колеса
        cycleLengthValue = cycleLengthValueMM / 1000.0 * 3.6;          // вычисляем длину колеса в формате для вычисления скорости
        calculateMaxMinTimeForSpeedReg();                              // вычисляем пороговые значения для новой длины колеса
        break;
      case 2:                                                          // меню настройки часов
        if (timeModeSet == 2)                                          // устанавливаем минуты
          time.settime(-1, (time.minutes == 59 ? 0 : time.minutes + 1), -1, -1, -1, -1, -1);
        else if (timeModeSet == 3)                                     // устанавливаем часы
          time.settime(-1, -1, (time.Hours == 23 ? 0 : time.Hours + 1), -1, -1, -1, -1);
        else if (timeModeSet == 4)                                     // устанавливаем дни
          time.settime(-1, -1, -1, (time.day == 31 ? 1 : time.day + 1), -1, -1, -1);
        else if (timeModeSet == 5)                                     // устанавливаем месяц
          time.settime(-1, -1, -1, -1, (time.month == 12 ? 1 : time.month + 1), -1, -1);
        else if (timeModeSet == 6)                                     // устанавливаем год
          time.settime(-1, -1, -1, -1, -1, (time.year == 99 ? 0 : time.year + 1), -1);
        break;
      default:
        break;
    }
    redrawValues = true;                                               // обновляем экран значений
  }
}

void PressedLongTheFirstButton() {                                     // длинное нажатие кнопки 1
  if (isSettingsMenuActive) {
    switch (menuPosition) {
      case 1:                                                          // смена разряда настройки длины колеса
        cycleLengthCurPos = cycleLengthCurPos == 3 ? 0 : cycleLengthCurPos + 1;
        lcdSetCursor(cycleLengthCurPos, 1);
        break;
      case 2:                                                          // выбор позиции настройки времени -минуты-дни -дни
        timeModeSet = timeModeSet == 6 ? 2 : timeModeSet + 1;
        break;
      case 3:                                                          // hard reset
        lcdSetCursor(14, 1);
        lcd.print(F("W8"));
        resetAchievements();                                           // чистим достижения
        eraseSD();                                                     // чистим карточку
        lcdSetCursor(14, 1);
        lcd.print(F("OK"));
        redrawScreen = true;
        break;
      default:
        break;
    }
  }
}

void PressedTheSecondButton() {                                        // короткое нажатие кнопки 2
  if (!isSettingsMenuActive) {                                         // смена экрана в зависимости от того в настрайках мы или нет
    menuPosition == 4 ? menuPosition = 0  : menuPosition ++;
  } else {
    menuPosition == 3 ? menuPosition = 0  : menuPosition ++;
  }
  redrawValues = true;
  redrawScreen = true;
}


void PressedLongTheSecondButton() {                                     // долго нажатие кнопки 2
  menuPosition = 0;                                                     // на начальный экран
  if (isSettingsMenuActive) {                                           // если мы перешли в меню отображения информации
    attachInt();	                                                      // врубаем регистратор скорости
    isSettingsMenuActive = false;                                       // меняем режим меню
    lcd.noBlink();                                                      // если были в меню настроек длины колеса то вырубаем блинк
    printCurrentScreenTittles();
    printCurrnetScreenValues();
  } else {                                                              // если перешли в меню настроек
    detachInt();                                                        // вырубаем регистратор скорости
    isSettingsMenuActive = true;                                        // свпаем режим
    printCurrentScreenSettingsTittles();
    printCurrentScreenSettingsValues();
  }
}

