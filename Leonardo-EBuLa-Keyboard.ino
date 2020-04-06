#include <Keyboard.h>
#include <Keypad.h>
bool toggleWindowBeforeKey = false;
const byte ROWS = 5; // Five rows
const byte COLS = 5; // Five columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'a','s','i','t','w'},
  {'v','h','k','x','c'},
  {'l','r','u','d','e'},
  {'0','9','8','7','6'},
  {'5','4','3','2','1'}
};


// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 3, 5, 7, 9, 11 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 4, 6, 8, 10, 12 };

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
#define ledpin 13
void setup()
{
  pinMode(ledpin,OUTPUT);
  digitalWrite(ledpin, LOW);
  Serial.begin(9600);
  Keyboard.begin();
}

void loop()
{
  delay(100);
  char key = kpd.getKey();
  if (key != NO_KEY){
    Serial.println(key);
    switch (key)
    {
      case 's':       // Key S not used by Zusi-Display. Lets use it for Atl+Tab
        sendAltTab(0);
        break;
      case 'i':       // Key i not used by Zusi-Display. Lets use it to turn
                      // on and off the window toggle mode.
        switchWindowToggle();
        break;
      case 't':       // Key St not used by Zusi-Display. Lets use it fast forward in zusi
        pressAndRelease(KEY_F1);
        break;
      case 'w':       // Key V>0 not used by Zusi-Display. Lets use it for a break in zusi
        pressAndRelease(KEY_F2);
        break;
      case 'v':       // Key St not used by Zusi-Display. Lets use it for time jump in zusi
        pressAndRelease(KEY_F4);
        break;
      default:
        sendKey(key);
    }
  }
}
void sendKey(char key){
  digitalWrite(ledpin, HIGH);
  if(toggleWindowBeforeKey)sendAltTab(100);
  pressAndRelease(key);
  if(toggleWindowBeforeKey)sendAltTab(0);
  digitalWrite(ledpin, LOW);
}
void pressAndRelease(char key){
  Keyboard.press(key);
  delay(200);
  Keyboard.releaseAll();
}
void switchWindowToggle(){
  digitalWrite(ledpin, HIGH);
  delay(50);
  digitalWrite(ledpin, LOW);
  delay(100);
  toggleWindowBeforeKey = !toggleWindowBeforeKey;
  if(toggleWindowBeforeKey){
    digitalWrite(ledpin, HIGH);
    delay(50);
    digitalWrite(ledpin, LOW);
  }
}

void sendAltTab(byte waitMs){
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_TAB);
  Keyboard.releaseAll();
  delay(waitMs);
}
