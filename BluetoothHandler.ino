void serialEvent() {                                                                    // ивент происходит при получении данных через UART
  if (Serial.available()) {                                                             // если есть данные
    char msgDev[3];
    Serial.readBytes(msgDev, 3);
    if (msgDev[0] == '#')
      if (msgDev[1] == 'S') {
        if (msgDev[2] == 'F') {
          unsigned long filePosDevice = 0;
          setFilePos(filePosDevice);                                                      // с позиции которой пришла
          while (isFileAvailable())                                                       // отправляем файл как поток байтов
            Serial.print(fileRead());
          closeFile();
          Serial.print(F("#!"));
        } else if (msgDev[2] == 'D') {
          Serial.print(F("s="));                                                                // текущая скорость
          Serial.print(curSpeed);
          Serial.print(F("h="));                                                                // хартрейт
          Serial.print(BPM);
          Serial.print(F("d="));                                                                // дистанция
          Serial.print((unsigned long)(travelDistance / 1000));
          Serial.print(F("c="));                                                                // ккал
          Serial.print(curCal);
          Serial.print(F("#!"));
          Serial.flush();
        }
      }
  }
}

