// ESP32板载LED引脚
const int ledPin = 4;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}
void loop() {
  currentMillis = millis();
}