// talking table final iteration: RGB + Mic + Button + Simulated Pressure Sensor
//in the future: pressure sensor triggered start, vibration motor

const int buttonPin = 2;
const int micPin = A0;
const int bluePin = 9;
const int greenPin = 10;
const int redPin = 11;
const int micDigitalPin = 3;

// --- Game mode ---
bool gameMode = false;
unsigned long gameStart = 0;
const unsigned long gameDuration = 6 * 1000;

// --- Session (pressure sensor simulation) ---
bool sessionActive = false;
bool sessionStartedOnce = false;
unsigned long sessionStart = 0;
const unsigned long sessionDuration = 60 * 1000; // 1 minute
unsigned long startupTime = 0;

// --- Color ---
int hue = 0;
float currentR = 0;
float currentG = 0;
float currentB = 0;
const float smoothing = 0.1;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(micDigitalPin, INPUT);

  startupTime = millis();
  //for mic debugging
  Serial.begin(9600); // in setup
}

void loop() {


  // --- Simulated pressure sensor (auto start after 3s) ---
  if (!sessionStartedOnce && millis() - startupTime > 3000) {
    sessionActive = true;
    sessionStart = millis();
    sessionStartedOnce = true;
  }

  // --- If no one "at table" ---
  if (!sessionActive) {
    setColor(0, 0, 0);
    return;
  }

  // --- End session after 1 minute ---
  if (millis() - sessionStart > sessionDuration) {
    sessionActive = false;
    setColor(0, 0, 0);
    return;
  }

  // --- Button handling ---
  static bool lastButtonState = HIGH;
  bool reading = digitalRead(buttonPin);

  if (reading == LOW && lastButtonState == HIGH) {
    if (!gameMode) {
      gameMode = true;
      gameStart = millis();
    } else {
      gameMode = false;
    }
    delay(50);
  }
  lastButtonState = reading;

  // --- Game mode ---
  if (gameMode) {
    if (millis() - gameStart < gameDuration) {
      rainbowFadeStep();
    } else {
      gameMode = false;
    }
  } 
  // --- Ambient mic mode ---
  else {
    ambientMicMode();
  }
}

// 🌈 Rainbow (faster + smooth)
void rainbowFadeStep() {
  int r, g, b;
  hsvToRgb(hue, 255, 255, r, g, b);
  smoothSetColor(r, g, b);
  hue = (hue + 3) % 256; // faster color cycling
  delay(10);
}

void ambientMicMode() {
  int sound = digitalRead(micDigitalPin);

  int targetR, targetG, targetB;

  if (sound == HIGH) {
    // Loud → warm, active color
    targetR = 255; targetG = 100; targetB = 0; // orange/red
  } else {
    // Quiet → calm color
    targetR = 0; targetG = 0; targetB = 255; // blue
  }

  smoothSetColor(targetR, targetG, targetB);
  delay(20);
}

// ✨ Smooth transition (used everywhere)
void smoothSetColor(int targetR, int targetG, int targetB) {
  currentR += (targetR - currentR) * smoothing;
  currentG += (targetG - currentG) * smoothing;
  currentB += (targetB - currentB) * smoothing;

  setColor((int)currentR, (int)currentG, (int)currentB);
}

// 🔌 Common anode output
void setColor(int r, int g, int b) {
  analogWrite(redPin, 255 - r);
  analogWrite(greenPin, 255 - g);
  analogWrite(bluePin, 255 - b);
}

// 🎨 HSV → RGB
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