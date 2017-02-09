void serialEvent() {                                                                    // ивент происходит при получении данных через UART
  if (Serial.available()) {                                                             // если есть данные
    if (statusBLE == 0) {                                                               // данный режим нужен для ограничения от "левых" подключений
      char msgDev[3];
      Serial.readBytes(msgDev, 3);                                                      // принимаем команду от устройства. Если она опознана
      if (msgDev[0] == '#')
        if (msgDev[1] == 'S')
          if (msgDev[2] == 'F') {
            statusBLE = 1;                                                              // переходим в следующий режим
            setRecResetTimeStamp();                                                     // фиксируем время приема
          }
    } else if (statusBLE == 1) {
      unsigned long filePosDevice = 0;
      while (Serial.available())                                                        // получаем размер файла
        filePosDevice = Serial.parseInt();
      if (filePosDevice != getFilePos() || filePosDevice == 0) {                        // если размер файла не равен тому что есть на карте то отсылаем данные
        setFilePos(filePosDevice);                                                      // с позиции которой пришла
        while (isFileAvailable()) {                                                     // отправляем файл как поток байтов
          Serial.print(fileRead());
        }
        closeFile();
      }
      Serial.print(F("#!"));                                                            // конец отправки файла
      statusBLE = 2;                                                                    // переход в режим отправки динамических данных
      setRecResetTimeStamp();                                                           // фиксируем время приема
    } else if (statusBLE == 2) {
      char msgDev;
      msgDev = Serial.read();
      if (msgDev == 'N') {                                                              // распознаем команду дальнейшей отправки данных
        setRecResetTimeStamp();                                                         // фиксируем время приема
      } else  if (msgDev == 'S') {                                                      // распознаем команду остановки отправки данных
        statusBLE = 0;                                                                  // уходим в режим ожидания синхронизации
      }
    }
  }
}

void setRecResetTimeStamp() {                                                           // фиксируем время приема сообщения
  requestResetTimeStamp = millis();
}

void sendStopField() {                                                                  // отправка символа - разделителя полей данных
  Serial.print(F("|"));
}

void sendDynDataToDevice() {                                                            // отправка динамических данных шаблоном field=value|
  Serial.print(F("s="));                                                                // текущая скорость
  Serial.print(curSpeed);
  sendStopField();
  Serial.print(F("h="));                                                                // хартрейт
  Serial.print(BPM);
  sendStopField();
  Serial.print(F("d="));                                                                // дистанция
  Serial.print((unsigned long)(travelDistance / 1000));
  sendStopField();
  Serial.print(F("c="));                                                                // ккал
  Serial.print(curCal);
  sendStopField();
}
