/*
 * Mit diesem Code kann ein Arduino Leonardo an einer Gercom EBuLa -Tastatur betrieben werden.
 * Die Matrixtastatur wird gemaeß Anschluss-EBuLa-Leonardo.png an den Arduino angeschlossen. 
 * Dieser Code bietet neben der Bedienung der EbuLa-Simulation von Zusi-Display auch die
 * Moeglichkeit Zusi und auch den Monitor zu bedienen.
 * Zur Bedienung von Zusi muss dessen Fenster im Focus von Windows sein. Unmittelbar dahinter
 * muss Zusi-Display befinden.
 * Auf diese Art und Weise kann durch die Uebertragung der Tastenkombination Alt + Tab zwischen
 * den beiden Fenstern umgeschaltet werden.
 * Durch druecken der EBuLa-Taste s wird der Arduino in den EBuLa-Modus geschaltet.
 * Durch druecken der EBuLa-Taste i wird der Arduino in den Zusi-Modus geschaltet.
 * Im Zusi-Modus werden die unteren Tasten 1 bis 0 als F1 bis F10 an Zusi gesendet.
 * Die Pfeiltasten dienen zum Umschalten der Ansichten im Fuehrerstand.
 * 
 * Unter Verwendung von Optokopplern oder Relais kann der Arduino auch an die Bedientasten des
 * verwendeten Monitors angeschlossen werden. Hiermit wird es moeglich Einstellungen im OSD des
 * Monitors vorzunehmen. Dies ist insbesondere fuer die Helligkeitseinstellung praktisch.
 * Fuer die Ansteuerung der Monitortasten sind die Anschluesse A0 bis A5 vorgesehen.
 * Der Code ist Kassenmonitore der Marke TVS vorgesehen. Monitore von Siemens/Wincor/Nixdorf
 * sind in der Regel baugleich. Fuer andere Geraete muesste der Code entsprechend angepasst werden.
 * Nach druecken von St werdend die Tasten auf der rechten Seute an den Monitor geleitet.
 * Druecken der Helligkeitstaste navigiert zusaetzlich direkt ins Helligkeitsmenue des Monitor OSD.
 * Wer die Funktion nutzen moechte, muss die '//' in Zeile 33 entfernen.
 */

#include <Keyboard.h>
#include <Keypad.h>
#include <Mouse.h>
byte mode = 0;
#define mode_zusi 0
#define mode_zusiDisplay  1
#define mode_monitorOsd  2
//#define MonitorControl
const byte ROWS = 5; // Fuenf Zeilen
const byte COLS = 5; // Fuenf Spalten
// Define the Keymap
char keys[ROWS][COLS] = {
  {'a','s','i','t','w'},
  {'v','h','k','x','c'},
  {'l','r','u','d','e'},
  {'0','9','8','7','6'},
  {'5','4','3','2','1'}
};

byte rowPins[ROWS] = { 3, 5, 7, 9, 11 };  // Zeilen an diese Pins anschließen:
byte colPins[COLS] = { 4, 6, 8, 10, 12 }; // Spalten an diese Pins anschließen:

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );  // Keypad erzeugen
#define ledpin 13    // LED wird an Pin 13 angeschlossen
void setup()
{ 
#ifdef MonitorControl            // Wenn Monitorbedientatsten verwendet werden...
  pinMode(A0, OUTPUT);            // ...Ausgangspins festlegen
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);ha
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  digitalWrite(A0, LOW);          // Ist die Masse fuer die Optokoppler
#endif
  pinMode(ledpin,OUTPUT);
  digitalWrite(ledpin, LOW);
  Serial.begin(9600);
  Keyboard.begin();
  Mouse.begin();
}

void loop()
{
  delay(100);
  char key = kpd.getKey();
  if (key != NO_KEY){
    Serial.println(key);
    switch (key)
    {
      case 's':       // Taste s wird nicht von Zusi-Display benutzt. Sie schaltet hier in den Zusi-Display-Modus
        mode = mode_zusiDisplay;
        break;
      case 'i':       // Taste i wird nicht von Zusi-Display benutzt. Sie schaltet hier in den Zusi-Modus
        mode = mode_zusi;
        break;
#ifdef MonitorControl
      case 't':       // Taste St wird nicht von Zusi-Display benutzt. Hier wird sie benutzt, um in den Monitor OSD-Modus zu wechseln.
        mode = mode_monitorOsd;
        pushMonitorBtn('c');  // Nach wechsel in den OSD-Modus, das Menue direkt oeffnen.
        break;
      case 'h':       // Helligkeitstaste wird zwar von Zusi-Display benutzt...
                      // ...aber es soll das Helligkeitsmenue des Monitors verwendet werden.
        mode = mode_monitorOsd;
        pushMonitorBtn('c');
        delay(500);
        pushMonitorBtn('e');
        break;
#endif
      case 'w':       // Taste V>0 wird nicht von Zusi-Display benutzt.
      case 'v':       // Taste St wird nicht von Zusi-Display benutzt.
        break;
      default:
        if(mode == mode_zusiDisplay)  sendKey(key);
        if(mode == mode_zusi)         sendZusiKey(key);
#ifdef MonitorControl
        if(mode == mode_monitorOsd)   pushMonitorBtn(key);
#endif
    }
  }
}
void sendKey(char key){
  digitalWrite(ledpin, HIGH);
  sendAltTab(100);
  pressAndRelease(key);
  sendAltTab(0);
  digitalWrite(ledpin, LOW);
}
void sendZusiKey(char key){
  digitalWrite(ledpin, HIGH);
    switch (key)
    {
      case '1':
        pressAndRelease(KEY_F1);  // Zeitraffer
        break;
      case '2':
        pressAndRelease(KEY_F2);  // Pause
        break;
      case '3':
        pressAndRelease(KEY_F3);  // Autopilot
        break;
      case '4':
        pressAndRelease(KEY_F4);  // Zeitsprung
        break;
      case '5':
        pressAndRelease(KEY_F5);  // Fuehrerstand ein-/ausblenden
        break;
      case '6':
        pressAndRelease(KEY_F6);  // Fuehrerstand-Zusatzinfo anzeigen
        break;
      case '7':
        pressAndRelease(KEY_F7);  // Zugdaten einblenden
        break;
      case '8':
        pressAndRelease(KEY_F8);  // Schummelinfo anzeigen
        break;
      case '9':
        pressAndRelease(KEY_F9);  // Standort Fuehrerstand
        break;
      case '0':
        pressAndRelease(KEY_F12); // Standort Zug-Aussenansicht hinten
        Mouse.press();            // Damit sich der Blick um das Zugende dreht...
        Mouse.move(50, 0, 0);     // ...muss ein Mausklick mit Bewegung gemacht werden.
        Mouse.release();
        Mouse.move(-50, 0, 0);
        break;
      case 'u':
        pressAndRelease(KEY_UP_ARROW);    // Pfleil hoch
        break;
      case 'd':
        pressAndRelease(KEY_DOWN_ARROW);  // Pfleil runter
        break;
      case 'l':
        pressAndRelease(KEY_LEFT_ARROW);  // Pfleil links
        break;
      case 'r':
        pressAndRelease(KEY_RIGHT_ARROW); // Pfleil rechts
        break;
      default:
        sendKey(key);
    }
  digitalWrite(ledpin, LOW);
}
void pressAndRelease(char key){
  Keyboard.press(key);
  delay(200);
  Keyboard.releaseAll();
}
void sendAltTab(byte waitMs){
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_TAB);
  Keyboard.releaseAll();
  delay(waitMs);
}
#ifdef MonitorControl
void pushMonitorBtn(char key){
  switch (key){
    case 'a':                 // Taste An / Aus
      digitalWrite(A1, HIGH);
      delay(65);
      digitalWrite(A1, LOW);
      break;
    case 'e':                 // Taste Enter
      digitalWrite(A2, HIGH);
      delay(65);
      digitalWrite(A2, LOW);
      break;
    case 'c':                 // Taste Menue
      digitalWrite(A3, HIGH);
      delay(65);
      digitalWrite(A3, LOW);
      break;
    case 'd':                 // Taste Hoch
      digitalWrite(A4, HIGH);
      delay(65);
      digitalWrite(A4, LOW);
      break;
    case 'u':                 // Taste Runter
      digitalWrite(A5, HIGH);
      delay(65);
      digitalWrite(A5, LOW);
      break;
    case 'l':                 // Taste Hoch 10 mal druecken
      for(byte n = 0; n<10; n++){
        digitalWrite(A4, HIGH);
        delay(65);
        digitalWrite(A4, LOW);
        delay(65);
      }
      break;
    case 'r':                 // Taste Runter 10 mal druecken
      for(byte n = 0; n<10; n++){
        digitalWrite(A5, HIGH);
        delay(65);
        digitalWrite(A5, LOW);
        delay(65);
      }
  }
  delay(25);
}
#endif
