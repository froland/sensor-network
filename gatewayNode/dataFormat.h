#define MT_TEMP 0
#define MT_HUM 1
#define MT_LIGHT 2
#define MT_MOTION 3

typedef struct {
  byte nodeId :5;
  byte sensorId :4;
  byte measureType: 5;
  int payload;
} Measure;

