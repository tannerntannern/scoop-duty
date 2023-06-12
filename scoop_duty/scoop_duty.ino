#define MODE_HOUR 1
#define MODE_MINUTE 2
#define MODE_MAIN 3

int leftLights[] = {8, 9, 10, 11};
int rightLights[] = {3, 4, 5, 6};
int BUTTON_PIN = 12;
int BEEPER_PIN = 2;

void multiPinMode(int pins[], int pinCount, int mode) {
  for (int i = 0; i < pinCount; i ++)
    pinMode(pins[i], mode);
} 

void multiWrite(int pins[], int pinCount, int data) {
  for (int i = 0; i < pinCount; i ++)
    digitalWrite(pins[i], data);
}

void lowBeep() {
  tone(BEEPER_PIN, 800, 250);
}

void jingle() {
  tone(BEEPER_PIN, 800, 90);
  delay(90);
  tone(BEEPER_PIN, 1200, 90);
  delay(90);
  tone(BEEPER_PIN, 1800, 90);
  delay(90);
  tone(BEEPER_PIN, 2700, 90);
}

void setup() {
  pinMode(BEEPER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  multiPinMode(leftLights, 4, OUTPUT);
  multiPinMode(rightLights, 4, OUTPUT);
  jingle();
}

bool leftOn = false;
bool rightOn = false;
void updateLights() {
  multiWrite(leftLights, 4, leftOn ? HIGH : LOW);
  multiWrite(rightLights, 4, rightOn ? HIGH : LOW);
}

#define RELEASE_NONE 0
#define RELEASE_SHORT 1
#define RELEASE_LONG 2
#define DELAY_INTERVAL 50

int mode = MODE_HOUR;
bool buttonPressed = false;
int buttonPressDuration = 0;
int buttonReleaseType = RELEASE_NONE;
void loop() {
  bool nextButtonPressed = (digitalRead(BUTTON_PIN) == LOW);
  bool buttonChanged = buttonPressed != nextButtonPressed;
  buttonPressed = nextButtonPressed;

  if (buttonPressed) {
    buttonPressDuration += DELAY_INTERVAL;
    buttonReleaseType = RELEASE_NONE;
  } else {
    if (buttonChanged) {
      if (buttonPressDuration < 500) {
        buttonReleaseType = RELEASE_SHORT;
      } else {
        buttonReleaseType = RELEASE_LONG;
      }
    } else {
      buttonReleaseType = RELEASE_NONE;
    }
    buttonPressDuration = 0;
  }

  if (mode == MODE_HOUR) {
    hourSetup();
  } else if (mode == MODE_MINUTE) {
    minuteSetup();
  } else {
    mainMode();
  }

  updateLights();
  delay(DELAY_INTERVAL);
}

int hour = 0;
void hourSetup() {
  leftOn = true;
  rightOn = false;

  if (buttonReleaseType == RELEASE_LONG) {
    jingle();
    mode = MODE_MINUTE;  
  } else if (buttonReleaseType == RELEASE_SHORT) {
    lowBeep();
    hour = (hour + 1) % 24;
  }
}

int minute = 0;
unsigned long millisOffset = 0;
void minuteSetup() {
  leftOn = false;
  rightOn = true;

  if (buttonReleaseType == RELEASE_LONG) {
    jingle();
    millisOffset = ((unsigned long)hour * (unsigned long)60 + (unsigned long)minute) * (unsigned long)60 * (unsigned long)1000;
    mode = MODE_MAIN;
  } else if (buttonReleaseType == RELEASE_SHORT) {
    lowBeep();
    minute = (minute + 1) % 60;
  }
}

const unsigned long switchInterval = 86400000;  // 24h in millis
unsigned long previousMillis = 0;
bool leftTurn = true;
void mainMode() {
  unsigned long currentMillis = millis() + millisOffset;
  if (((unsigned long)(currentMillis - previousMillis)) >= switchInterval) {
    leftTurn = !leftTurn;
    previousMillis = currentMillis;
  }
 
  leftOn = leftTurn;
  rightOn = !leftTurn;

  if (buttonReleaseType == RELEASE_SHORT) {
    lowBeep();
    leftTurn = !leftTurn;
  }
}
