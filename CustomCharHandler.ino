// индексы спец символов
const byte charIndex[] PROGMEM = {
  0,// clockIndex
  1,// heartIndex
  2, // energyIndex
  3,// achievementIndex
  4 //travelClockIndex
};

byte clock[8] = {
  0b01110,
  0b10011,
  0b10101,
  0b10001,
  0b01110,
  0b00000,
  0b00000,
  0b00000
};

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

byte energy[8] = {
  0b00011,
  0b00110,
  0b01100,
  0b11110,
  0b11111,
  0b00110,
  0b01100,
  0b11000
};

byte achievement[8] = {
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00100,
  0b01110
};

byte travelClock[8] = {
  0b01110,
  0b10011,
  0b10101,
  0b10001,
  0b01110,
  0b00000,
  0b01110,
  0b00100
};

void initCustomChar() {                                              // инициализация символов
  lcd.createChar(pgm_read_byte(&(charIndex[0])), clock);
  lcd.createChar(pgm_read_byte(&(charIndex[1])), heart);
  lcd.createChar(pgm_read_byte(&(charIndex[2])), energy);
  lcd.createChar(pgm_read_byte(&(charIndex[3])), achievement);
  lcd.createChar(pgm_read_byte(&(charIndex[4])), travelClock);
}


