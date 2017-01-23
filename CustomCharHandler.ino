const byte clockIndex = 0;
const byte heartIndex = 1;
const byte energyIndex = 2;
const byte bluetoothIndex = 3;
const byte achievementIndex = 4;
const byte travelClockIndex = 5;

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

void initCustomChar() {
  lcd.createChar(clockIndex, clock);
  lcd.createChar(heartIndex, heart);
  lcd.createChar(energyIndex, energy);
  lcd.createChar(achievementIndex, achievement);
  lcd.createChar(travelClockIndex, travelClock);
}


