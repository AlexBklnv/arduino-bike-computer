/*     адреса данных
  0  - яркость экрана
  1  - totalDays
  3  - cycleLengthValueMM
  5  - MaxSpeed
  9  - Weight
  11 - totalDistance
  15 - totalDistanceMM
  19 - totalTime
  23 - Конец
*/

void readDataFromEEPROM() {                                           // читаем данные из EEPROM
  EEPROM.get(0, brightness);
  EEPROM.get(1, totalDays);
  EEPROM.get(3, cycleLengthValueMM);
  cycleLengthValue = cycleLengthValueMM / 1000.0 * 3.6;
  EEPROM.get(5, maxSpeed);
  EEPROM.get(9, weight);
  EEPROM.get(11, totalDistance);
  EEPROM.get(15, totalDistanceMM);
  EEPROM.get(19, totalTime);
}

void saveDataAtEEPROM() {                                             // сохранение данных в EEPROM
  writeDataToEEPROM(1, totalDays);
  writeDataToEEPROM(5, maxSpeed);
  writeDataToEEPROM(11, totalDistance);
  writeDataToEEPROM(15, totalDistanceMM);
  writeDataToEEPROM(19, totalTime);
}

void initStartEEPROM() {
  writeDataToEEPROM(0, 127);
  writeDataToEEPROM(1, 0);
  writeDataToEEPROM(3, 2000);
  writeDataToEEPROM(5, (float)0.0);
  writeDataToEEPROM(9, 50);
  writeDataToEEPROM(11, (unsigned long)0);
  writeDataToEEPROM(15, (unsigned long)0);
  writeDataToEEPROM(19, (unsigned long)0);
}


void setCycleLenght(int num) {                                        // записываем длину колеса
  writeDataToEEPROM(3, num);
}

void setBrightness(byte num) {                                        // записываем уровень яркости
  writeDataToEEPROM(0, num);
}

void setWeight(int num) {
  writeDataToEEPROM(9, num);
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
