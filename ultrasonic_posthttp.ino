#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <base64.h>
#include <ArduinoJson.h>

#define trigPin D5
#define echoPin D6
#define ledPin D0

const char* ssid = "Galaxy A512572";
const char* password = "farahcantik";
String authUsername = "haninfaishal13";
String authPassword = "hidupmulia13";

char jsonOutput1[128];
HTTPClient http;
 
void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.println();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(ledPin, LOW);
  }
  digitalWrite(ledPin, HIGH);
  Serial.println();
}
 
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    setup();
  }
  String auth = base64::encode(authUsername + ":" + authPassword);
  http.begin("http://192.168.43.42:5000/channel");
  http.addHeader("Authorization", "Basic " + auth);
  http.addHeader("Content-Type", "application/json");

  long duration, inches, cm;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  
  const size_t Ultrasonic = JSON_OBJECT_SIZE(3);
  StaticJsonDocument<Ultrasonic> ultrasonic;
  JsonObject object = ultrasonic.to<JsonObject>();
  object["value"] = cm; //variable (?)
  object["id_sensor"] = 23;  
  serializeJson(ultrasonic, jsonOutput1);
  int httpCode1 = http.POST(String(jsonOutput1));
  
  String payload = http.getString();
  Serial.println(payload);
  http.end();
  delay(5000);
 
}
 
long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}
long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
