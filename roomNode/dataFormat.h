typedef struct {
  byte nodeId :5;
  int temperature :10;
  byte humidity :7;
  byte brightness;
  boolean presence;
} Measure;

