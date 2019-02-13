#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t address = 0xF578DE93A5LL;

RF24 radio(9, 10);

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
}

void setup() {
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);

  radio.openWritingPipe(address);

  resetData();
}

void loop() {
  data.yl = map(analogRead(0), 0, 1023, 0, 254);
  data.xl = map(analogRead(1), 0, 1023, 0, 254);
  data.yr = map(analogRead(2), 0, 1023, 0, 254);
  data.xr = map(analogRead(3), 0, 1023, 0, 254);

  radio.write(&data, sizeof(TData));
}



