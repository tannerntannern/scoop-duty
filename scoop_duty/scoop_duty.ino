int leftLights[] = {8, 7, 6, 5};
int rightLights[] = {12, 11, 10, 9};
int BUTTON_PIN = 4;
int BEEPER_PIN = 2;

void multiPinMode(int pins[], int pinCount, int mode) {
  for (int i = 0; i < pinCount; i ++)
    pinMode(pins[i], mode);
} 

void multiWrite(int pins[], int pinCount, int data) {
  for (int i = 0; i < pinCount; i ++)
    digitalWrite(pins[i], data);
}

void setup() {
  pinMode(BEEPER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  multiPinMode(leftLights, 4, OUTPUT);
  multiPinMode(rightLights, 4, OUTPUT);
}

void loop() {
  noTone(BEEPER_PIN);
  if (digitalRead(BUTTON_PIN) == HIGH) {
    multiWrite(leftLights, 4, HIGH);
    multiWrite(rightLights, 4, LOW);
  } else {
    tone(BEEPER_PIN, 880);
    multiWrite(leftLights, 4, LOW);
    multiWrite(rightLights, 4, HIGH);
  }
  delay(100);
}
