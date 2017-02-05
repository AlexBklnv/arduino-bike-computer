SdFat SD;
SdFile file;

bool initSDCard() {
  return SD.begin();
}

void eraseSD() {
  openFile(FILE_WRITE);
  SD.wipe();
  file.close();
}

void openFile(byte mode) {
  file.open("train.txt", mode);
}

void closeFile() {
  file.close();
}

void writeStartDynDataToSD() {
  openFile(FILE_WRITE);
  file.print(F("<spc d=\""));  file.print(time.gettime("d:m:Y"));
  file.print(F("\" tc_b=\"")); file.print(time.gettime("H:i"));
  closeFile();
}


void writeStopDynDataToSD() {
  openFile(FILE_WRITE);
  file.print(F("\" tc_e=\"")); file.print(time.gettime("H:i"));
  file.print(F("\" dst=\""));  file.write((unsigned long)(dynDist / 1000));
  file.print(F("\" brn=\""));  file.write(dynBurned);
  file.print(F("\" spd=\""));  file.write(dynSpeed / countDynAvgSpeed);
  file.print(F("\" bpm=\""));  file.write((int)(dynHR / countDynAvgHR));
  file.println(F("\"\">"));
  closeFile();
}

unsigned long getFilePos(){
    openFile(FILE_READ);
    file.fileSize();
    closeFile();
}
