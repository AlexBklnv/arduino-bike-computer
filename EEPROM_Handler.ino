#include <EEPROM.h>

void readDataFromEEPROM() {                                           // читаем данные из EEPROM
  EEPROM.get(0, brightness);
  EEPROM.get(1, totalDays);
  EEPROM.get(3, cycleLengthValueMM);
  cycleLengthValue = cycleLengthValueMM / 1000.0;
  EEPROM.get(5, MaxSpeed);
  EEPROM.get(9, totalDistance);
  EEPROM.get(13, totalDistanceMM);
  EEPROM.get(17, totalTime);
}

void saveDataAtEEPROM() {                                             // сохранение данных в EEPROM
  writeDataToEEPROM(1, totalDays);
  writeDataToEEPROM(5, MaxSpeed);
  writeDataToEEPROM(9, totalDistance);
  writeDataToEEPROM(13, totalDistanceMM);
  writeDataToEEPROM(17, totalTime);
}

void writeBrightnessToEEPROM() {                                      // отдельная функция тк будет вызываться только при изменении в настройках
  writeDataToEEPROM(0, brightness);
}

void writeCycleLengthToEEPROM() {                                     // отдельная функция тк будет вызываться только при изменении в настройках
  writeDataToEEPROM(3, cycleLengthValueMM);
}

void writeDataToEEPROM(int addr, byte value) {                        // запись в EEPROM переменной типа byte
  if (EEPROM.read(0) != value)
    EEPROM.put(0, value);
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

