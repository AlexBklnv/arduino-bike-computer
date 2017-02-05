// индексы спец символов
const byte clockIndex = 0;
const byte heartIndex = 1;
const byte achievementIndex = 2;
const byte travelClockIndex = 3;

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
  lcd.createChar(clockIndex, clock);
  lcd.createChar(heartIndex, heart);
  lcd.createChar(achievementIndex, achievement);
  lcd.createChar(travelClockIndex, travelClock);
}
