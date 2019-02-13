#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <HBridge.h>

const uint64_t address = 0xF578DE93A5LL;
unsigned long lastReceivedTime = 0;

RF24 radio(9, 10);
Servo servo;
HBridge motor(3, 11);

struct TData {
  byte yl;
  byte xl;
  byte yr;
  byte xr;
};

TData data;

void resetData() {
  data.yl = 127;
  data.xl = 127;
  data.yr = 127;
  data.xr = 127;

  updateOutput();
}

void updateOutput() {
  servo.write(map(data.xr, 0, 254, 15, 165));

  if(127 >= data.yl) {
    motor.setDirection(CW);
    motor.setSpeed(map(data.yl, 0, 127, 0, 255));
  } else {
    motor.setDirection(CCW);
    motor.setSpeed(map(data.yl, 128, 254, 0, 255));
  }
}

void setupRadio() {
  resetData();

  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(false);

  radio.openReadingPipe(1, address);
  radio.startListening();
}

void setup() {
  servo.attach(5);

  setupRadio();
}

void loop() {
  while(radio.available()) {
    radio.read(&data, sizeof(TData));
    lastReceivedTime = millis();
  }

  unsigned long now = millis();
  if(now - lastRecvTime > 500) {
    setupRadio();
  }

  updateOutput();
}
