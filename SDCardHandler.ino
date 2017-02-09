SdFat SD;
SdFile file;

bool initSDCard() {                                                                        // инициализация карточки
  return SD.begin();
}

void eraseSD() {                                                                           // уничтожаем с ионной пушки файл лога
  openFile(FILE_WRITE);   
  SD.wipe();
  file.close();
}

void openFile(byte mode) {                                                                 // открываем файлик на запись
    file.open("logfile.txt", mode);
}

void openFile() {                                                                          // открываем файлик на чтение
    file.open("logfile.txt");
}

void closeFile() {                                                                         // закрываем файлик
  file.close();
}

void writeDynDataToSD() {                                                                  // пишем характеристики на карточку 
  openFile(FILE_WRITE);
  file.print(F("<spec d=\""));  file.print(time.gettime("d:m:Y"));
  file.print(F("\" tc=\"")); file.print(time.gettime("H:i"));
  file.print(F("\" dst=\""));  file.write((unsigned long)(dynDist / 1000));
  file.print(F("\" brn=\""));  file.write(dynBurned);
  file.print(F("\" spd=\""));  file.write(dynSpeed / countDynAvgSpeed);
  file.print(F("\" bpm=\""));  file.write((int)(dynHR / countDynAvgHR));
  file.println(F("\"\">"));
  closeFile();
}

bool isFileAvailable(){                                                                     // проверяем наличие данных на чтение
  if (file.available() > 0)
    return true;
  else 
    return false;
}

char fileRead(){                                                                            // читаем байт данных
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
