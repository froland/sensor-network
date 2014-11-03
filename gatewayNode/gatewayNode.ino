#include <JeeLib.h>
#include "dataFormat.h"

void setup() {
  Serial.begin(57600);
  rf12_configSilent();
}

void loop() {
  if (rf12_recvDone() && rf12_crc == 0 && rf12_len == sizeof (Measure)) {
    Measure measure = *(Measure*) rf12_data;
    Serial.print(measure.nodeId, DEC);
    Serial.print(';');
    Serial.print(measure.sensorId, DEC);
    Serial.print(';');
    Serial.print(measure.measureType, DEC);
    Serial.print(';');
    Serial.print(measure.payload);
    Serial.println();
  }
}
