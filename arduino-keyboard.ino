#include <Keypad.h>
#include <Keyboard.h>
#include <InputDebounce.h>
#define KEY_CMD    0x37
#define KEY_LEFT_GUI  0x83
#define KEY_RIGHT_SHIFT    0x85
#define KEY_RIGHT_ALT  0x86
#define KEY_RIGHT_GUI  0x87

#define KEY_UP_ARROW   0xDA
#define KEY_DOWN_ARROW 0xD9
#define KEY_LEFT_ARROW 0xD8
#define KEY_RIGHT_ARROW    0xD7
#define KEY_BACKSPACE  0xB2
#define KEY_TAB        0xB3
#define KEY_RETURN 0xB0
#define KEY_ESC        0xB1
#define KEY_INSERT 0xD1
#define KEY_DELETE 0xD4
#define KEY_PAGE_UP    0xD3
#define KEY_PAGE_DOWN  0xD6
#define KEY_HOME   0xD2
#define KEY_END        0xD5
#define KEY_CAPS_LOCK  0xC1
#define KEY_F1     0xC2
#define KEY_F2     0xC3
#define KEY_F3     0xC4
#define KEY_F4     0xC5
#define KEY_F5     0xC6
#define KEY_F6     0xC7
#define KEY_F7     0xC8
#define KEY_F8     0xC9
#define KEY_F9     0xCA
#define KEY_F10        0xCB
#define KEY_F11        0xCC
#define KEY_F12        0xCD
#define KEY_LEFT_CTRL  0x80

const int LEFT_BLUE_LED_PIN = 7;
const int RIGHT_BLUE_LED_PIN = 10;
int ledStateLeftBlue = LOW;
int ledStateRightBlue = LOW;

#define DEBOUNCE_DELAY 50 // ms

int buttonPin = 9;  
int buttonPin1 = 10;  
int buttonPin2 = 8;  
int buttonPin3 = 6; 
int buttonPin4 = 5;  
const int btnCount = 2;
const int pinBtn[17] = {
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0, 1}; // map pins to buttons
const int btnPin[btnCount] = {9, 16};  // map buttons to pins

static InputDebounce buttons[btnCount];

void btn_pressedCallback(uint8_t pinIn)
{
  // handle pressed state
  digitalWrite(LED_BUILTIN, HIGH); // turn the LED on
#ifdef DEBUG
  Serial.print("BUTTON (pin: ");
  Serial.print(pinIn);
  Serial.println(")");
#endif
}

// handle released state
void btn_releasedCallback(uint8_t pinIn)
{
  digitalWrite(LED_BUILTIN, LOW); // turn the LED off

  const int btnIndex = pinBtn[pinIn];
  // TODO: code for each sequence
  switch (btnIndex) {
    case 0:
      // toggle the mute light
      ledStateLeftBlue = !ledStateLeftBlue; 
      digitalWrite(LEFT_BLUE_LED_PIN, ledStateLeftBlue);
      break;
    case 1:
      // toggle the rec light
      ledStateRightBlue = !ledStateRightBlue; 
      digitalWrite(RIGHT_BLUE_LED_PIN, ledStateRightBlue);
      break;
    default:
      break;
  }
}

void btn_pressedDurationCallback(uint8_t pinIn, unsigned long duration)
{
  // handle still pressed state
  // Serial.print("SEQ HIGH (pin: ");
  // Serial.print(pinIn);
  // Serial.print(") still pressed, duration ");
  // Serial.print(duration);
  // Serial.println("ms");
  // writeBtnPitch(pinBtn[pinIn] - ctrlBtnCount);
}

void btn_releasedDurationCallback(uint8_t pinIn, unsigned long duration)
{
  // handle released state
  // Serial.print("SEQ LOW (pin: ");
  // Serial.print(pinIn);
  // Serial.print("), duration ");
  // Serial.print(duration);
  // Serial.println("ms");
}

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

// Zoom Command(⌘)+Shift hotkeys for Mac:
// Command(⌘)+Shift+A: Mute/unmute audio
// Command(⌘)+Shift+V: Start/stop video
// Command(⌘)+Shift+N: Switch camera
// Command(⌘)+Shift+S: Start/stop screen share
// Command(⌘)+Shift+T: Pause or resume screen share
// Command(⌘)+Shift+R: Start local recording
// Command(⌘)+Shift+C: Start cloud recording
// Command(⌘)+Shift+P: Pause or resume recording
// Command(⌘)+Shift+W: Switch between speaker view and gallery view
// Command(⌘)+Shift+H: Show/hide in-meeting chat panel
// Command(⌘)+Shift+I: Copy invite link
// Command(⌘)+Shift+F: Enter or exit full screen
// Command(⌘)+Shift+M: Switch to minimal window
// Command(⌘)+Shift+D: Enable/disable dual monitor mode.
char keys[ROWS][COLS] = {
  {'A','V','S','T'},
  {'D','F','M','W'},
  {'C','R','P','I'},
  {'H','5','3','4'}
};
byte rowPins[ROWS] = {18, 19, 20, 21}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 3, 4, 5}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
  pinMode(LEFT_BLUE_LED_PIN, OUTPUT); // LED
  digitalWrite(LEFT_BLUE_LED_PIN, ledStateLeftBlue);
  pinMode(RIGHT_BLUE_LED_PIN, OUTPUT); // LED
  digitalWrite(RIGHT_BLUE_LED_PIN, ledStateRightBlue);

  for (int i = 0; i < btnCount; i++)
  {
    // register callback functions (shared, used by all non-matrix buttons)
    buttons[i].registerCallbacks(btn_pressedCallback, btn_releasedCallback, btn_pressedDurationCallback, btn_releasedDurationCallback);

    // setup input buttons (debounced)
    buttons[i].setup(btnPin[i], DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
  }

  Keyboard.begin();

}

void loop(){
  char key = keypad.getKey();

  if (key != NO_KEY){
    Serial.println(key);
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_RIGHT_SHIFT);
    Keyboard.print(key);
    Keyboard.release(KEY_RIGHT_SHIFT);
    Keyboard.release(KEY_LEFT_GUI);
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on
    if (key == 'A') {
      // toggle the mute light
      ledStateLeftBlue = !ledStateLeftBlue; 
      digitalWrite(LEFT_BLUE_LED_PIN, ledStateLeftBlue);
    } else if (key == 'R' || key == 'C') {
      // toggle the recording light
      // TODO: handle when i'm local/cloud recording and hit the cloud/local (opposite) recording key (probably ignored by zoom; check)
      ledStateRightBlue = !ledStateRightBlue; 
      digitalWrite(RIGHT_BLUE_LED_PIN, ledStateRightBlue);
    }
  } else {
    digitalWrite(LED_BUILTIN, LOW); // turn the LED off
  }

  unsigned long now = millis();
  for (int i = 0; i < btnCount; i++)
  {
    buttons[i].process(now);
  }  
}
