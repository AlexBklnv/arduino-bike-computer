const uint64_t pipes[2] = { 0xABCDABCD71, 0x544d52687C};                  // адреса для каналов радио

void initRadio() {
  radio.begin();
  delay(2);
  radio.setChannel(85);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.setRetries(15, 15);                                               // (мс, попыток)
  radio.openWritingPipe(pipes[1]);                                        // Открываем канал передачи
  radio.openReadingPipe(1, pipes[0]);                                     // Открываем канал приема
  radio.startListening();
}

void writeDataToRadio(bool startScan) {                                   // отправляем запрос по радио пульсометру по шаблону 
  char msg[3];
  msg[0] = '#';                                                           
  if (startScan) {
    msg[1] = '<';
    msg[2] = '<';                                                         // #<< - начинаем сканировение
  } else {
    msg[1] = '>';
    msg[2] = '|';                                                         // #>| - останавливаем сканировение
  }

  radio.stopListening();
  radio.write(&msg, sizeof(msg));
  radio.startListening();
}

void readDataFromRadio() {                                                // читаем данные по сердцебиению
 if (radio.available()) {
    char msg[3];
    detachInt();                                                          // отключаем регистратор на короткое время для приема данных
    radio.read(&msg, 3);
    BPM = atoi(msg);
    attachInt();                                                          // данные приняты и можем возращать регистратор
    dynHR += BPM;
    countDynAvgHR++;
    redrawValues = true;
  }
}
