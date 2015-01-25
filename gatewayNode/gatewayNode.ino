/// @dir rf12serial
/// Serial data over RF12 demo, works in both directions.
// 2009-04-17 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <JeeLib.h>
#include <RF12sio.h>

RF12 RF12;

struct Payload {
  byte light;     // light sensor: 0..255
  byte moved :1;  // motion detector: 0..1
  byte humi  :7;  // humidity: 0..100
  int temp   :10; // temperature: -500..+500 (tenths)
  byte lobat :1;  // supply voltage dropped under 3.1V: 0..1
};
Payload payload;

void setup() {
  Serial.begin(9600);
  Serial.print("\n[rf12serial]");
  rf12_config();
}

void loop() {
  if (rf12_recvDone() && rf12_crc == 0) {
    if (rf12_len == sizeof (Payload)) {
      payload = *(Payload*) rf12_data;
      int nodeId = RF12_HDR_MASK & rf12_hdr;
      Serial.print(nodeId, DEC);
      Serial.print(": light=");
      Serial.print(payload.light, DEC);
      Serial.print(", moved:");
      Serial.print(payload.moved, DEC);
      Serial.print(", humi:");
      Serial.print(payload.humi, DEC);
      Serial.print(", temp:");
      Serial.print(payload.temp / 10.0, 1);
      Serial.print(", lobat:");
      Serial.print(payload.lobat, DEC);
      Serial.println();
    }
    if (RF12_WANTS_ACK) {
      rf12_sendStart(RF12_ACK_REPLY, 0, 0);
    }
  }
}
