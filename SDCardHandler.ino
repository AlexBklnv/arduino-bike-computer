/*File trainingFile;
bool isSDExists;
void initSDCard() {
  SD.begin();
  if (!SD.exists("train.txt")) {
    isSDExists = true;
    SD.open("train.txt", FILE_WRITE);
    trainingFile.close();
  } else
    isSDExists = false;
}

void writeStartDynDataToSDCard() {
  if (isSDExists) {
    trainingFile = SD.open("train.txt", FILE_WRITE);
    if (trainingFile) {
         printToSD(F("<date date=\"")); printToSD(time.gettime("d:m:Y")); printToSD(F("\">"));
         printToSD(F("<spec tc_start=\"")); printToSD(time.gettime("H:i")); printToSD(F("\" "));
    }
    trainingFile.close();
  }
}

void writeStopDynDataToSDCard() {
  if (isSDExists) {
    trainingFile = SD.open("train.txt", FILE_WRITE);
    if (trainingFile) {
       printToSD(F("tc_stop=\"")); printToSD(time.gettime("H:i")); printToSD(F("\" "));
      // printToSD(F("dist=\"")); printToSD(round(dynDist/1000)); printToSD(F("\" "));
       printToSD(F("burn"));

    }
    trainingFile.close();
  }
}

void printToSD(String val) {
  trainingFile.print(val);
}

void printToSD(int val) {
  trainingFile.print(val);
}*/
