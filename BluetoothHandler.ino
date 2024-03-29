void serialEvent() {                                                                    // ивент происходит при получении данных через UART
  detachInt();
  if (Serial.available()) {                                                             // если есть данные
    char msgDev[3];
    Serial.readBytes(msgDev, 3);
    if (msgDev[0] == '#')
      if (msgDev[1] == 'S') {
        if (msgDev[2] == 'F') {
          isSendDynData = false;
          unsigned long filePosDevice = 0;
          Serial.print(F("#SF"));
          setFilePos(filePosDevice);                                                      // с позиции которой пришла
          while (isFileAvailable())                                                       // отправляем файл как поток байтов
            Serial.print(fileRead());
          closeFile();
          sendStopTransferField();
        } else if (msgDev[2] == 'D') {
          isSendDynData = true;
          sendDynDataTimeStamp = millis();
        }
      }
  }
  attachInt();
}

void sendStopField() {
  Serial.print(F("$"));
}

void sendStopTransferField() {
  Serial.print(F("#!"));
}

void sendDynData() {
  Serial.print(F("#SD"));
  Serial.print(F("s="));                                                                // текущая скорость
  Serial.print(curSpeed);
  sendStopField();
  Serial.print(F("h="));                                                                // хартрейт
  Serial.print(BPM);
  sendStopField();
  Serial.print(F("d="));                                                                // дистанция
  Serial.print(round(travelDistance / 1000));
  sendStopField();
  Serial.print(F("c="));                                                                // кал
  Serial.print(curCal);
  sendStopField();
  Serial.flush();
  sendStopTransferField();
}
