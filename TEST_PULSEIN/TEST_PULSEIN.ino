int sensor = 3;

void setup() {
  Serial.begin(9600);
  pinMode(sensor, INPUT);
}

void loop() {
  unsigned long measurment = getVibration();
  Serial.println(measurment);
}

unsigned long getVibration() {
  unsigned long measurment = pulseIn(sensor, HIGH);
  return measurment;
}
