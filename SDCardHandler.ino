bool isSDCardExist() {
  return SD.begin();
}

File openFile() {
  return SD.open("train.txt", FILE_WRITE);
}

void eraseSD() {
  SD.remove("train.txt");
}

void closeFile(File file) {
  file.close();
}

void writeStartDynDataToSDCard() {
  File file = openFile();
  file.print(F("<date date=\"")); file.print(time.gettime("d:m:Y")); file.print(F("\">"));
  file.print(F("<spec tc_start=\"")); file.print(time.gettime("H:i")); file.print(F("\" "));
  closeFile(file);
}

void writeStopDynDataToSDCard() {
  File file = openFile();
  file.print(F("tc_stop=\"")); file.print(time.gettime("H:i")); file.print(F("\" "));
  file.print(F("dist=\"")); file.print((unsigned long)(dynDist / 1000.0)); file.print(F("\" "));
  file.print(F("burn=\"")); file.print(dynBurned); file.print(F("\" "));
  file.print(F("avg_speed=\"")); file.print(dynSpeed / countDynAvgSpeed); file.print(F("\" "));
  file.print(F("avg_HR=\"")); file.print((int)(dynHR / countDynAvgHR)); file.print(F("\" "));
  file.print(F("\">"));
  closeFile(file);
}

void writeDayIsOver() {
  File file = openFile();
  file.print(F("<\/date>"));
  closeFile(file);
}
