// KY-037 Microphone Test
// Read analog values and print to Serial Monitor

const int micPin = A0; // analog pin connected to KY-037 A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  int level = analogRead(micPin); // read analog value (0-1023)
  Serial.println(level);
  delay(100); // 10 readings per second
}