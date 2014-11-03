#include <JeeLib.h>
#include "dataFormat.h"

#define HYT131_PORT 1
#define LDR_PORT 4
#define PIR_PORT 4

#define SERIAL_DELAY 2
#define LOOP_DELAY 10000

typedef struct {
  byte humi :7; // humidity [0..100]%
  int temp :10; // temperature [-500..500] * 10^-1 °C
} EnvMeas;

Measure measure;
EnvMeas envMeasure;
PortI2C hyti2cport (HYT131_PORT);
HYT131 hyt131 (hyti2cport);
Port ldr(LDR_PORT);
Port pir(PIR_PORT);

ISR(WDT_vect) { Sleepy::watchdogEvent(); }

static void measureTempAndHumidity() {
  int humi, temp;
  hyt131.reading(temp, humi);
  envMeasure.humi = humi/10;
  envMeasure.temp = temp;
}

static byte measureLight() {
  ldr.digiWrite2(1);
  byte light = ~ ldr.anaRead() >> 2;
  ldr.digiWrite2(0);
  return light;
}

static boolean measurePresence() {
  return pir.digiRead();
}

void sendMeasure(byte sensorId, byte measureType, int payload) {
  measure.sensorId = sensorId;
  measure.measureType = measureType;
  measure.payload = payload;
  
  rf12_sendNow(0, &measure, sizeof (Measure));
  rf12_sendWait(2);
}

void setup() {
  measure.nodeId = rf12_configSilent();
  delay(100);
  rf12_sleep(RF12_SLEEP);
}

void loop() {
  rf12_sleep(RF12_WAKEUP);
  measureTempAndHumidity();
  sendMeasure(0, MT_TEMP, envMeasure.temp);
  sendMeasure(1, MT_HUMI, envMeasure.humi);
  
  sendMeasure(2, MT_LIGHT, measureLight());
  
  sendMeasure(3, MT_MOTION, measurePresence());
  
  rf12_sleep(RF12_SLEEP);
  Sleepy::loseSomeTime(LOOP_DELAY);
}
