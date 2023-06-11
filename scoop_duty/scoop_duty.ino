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

int mode = MODE_HOUR;
bool buttonPressed = false;
int buttonPressDuration = 0;
int buttonReleaseType = RELEASE_NONE;
int delayInterval = 50;
void loop() {
  if (mode == MODE_HOUR)
    hourSetup();
  else if (mode == MODE_MINUTE)
    minuteSetup();
  else
    mainMode();

  bool nextButtonPressed = (digitalRead(BUTTON_PIN) == LOW);
  bool buttonChanged = buttonPressed != nextButtonPressed;
  buttonPressed = nextButtonPressed;

  if (buttonPressed) {
    buttonPressDuration ++;
    buttonReleaseType = RELEASE_NONE;
    if (buttonChanged) {
      // on push
    }
  } else {
    if (buttonChanged) {
      // on release
      if (buttonPressDuration < (1 * 15)) {
        buttonReleaseType = RELEASE_SHORT;
      } else {
        buttonReleaseType = RELEASE_LONG;
      }
    } else {
      buttonReleaseType = RELEASE_NONE;
    }
    buttonPressDuration = 0;
  }
  updateLights();
  delay(delayInterval);
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
    hour ++;
  }
}

int minute = 0;
int millisOffset = 0;
void minuteSetup() {
  leftOn = false;
  rightOn = true;

  if (buttonReleaseType == RELEASE_LONG) {
    jingle();
    millisOffset = (hour * 60 + minute) * 60 * 1000;
    mode = MODE_MAIN;
  } else if (buttonReleaseType == RELEASE_SHORT) {
    lowBeep();
    minute ++;
  }
}

// FIXME: why is the time so inaccurate??

#define MILLIS_24H 86400000
unsigned long currentMillis = 0;
bool leftTurn = true;
void mainMode() {
  unsigned long nextMillis = (millis() + millisOffset);
  if ((unsigned long)(nextMillis - currentMillis) >= (unsigned long)MILLIS_24H) {
    leftTurn = !leftTurn;
    currentMillis = nextMillis;
  }
 
  leftOn = leftTurn;
  rightOn = !leftTurn;

  if (buttonReleaseType == RELEASE_SHORT) {
    lowBeep();
    leftTurn = !leftTurn;
  }
}
