int sensor = 5; // GPIO 5
unsigned long start_time = 0;
unsigned long end_time = 0;
int steps=0;
float steps_old=0;
float temp=0;
float rps=0;
volatile int count = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(sensor, INPUT);

  attachInterrupt(digitalPinToInterrupt(sensor), countRevolution, RISING);
}
 
void loop()
{
  start_time = millis();
//  
//  while (millis() < end_time) {
//   if (digitalRead(sensor)) {
//    steps = steps + 1; 
//    while (digitalRead(sensor));
//   }
//  }
//  temp = steps - steps_old;
//  steps_old = steps;
//  rps = (temp / 20);
//
//  Serial.println(rps);
//
// ==================================
//
//  start_time = millis();
//  end_time = start_time + 1000;
//
//  while (millis() < end_time) {
//    if (digitalRead(sensor)) {
//      steps++;
//      Serial.print("masuk - ");
//      Serial.print(steps);
//      while (digitalRead(sensor));
//    }
//  }
//  Serial.print(start_time);
//  Serial.print(" - ");
//  Serial.print(end_time);
//  Serial.print(" - ");
//  Serial.println(steps);
//  
//  steps = 0;

  if ((start_time - end_time) >= 1000) {
    Serial.print("GPIO: ");
    Serial.print(sensor);
    Serial.print(" | InterruptPin: ");
    Serial.print(digitalPinToInterrupt(sensor));
    Serial.print(" - "); 
    Serial.print(digitalRead(sensor));
    Serial.print(" - "); 
    Serial.print(count * 60);
    end_time = millis();
    count = 0;
  }
  Serial.println();

//  delay(500);
}


ICACHE_RAM_ATTR void countRevolution() {
  count++;
}
