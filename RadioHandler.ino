const uint64_t pipes[2] = { 0xABCDABCD71, 0x544d52687C};

void initRadio() {
  radio.begin();
  delay(2);
  radio.setChannel(91);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.setRetries(15, 15);
  radio.openWritingPipe(pipes[1]);               // Открываем канал передачи
  radio.openReadingPipe(1, pipes[0]);             // Открываем канал приема
  radio.startListening();
}

void writeDataToRadio(bool startScan) {
  char msg[3];
  msg[0] = '#';
  if (startScan) {
    msg[1] = '<';
    msg[2] = '<';
  } else {
    msg[1] = '>';
    msg[2] = '|';
  }

  radio.stopListening();
  radio.write(&msg, sizeof(msg));
  radio.startListening();
}

void readDataFromRadio() {
  if (radio.available()) {
    detachInt();
    char msg[3];
    radio.read(&msg, 3);
    BPM = atoi(msg);
    redrawValues = true;
    attachInt();
  }
}

