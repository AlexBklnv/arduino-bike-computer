SdFat SD;
SdFile file;

bool initSDCard() {                                                                        // инициализация карточки
  if (!SD.begin()) {
    lcd.clear();
    lcd.print(F("SD FAILED!"));
    delay(2000);
    return false;
  }
  return true;
}

void eraseSD() {                                                                           // уничтожаем с ионной пушки файл лога
  openFile(FILE_WRITE);
  SD.wipe();
  file.close();
}

void openFile(byte mode) {                                                                 // открываем файлик на запись
  file.open("log.txt", mode);
}

void openFile() {                                                                          // открываем файлик на чтение
  file.open("log.txt");
}

void closeFile() {                                                                         // закрываем файлик
  file.close();
}

void writeDynDataToSD() {                                                                  // пишем характеристики на карточку
  detachInt();
  openFile(FILE_WRITE);
  file.print(F("<spec d=\""));  file.print(time.gettime("d:m:Y"));
  file.print(F("\" tc=\""));    file.print(time.gettime("H:i"));
  file.print(F("\" dst=\""));   file.print((unsigned long)(dynDist / 1000));
  file.print(F("\" brn=\""));

  if (countDynAvgHR < 30)
    dynHR = 120;
  else
    dynHR = (dynHR / countDynAvgHR);
  dynBurned = (unsigned long)((float)weight * 0.014 * ((float)0.12 * dynHR - 7));
  curCal = curCal + dynBurned;
  file.print(dynBurned);

  file.print(F("\" spd=\""));   file.print(dynSpeed / countDynAvgSpeed);
  file.print(F("\" bpm=\""));   file.print(dynHR);
  file.println(F("\"/>"));
  closeFile();
  resetDynCharSD();
  attachInt();
}



bool isFileAvailable() {                                                                    // проверяем наличие данных на чтение
  if (file.available() > 0)
    return true;
  else
    return false;
}

char fileRead() {                                                                           // читаем байт данных
  return file.read();
}

void setFilePos(unsigned long filePos) {                                                    // устанавливаем позицию в файле старта чтения
  openFile();
  file.seekSet(filePos);
}

unsigned long getFilePos() {                                                                // хватаем размер файла
  openFile();
  unsigned long fSize = file.fileSize();
  closeFile();
  return fSize;
}
