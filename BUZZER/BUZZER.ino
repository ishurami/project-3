#define IR_PIN      16
#define BUZZER_PIN  14

void setup() {
  Serial.begin(9600);
  pinMode(IR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);
}

void loop() {
  Serial.print(digitalRead(IR_PIN));
  Serial.print(" - "  );
  Serial.println(digitalRead(BUZZER_PIN));
  
  if (digitalRead(IR_PIN)) {
    digitalWrite(BUZZER_PIN, LOW);
  } else {
    digitalWrite(BUZZER_PIN, HIGH);
  }
}
