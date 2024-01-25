/*
  State change detection (edge detection)

  Often, you don't need to know the state of a digital input all the time, but
  you just need to know when the input changes from one state to another.
  For example, you want to know when a button goes from OFF to ON. This is called
  state change detection, or edge detection.

  This example shows how to detect when a button or button changes from off to on
  and on to off.

  The circuit:
  - pushbutton attached to pin 2 from +5V
  - 10 kilohm resistor attached to pin 2 from ground
  - LED attached from pin 13 to ground through 220 ohm resistor (or use the
    built-in LED on most Arduino boards)

  created  27 Sep 2005
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/StateChangeDetection
*/

 //
 // Parse incoming messages consisting of three decimal values followed by a carriage return
 //  Example  "12 34 56\n"
 //  In TouchDesigner:     op('serial1').send("12 34 56", terminator="\n")
 //
 char buffer[16];   //maximum expected length 
 int len = 0;

int buttonPins[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A13, A15};
char *buttonNames[] = {"w1", "w2", "w3", "w4", "w5", "w6", "y1", "y2", "b1", "b2", "g1", "g2", "r1", "r2"};
int buttonPinsSize, joyPinsSize, ledPinsSize;


int joyPins[] = {26, 27, 28, 29};
char *joyNames[] = {"j1_left", "j1_right", "j2_left", "j2_right"};

int ledPins[] = {14, 15, 16, 17, 18, 19, 20, 21};
char *ledNames[] = {"r1", "r2", "w1", "w2", "w3", "w4", "w5", "w6"}; //  (digital pins)

// Variables will change:
int buttonStates[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0};        // current state of the button
int lastButtonStates[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0};    // previous state of the button

int joyStates[] = {0, 0, 0, 0};
int lastJoyStates[] = {0, 0, 0, 0};

void setup() {
  buttonPinsSize = sizeof(buttonPins) / sizeof(int);
  for (int i = 0; i < buttonPinsSize; ++i) {
    pinMode(buttonPins[i], INPUT);
  }

  joyPinsSize = sizeof(joyPins) / sizeof(int);
  for (int i = 0; i < joyPinsSize; ++i) {
    pinMode(joyPins[i], INPUT);
  }

  ledPinsSize = sizeof(ledPins) / sizeof(int);
  for (int i = 0; i < ledPinsSize; ++i) {
    pinMode(ledPins[i], OUTPUT);
  }

  // initialize serial communication:
  Serial.begin(9600);
}

void loop() {
  checkIncomingMessage();

  // read the pushbutton input pin:
  for (int i = 0; i < buttonPinsSize; ++i) {
    buttonStates[i] = digitalRead(buttonPins[i]);
    if (buttonStates[i] != lastButtonStates[i]) {
      if (buttonStates[i] == HIGH) {
        Serial.println("button state %s is on", buttonNames[i]);
        //buttonOn(i);
      } else {
        Serial.println("button state %s is off", buttonNames[i]);
        //buttonOff(i);
      }
    }
  }

  for (int i = 0; i < joyPinsSize; ++i) {
    joyStates[i] = digitalRead(joyPins[i]);
    if (joyStates[i] != lastJoyStates[i]) {
      if (joyStates[i] == HIGH) {
        Serial.println("joy state %s is on", joyNames[i]);
        //buttonOn(i);
      } else {
        Serial.println("button state %s is off", joyNames[i]);
        //buttonOff(i);
      }
    }
  }
  // Delay a little bit to avoid bouncing
  delay(50);
  Serial.println("setting current button states");
  for (int i = 0; i < buttonPinsSize; ++i) {
    lastButtonStates[i] = buttonStates[i];
  }
  for (int i = 0; i < buttonPinsSize; ++i) {
    lastJoyStates[i] = joyStates[i];
  }
}

void checkIncomingMessage() {
  Serial.println("checking incoming messages");
  if (Serial.available() > 0) {
    Serial.println("serial has message");
    int incomingByte = Serial.read();
    buffer[len++] = incomingByte;

    Serial.println("buffer = %s", buffer);

    // check for overflow
    if (len >= 16) {
        // overflow, resetting
        len = 0;
    }

    char[2] led;
    char[1] state;
    int stateNum, ledPin;
    // check for newline (end of message)
    if (incomingByte == '\n') {
        Serial.println("incoming byte detected");
        Serial.println("finished buffer = %s", buffer);

        len = 0; // reset buffer counter
        led = strtok(buffer, "_");

        if (led == NULL) {
          Serial.println("error in led");
          return;
        }
        ledPin = parseLed(led);
        Serial.println("parsed led is %d", ledPin);
        if (ledPin == -1) {
          return;
        }
      
        state = strktok(buffer, "_");
        if (state == NULL) {
          Serial.println("error in state");
          return;
        }
        if (!isdigit(state)) {
          Serial.println("state is not digit");
          return;
        }

        stateNum = atoi(state);
        Serial.println("parsed state is %d", stateNum);

    }

    Serial.println("writing led pin %d, state %d", ledPin, stateNum);
    digitalWrite(ledPin, stateNum == 1 ? HIGH : LOW);
  }
}

int parseLed(char* led) {
  for (int i; i < ledPinsSize; ++i) {
    if (strcmp(ledNames[i], led) == 0) {
      return ledPins[i];
    }
  }
  Serial.println("led not found: %s", led);
  return -1;
}
