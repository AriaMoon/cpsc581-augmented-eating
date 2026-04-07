//COMMON ANODE RGB: HIGH = OFF, LOW = ON

/*
//test code
// COMMON ANODE RGB LED - basic RGB cycle
// Pin mapping for your Arduino Nano
// Blue → 9, Green → 10, Red → 11
// Resistors: 270 ohms per color

int bluePin = 9;
int greenPin = 10;
int redPin = 11;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // Red
  setColor(255, 0, 0);
  delay(1000);

  // Green
  setColor(0, 255, 0);
  delay(1000);

  // Blue
  setColor(0, 0, 255);
  delay(1000);
}

// Function to set color with inversion for common anode
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, 255 - redValue);
  analogWrite(greenPin, 255 - greenValue);
  analogWrite(bluePin, 255 - blueValue);
}
*/

///////////////////////////////////////////////////////////////
// COMMON ANODE RGB LED - smooth rainbow fade
// Pin mapping for Arduino Nano
// Blue → 9, Green → 10, Red → 11
// Resistors: 270 ohms per color

int bluePin = 9;
int greenPin = 10;
int redPin = 11;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // Cycle through hue values from 0 to 255
  for (int i = 0; i < 256; i++) {
    // Convert hue to RGB
    int r, g, b;
    hsvToRgb(i, 255, 255, r, g, b);
    setColor(r, g, b);
    delay(20); // smaller = faster fade
  }
}

// Function to set color with inversion for common anode
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, 255 - redValue);
  analogWrite(greenPin, 255 - greenValue);
  analogWrite(bluePin, 255 - blueValue);
}

// HSV (Hue, Saturation, Value) to RGB conversion
void hsvToRgb(int h, int s, int v, int &r, int &g, int &b) {
  float hf = h / 255.0 * 6.0;
  int i = int(hf);
  float f = hf - i;
  float p = v * (1 - s / 255.0);
  float q = v * (1 - f * s / 255.0);
  float t = v * (1 - (1 - f) * s / 255.0);

  switch (i % 6) {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    case 5: r = v; g = p; b = q; break;
  }
}