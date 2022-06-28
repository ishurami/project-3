#include <ESP8266WiFi.h>
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include "DHT.h"

// Thingspeak setup
#define SECRET_SSID "skematology"  // Change this to your WIFI name
#define SECRET_PASS "skematology"  // Change this to your WIFI password
#define SECRET_CH_ID 1345830
#define SECRET_WRITE_APIKEY "0O6B77OMVMAC7SZL"
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// WIFI setup
char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
WiFiClient  client;

// Input pin and setup
const int VIBRATION_PIN = 3;
const int IR_PIN = 5;
const int DHT_PIN = 4;
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

// Output pin
const int BUZZER_VIBRATION_PIN = 14;
const int BUZZER_IR_PIN = 12;
const int BUZZER_DHT_PIN = 13;

// Set value
const int maxRPM = 200;
const int maxTemperature = 32;

// Variable
int vibrationVal = 0;
float temperatureVal = 0;
int RPM = 0;
int count = 0;
String buzzerStatus = "";

// Variable time for millis
unsigned long current_time = 0;
unsigned long previous_count_rpm = 0;
unsigned long previous_buzzer_ir = 0;
unsigned long previous_buzzer_temperature = 0;
unsigned long previous_send_thinkspeak = 0;

ICACHE_RAM_ATTR void countRevolution() {
  count++;
}

int second(float sec) {
  return sec * 1000;
}

void setup() {
  Serial.begin(9600);
  
  pinMode(VIBRATION_PIN, INPUT);
  pinMode(IR_PIN, INPUT);
  
  pinMode(BUZZER_VIBRATION_PIN, OUTPUT);
  digitalWrite(BUZZER_VIBRATION_PIN, HIGH);
  pinMode(BUZZER_IR_PIN, OUTPUT);
  digitalWrite(BUZZER_IR_PIN, HIGH);
  pinMode(BUZZER_DHT_PIN, OUTPUT);
  digitalWrite(BUZZER_DHT_PIN, HIGH);
  
  attachInterrupt(digitalPinToInterrupt(IR_PIN), countRevolution, RISING);

  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);
  dht.begin();
}

void loop() {
  if (isSetupWifiSuccess()) {
    current_time = millis();
  
    getSensorData();
    serialPrint();
    sendSensorDataToThingspeak();
  } else {
    Serial.println("WIFI is not connected.");
  }
}

bool isSetupWifiSuccess() {
  int attempts = 1;
   
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED && attempts <=3 ){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
      attempts++;
    } 
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConnected.");
    }
    return false;
  }
  return true;
}

void getSensorData() {
  buzzerStatus = "";
  vibrationVal = digitalRead(VIBRATION_PIN);
  temperatureVal = dht.readTemperature();
  
  if (vibrationVal) {
    digitalWrite(BUZZER_VIBRATION_PIN, LOW);
    buzzerStatus += "Buzzer Vibration ON.\n";
  } else {
    digitalWrite(BUZZER_VIBRATION_PIN, HIGH);
  }

  if ((current_time - previous_count_rpm) >= second(1)) {
    RPM = count * 60;
    count = 0;
    previous_count_rpm = millis();
  }
  
  if (RPM > maxRPM) {
    if ((current_time - previous_buzzer_ir) >= second(1)) {
      digitalWrite(BUZZER_IR_PIN, !(digitalRead(BUZZER_IR_PIN)));
      previous_buzzer_ir = millis();
    }
    buzzerStatus += "Buzzer IR ON.\n";
  } else {
    digitalWrite(BUZZER_IR_PIN, HIGH);
  }
  
  if (temperatureVal >= maxTemperature) {
    if ((current_time - previous_buzzer_temperature) >= second(0.1)) {
      digitalWrite(BUZZER_DHT_PIN, !(digitalRead(BUZZER_DHT_PIN)));
      previous_buzzer_temperature = millis();
    }
    buzzerStatus += "Buzzer Temperature ON.";
  } else {
    digitalWrite(BUZZER_DHT_PIN, HIGH);
  }
}

void serialPrint() {
  Serial.print(vibrationVal);
  Serial.print(" - ");
  Serial.print(RPM);
  Serial.print(" - ");
  Serial.print(temperatureVal);
  Serial.print(" - ");
  Serial.print(buzzerStatus);
  Serial.println();
}

void sendSensorDataToThingspeak() {
  if ((current_time - previous_send_thinkspeak) >= second(15)) {
    ThingSpeak.setField(1, vibrationVal);
    ThingSpeak.setField(2, RPM);
    ThingSpeak.setField(3, temperatureVal);
    ThingSpeak.setStatus(buzzerStatus);
  
    int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(httpCode == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(httpCode));
    }
    
    previous_send_thinkspeak = millis();
  }
}

