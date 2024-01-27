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

 enum BUTTON_STATE {
  UNPRESSED,
  PRESSED
 };

int buttonPins[] = {A0/*, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A13, A15*/};
char *buttonNames[] = {"w_1", "w_2", "w_3", "w_4", "w_5", "w_6", "y_1", "y_2", "b_1", "b_2", "g_1", "g_2", "r_1", "r_2"};
//int buttonPins[] = {A0 };
//char *buttonNames[] = {"w1"};
int buttonPinsSize, joyPinsSize, ledPinsSize;


//int joyPins[] = {26, 27};
//char *joyNames[] = {"j1_left", "j1_right"};
int joyPins[] = {26, 27, 28, 29};
char *joyNames[] = {"j1_left", "j1_right", "j2_left", "j2_right"};

//int ledPins[] = {14, 15, 16, 17, 18, 19, 20, 21};
int ledPins[] = {15, 14, 21, 20, 19, 18, 17, 16};
char *ledNames[] = {"r1", "r2", "w1", "w2", "w3", "w4", "w5", "w6"}; //  (digital pins)

// Variables will change:
int buttonStates[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};        // current state of the button
int lastButtonStates[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};      // previous state of the button

// Variables will change:
//int buttonStates[] = {0};        // current state of the button
//int lastButtonStates[] = {0};      // previous state of the button

int joyStates[] = {0, 0, 0, 0};
int lastJoyStates[] = {0, 0, 0, 0};
int lastLastJoyStates[] = {0, 0, 0, 0};
int storedJoyStates[] = {0, 0, 0, 0};

void setup() {
    //Serial.print("joy ");
    Serial.begin(115200);


  Serial.print("INITIALIZING \n");
  buttonPinsSize = sizeof(buttonPins) / sizeof(int);
  for (int i = 0; i < buttonPinsSize; ++i) {
    Serial.print("registrando pin mode: ");
    
    Serial.print(buttonPins[i]);
     Serial.print(" nome: ");
    Serial.print(buttonNames[i]);
    Serial.print("\n");
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  joyPinsSize = sizeof(joyPins) / sizeof(int);
  for (int i = 0; i < joyPinsSize; ++i) {
    pinMode(joyPins[i], INPUT_PULLUP);
  }

  ledPinsSize = sizeof(ledPins) / sizeof(int);
  for (int i = 0; i < ledPinsSize; ++i) {
    pinMode(ledPins[i], OUTPUT);
  }

  // initialize serial communication:
  }

void printByte(char * bits) {
  unsigned int byt;
  byt = bitsToBytes(bits);
  Serial.print(byt);
  Serial.print("\n");
  delay(500);
}


void loop() {
  checkIncomingMessage();

  // read the pushbutton input pin:
  for (int i = 0; i < buttonPinsSize; ++i) {
    int val = digitalRead(buttonPins[i]);
    //Serial.print(val);
    //Serial.print("\n");
    if (val == 1) {
      buttonStates[i] = UNPRESSED;
    } else {
      buttonStates[i] = PRESSED;
    }
    /*Serial.print("Last button state is: ");
    Serial.print(lastButtonStates[i]);
    Serial.print(" , current button state is: ");
    Serial.print(buttonStates[i]);
    Serial.print("\n");*/
    if (buttonStates[i] != lastButtonStates[i]) {
      if (buttonStates[i] == PRESSED) {
        Serial.print(buttonNames[i]);
        Serial.print("_p");
        Serial.print("\n");
        //digitalWrite(ledPins[i], LOW);

        //buttonOn(i);
      } else {
        Serial.print(buttonNames[i]);
        Serial.print("_u");
        Serial.print("\n");
        //digitalWrite(ledPins[i], HIGH);
      }
    }
  }

  for (int i = 0; i < joyPinsSize; ++i) {
    int val = digitalRead(joyPins[i]);
    /*Serial.print("joy ");
    Serial.print(i);
    Serial.print(" ");
    Serial.print(val);
    Serial.print("\n");*/
    if (val == 1) {
      joyStates[i] = 0;
    } else {
      joyStates[i] = 1;
    }
    /*Serial.print("joy : ");
    Serial.print(joyPins[i]);
    Serial.print("state val: ");
    Serial.print(joyStates[i]);
    Serial.print("   ");*/
    if (joyStates[i] != storedJoyStates[i] && lastJoyStates[i] != storedJoyStates[i] && lastLastJoyStates[i] != storedJoyStates[i]) {
      if (joyStates[i] == 1) {
        Serial.print("joy ");
        Serial.print(joyNames[i]);
        Serial.print("state is on \n");
        storedJoyStates[i] = 1;
        //buttonOn(i);
      } else {
        Serial.print("joy ");
        Serial.print(joyNames[i]);
        Serial.print("state is off \n");
        storedJoyStates[i] = 0;
        //buttonOff(i);
      }
    }
  }
  //Serial.print("\n");
  // Delay a little bit to avoid bouncing
  delay(50);
  //Serial.print("setting current button states \n");
  for (int i = 0; i < buttonPinsSize; ++i) {
    lastButtonStates[i] = buttonStates[i];

    //Serial.print("Updating last button state to: ");
    //Serial.print(lastButtonStates[i]);
    //Serial.print("\n");

  }
  for (int i = 0; i < joyPinsSize; ++i) {
    lastLastJoyStates[i] = lastJoyStates[i];
    lastJoyStates[i] = joyStates[i];
  }
  //Serial.print("---------------------------");
  //Serial.print("\n");
}

void checkIncomingMessage() {
  //Serial.print("checking incoming messages \n");
  if (Serial.available() > 0) {
    //Serial.print("serial has message \n");
    char receivedChar = Serial.read();

    Serial.print("----------");
    Serial.print("ledState is: ");
    for (int i = 0; i < ledPinsSize; ++i) {
      bool ledState = (receivedChar >> i) & 1;
      Serial.print(ledState);
      Serial.print(" ");
      digitalWrite(ledPins[i], ledState);
    }
    Serial.print("\n");
    

  /*if (Serial.available() > 0) {
    //Serial.print("serial has message \n");
    char incomingByte = Serial.read();
    buffer[len++] = incomingByte;

    //Serial.print("buffer \n");

    // check for overflow
    if (len >= 16) {
        // overflow, resetting
        len = 0;
    }

    char* led;
    char* state;
    int stateNum, ledPin;
    // check for newline (end of message)
    if (incomingByte == '\n') {
        //Serial.print("incoming byte detected \n");
        Serial.print("----- incoming message: ");
          Serial.print(buffer);
          Serial.print("\n");
        
        if (strcmp('n', buffer[0]) == 0) {
          Serial.print("buffer is n");
          Serial.print("\n");
          for (int i = 0; i < ledPinsSize; ++i) {
            digitalWrite(ledPins[i], HIGH);
          }
          return;
        }
        if (strcmp('a', buffer[0]) == 0) {
          Serial.print("buffer is a");
          Serial.print("\n");
          for (int i = 0; i < ledPinsSize; ++i) {
            digitalWrite(ledPins[i], LOW);
          }
          return;
        }
        len = 0; // reset buffer counter
        led = strtok(buffer, "_");

        if (led == NULL) {
          Serial.print("error in led \n");
          return;
        }

        ledPin = parseLed(led);
        //Serial.print("parsed led is ");
        //Serial.print(ledPin);
        //Serial.print("\n");
        if (ledPin == -1) {
          Serial.print("wrong message: ");
          Serial.print(buffer);
          Serial.print("\n");
          return;
        }
      
        state = strtok(NULL, "_");
        if (state == NULL) {
          Serial.print("error in state \n");
          return;
        }
        //Serial.print("state is ");
        //Serial.print(state[0]);
        //Serial.print("\n");
        if (!isdigit(state[0])) {
          Serial.print("state is not digit \n");
          Serial.print("wrong message: ");
          Serial.print(buffer);
          Serial.print("\n");
          return;
        }

        stateNum = state[0] - '0';
        //Serial.print("stateNum; ");
        //Serial.print(stateNum);
        //Serial.print("parsed state is");
        //Serial.print(stateNum);
        //Serial.print("is it high? ");
        //Serial.print(stateNum == 1);
        //Serial.print("\n");
    }

    Serial.print("writing led pin, pin ");
    Serial.print(ledPin);
    Serial.print(" state ");
    Serial.print(stateNum);
    Serial.print("\n");

    digitalWrite(ledPin, stateNum == 1 ? HIGH : LOW);
    Serial.flush();*/
  }
}

unsigned int bitsToBytes(unsigned char *bits) {
    unsigned int sum = 0;
    for (int i = 0; i < 8; i++) {
        sum = (sum << 1) + (bits[i] - '0');
    }
    return sum;
}

int parseLed(char* led) {
  //Serial.print("parse led , par: ");
  //Serial.print(led);
  //Serial.print("\n");

  for (int i = 0; i < ledPinsSize; ++i) {

    if (strcmp(ledNames[i], led) == 0) {
      //Serial.print("ledNames , name ");
      //Serial.print(ledNames[i]);
      //Serial.print("\n");
      return ledPins[i];
    }
  }
  Serial.print("led not found \n");
  Serial.print(led);
  return -1;
}
