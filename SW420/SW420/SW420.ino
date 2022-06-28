const int vibrationPin = 16;
int vibrationVal = 0;

void setup() {
  Serial.begin(9600);
  pinMode(vibrationPin, INPUT);

}

void loop() {
  vibrationVal = digitalRead(vibrationPin);
  Serial.print(vibrationVal);
  Serial.println();

  delay(100);
}
