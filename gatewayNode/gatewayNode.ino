#include <JeeLib.h>
#include "dataFormat.h"

void setup() {
  Serial.begin(9600);
  rf12_configSilent();
}

void loop() {
  if (rf12_recvDone() && rf12_crc == 0 && rf12_len == sizeof (Measure)) {
    Measure measure = *(Measure*) rf12_data;
    Serial.print(measure.nodeId, DEC);
    Serial.print(';');
    Serial.print(measure.temperature, DEC);
    Serial.print(';');
    Serial.print(measure.humidity, DEC);
    Serial.print(';');
    Serial.print(measure.brightness, DEC);
    Serial.print(';');
    Serial.print(measure.presence, DEC);
    Serial.println();
  }
}
