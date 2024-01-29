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

 
#define DEBUG

 char buffer[16];   //maximum expected length 
 int len = 0;

 enum BUTTON_STATE {
  UNPRESSED,
  PRESSED
 };

int buttonPins[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A13, A15};
char *buttonNames[] = {"w_1", "w_2", "w_3", "w_4", "w_5", "w_6", "y_1", "y_2", "b_1", "b_2", "g_1", "g_2", "r_1", "r_2"};
//int buttonPins[] = {A0 };
//char *buttonNames[] = {"w1"};
int buttonPinsSize, joyPinsSize, ledPinsSize;


//int joyPins[] = {26, 27};
//char *joyNames[] = {"j1_left", "j1_right"};
int joyPins[] = {26, 27, 28, 29};
char *joyNames[] = {"j_1_l", "j_1_r", "j_2_l", "j_2_r"};

//int ledPins[] = {14, 15, 16, 17, 18, 19, 20, 21};
int ledPins[] = {14, 15, 16, 17, 18, 19, 20, 21};
char *ledNames[] = {"r1", "r2", "w1", "w2", "w3", "w4", "w5", "w6"}; //  (digital pins)

// Variables will change:
int buttonStates[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};        // current state of the button
int lastButtonStates[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};      // previous state of the button
int storedButtonStates[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int joyStates[] = {0, 0, 0, 0};
int lastJoyStates[] = {0, 0, 0, 0};
int lastLastJoyStates[] = {0, 0, 0, 0};
int storedJoyStates[] = {0, 0, 0, 0};

void setup() {
  // initialize serial communication:
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
}

void loop() {
  checkIncomingMessage();

  // read the pushbutton input pin:
  for (int i = 0; i < buttonPinsSize; ++i) {
    int val = digitalRead(buttonPins[i]);
    #ifdef DEBUG
      //Serial.print(val);
      //Serial.print("\n");
    #endif
    if (val == 1) {
      buttonStates[i] = UNPRESSED;
    } else {
      buttonStates[i] = PRESSED;
    }

    if (buttonStates[i] != storedButtonStates[i] && lastButtonStates[i] != storedButtonStates[i]) {
      if (buttonStates[i] == PRESSED) {
        Serial.print(buttonNames[i]);
        Serial.print("_p");
        Serial.print("\n");
        storedButtonStates[i] = PRESSED;
      } else {
        Serial.print(buttonNames[i]);
        Serial.print("_u");
        Serial.print("\n");
        storedButtonStates[i] = UNPRESSED;
      }
    }
  }
  //Serial.print("\n");
  // Read joy states
  for (int i = 0; i < joyPinsSize; ++i) {
    int val = digitalRead(joyPins[i]);

    if (val == 1) {
      joyStates[i] = UNPRESSED;
    } else {
      joyStates[i] = PRESSED;
    }
    /*Serial.print("joy : ");
    Serial.print(joyPins[i]);
    Serial.print("state val: ");
    Serial.print(joyStates[i]);
    Serial.print("   ");*/
    if (joyStates[i] != storedJoyStates[i] && lastJoyStates[i] != storedJoyStates[i] && lastLastJoyStates[i] != storedJoyStates[i]) {
      if (joyStates[i] == PRESSED) {
        Serial.print(joyNames[i]);
        Serial.print("\n");
        storedJoyStates[i] = PRESSED;
      } else {
        //Serial.print(joyNames[i]);
        //Serial.print("\n");
        storedJoyStates[i] = UNPRESSED;
      }
    }
  }

  // Delay a little bit to avoid bouncing
  delay(50);

  //Updating current states
  for (int i = 0; i < buttonPinsSize; ++i) {
    lastButtonStates[i] = buttonStates[i];
  }
  for (int i = 0; i < joyPinsSize; ++i) {
    lastLastJoyStates[i] = lastJoyStates[i];
    lastJoyStates[i] = joyStates[i];
  }
}

void checkIncomingMessage() {
  //Serial.print("checking incoming messages \n");
  if (Serial.available() > 0) {
    //Serial.print("serial has message \n");
    uint8_t receivedChar = Serial.read();
    Serial.print("receivedChar ");
    Serial.print(receivedChar);
    if (receivedChar == 255) {
      Serial.print("!!!!!");
    }
    receivedChar = receivedChar == 255 ? 0 : receivedChar;
    #ifdef DEBUG
      Serial.print("----------");
      Serial.print("ledState is: ");
    #endif
    for (int i = 0; i < ledPinsSize; ++i) {
      bool ledState = (receivedChar >> i) & 1;
      #ifdef DEBUG
        Serial.print(ledState);
        Serial.print(" ");
      #endif
      digitalWrite(ledPins[i], !ledState);
    }
    #ifdef DEBUG
      Serial.print("\n");
    #endif
  }
}
