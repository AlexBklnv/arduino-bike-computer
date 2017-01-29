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
  EEPROM.get(1, totalDays);
  cycleLengthValue = getCycleLength() / 1000.0;
  maxSpeed = (int)getMaxSpeed();
  EEPROM.get(9, totalDistance);
  EEPROM.get(13, totalDistanceMM);
  EEPROM.get(17, totalTime);
}

void saveDataAtEEPROM() {                                             // сохранение данных в EEPROM
  writeDataToEEPROM(1, totalDays);
  setMaxSpeed(0.0);
  writeDataToEEPROM(9, totalDistance);
  writeDataToEEPROM(13, totalDistanceMM);
  writeDataToEEPROM(17, totalTime);
}

int getCycleLength(){
  return readIntFromEEPROM(3);
}

void setCycleLenght(int num){
  writeDataToEEPROM(3, num);
}

float getMaxSpeed(){
  return readFloatFromEEPROM(5);
}

void setMaxSpeed(float num){
  writeDataToEEPROM(5, num);
}

byte getBrightness(){
  return readByteFromEEPROM(0);
}

void setBrightness(byte num){
  writeDataToEEPROM(0, num);
}

byte readByteFromEEPROM(int addr) {
  return EEPROM.read(addr);
}

int readIntFromEEPROM(int addr) {
  byte raw[2];
  for (byte i = 0; i < 2; i++)
    raw[i] = EEPROM.read(addr + i);
  int &num = (int&)raw;
  return num;
}

float readFloatFromEEPROM(int addr) {
  byte raw[4];
  for (byte i = 0; i < 4; i++)
    raw[i] = EEPROM.read(addr + i);
  float &num = (float&)raw;
  return num;
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


