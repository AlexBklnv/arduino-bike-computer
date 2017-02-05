/*
requestStatus
0 - ожидаем запроса подключения
1 - ожидаем получение размера файла
2 - ожидаем запрос на получение динамических данных

*/

byte requestStatus = 0;

void serialEvent() {
  if (Serial.available()) {
    if (requestStatus == 0) {
      if ((char)Serial.read() == '#')
        if ((char)Serial.read() == 'S')
          if ((char)Serial.read() == 'F')
            sendReplyToDevice('F');
    } else if (requestStatus == 1) {
      unsigned long filePos = 0;
      while (Serial.available())
        filePos = Serial.parseInt();
      if (filePos < getFilePos()) {}
      // отправить весь файл
      else {}
      // отправить только нужную часть файла
    } else if (requestStatus == 2) {
      if ((char)Serial.read() == '#')
        if ((char)Serial.read() == 'S')
          if ((char)Serial.read() == 'D')
            sendReplyToDevice('D');
    }
  }
}

void sendReplyToDevice(char mode) {
  if (mode == 'F') {
    Serial.print(F("p="));
    Serial.print(getFilePos());
    Serial.print(F("|"));
  } else if (mode == 'D') {
    Serial.print(F("p="));
    Serial.print(getFilePos());
    Serial.print(F("|"));
    Serial.print(F("p="));
    Serial.print(getFilePos());
    Serial.print(F("|"));
    Serial.print(F("p="));
    Serial.print(getFilePos());
    Serial.print(F("|"));
  }
}

void sendDynDataToDevice() {

}

/*
void sendFileToDevice(){
  openFile(FILE_READ);
  Serial.print(F("p="));
  int syncFail = millis();
  bool syncData = false;

  while(millis() - syncFail < 5000){
    while (Serial.available()) {
      if ((char)Serial.read() == 'S'){
        syncData = true;
      }
    }
  }

  Serial.write(file.fileSize());
}*/
