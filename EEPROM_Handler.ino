/*
0 - яркость экрана
1 - totalDays
3 - cycleLengthValueMM
5 - MaxSpeed
9 - totalDistance
13 - totalDistanceMM
17 - totalTime
*/

void readDataFromEEPROM() {                                           // читаем данные из EEPROM
  EEPROM.get(0,brightness);
  EEPROM.get(1, totalDays);
  EEPROM.get(3, cycleLengthValueMM);
  cycleLengthValue = cycleLengthValueMM / 1000.0;
  EEPROM.get(5,maxSpeed);
  EEPROM.get(9, totalDistance);
  EEPROM.get(13, totalDistanceMM);
  EEPROM.get(17, totalTime);
}

void saveDataAtEEPROM() {                                             // сохранение данных в EEPROM
  writeDataToEEPROM(1, totalDays);
  writeDataToEEPROM(5, maxSpeed);
  writeDataToEEPROM(9, totalDistance);
  writeDataToEEPROM(13, totalDistanceMM);
  writeDataToEEPROM(17, totalTime);
}

void setCycleLenght(int num){
  writeDataToEEPROM(3, num);
}

void setBrightness(byte num){
  writeDataToEEPROM(0, num);
}

void writeDataToEEPROM(int addr, int value) {                         // запись в EEPROM переменной типа int
  int tmp;
  EEPROM.get(addr, tmp);
  if (tmp != value)
    EEPROM.put(addr, value);
}

void writeDataToEEPROM(int addr, unsigned long value) {               // запись в EEPROM переменной типа unsigned long
  unsigned long tmp;
  EEPROM.get(addr, tmp);
  if (tmp != value)
    EEPROM.put(addr, value);
}

void writeDataToEEPROM(int addr, float value) {                       // запись в EEPROM переменной типа float
  float tmp;
  EEPROM.get(addr, tmp);
  if (tmp != value)
    EEPROM.put(addr, value);
}


