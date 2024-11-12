int buttonPins[4] = { 32, 31, 30, 29 };
int ledPins[4] = { 1, 3, 5, 7 };

bool buttonStates[4] = { LOW, LOW, LOW, LOW };
bool lastButtonStates[4] = { LOW, LOW, LOW, LOW };
bool buttonOn[4] = { LOW, LOW, LOW, LOW };

int pinTotal = 4;

unsigned long lastStepTime = 0;
unsigned long lastRandomTime = 0;

int currentStep = 0;
int totalStep = 4;
int stepLength = 1000;
int potPin = A14;

void setup() {
  for (int i = 0; i < pinTotal; i++) {
    pinMode(buttonPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  sequence();
  checkButtons();
  checkLeds();
}

void sequence() {
  int stepLength = analogRead(potPin);
  if (millis() - lastStepTime > stepLength) {
    lastStepTime = millis();
    usbMIDI.sendNoteOff(60, 0, 1);
    nextStepForward();
    if (buttonOn[currentStep]) {
      usbMIDI.sendNoteOn(60, 127, 1);
    }
  }
}

void checkButtons() {
  for (int i = 0; i < 4; i++) {
    lastButtonStates[i] = buttonStates[i];
    buttonStates[i] = digitalRead(buttonPins[i]);

    if (lastButtonStates[i] == LOW and buttonStates[i] == HIGH) {
      buttonOn[i] = !buttonOn[i];
      delay(5);
    }
  }
}

void checkLeds() {
  for (int i = 0; i < pinTotal; i++) {
    if (i == currentStep) {
      analogWrite(ledPins[i], 255);
    } else if (buttonOn[i] == true) {
      analogWrite(ledPins[i], 20);
    } else {
      analogWrite(ledPins[i], 0);
    }
  }
}

void nextStepForward() {
  currentStep = currentStep + 1;
  if (currentStep >= totalStep) {
    currentStep = 0;
  }
}