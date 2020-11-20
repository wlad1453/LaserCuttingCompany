/*
 * 1. The laser is switched off. The valve is switched off.  
 * 2. The laser is switched on -> the valve is switched on with no delay.
 * 3. 
 * 
 * laserPin - input of laser ON signal
 * logicPin - switch from negative to positive logic (laser ON/OFF)
 * valvePin - switch ON air valve
 * valveHoldPin - switch ON the ballast holding resistor
 * del10Pin - delay 10 sec switch
 * del15Pin - delay 15 sec switch
 */


#define laserPin 7  
#define logicPin 8
#define valvePin 9
#define valveHoldPin 6

bool laserOn = false, valveOn = false;
bool switchingOff = false;                    // Switching off procedure going on
bool positiveLogic = true;                    // Switch on if the laser signal is high
// bool plus10 = false, plus15 = false;

int switchOffDelay = 5000;
int blinkDuration  = 25;
unsigned long stopWatchStart = 0;
unsigned long blinkStart = 0;


void setup() {  
  pinMode(laserPin, INPUT);
  pinMode(logicPin, INPUT);
//  pinMode(del10Pin, INPUT);
//  pinMode(del15Pin, INPUT);
  pinMode(valvePin, OUTPUT);
  pinMode(valveHoldPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT); // Pin 13

  positiveLogic = digitalRead(logicPin);

  digitalWrite  ( valvePin, LOW );  
  delay(1000);
}

void loop() {
  /* plus10 = !digitalRead(del10Pin);  
  plus15 = !digitalRead(del15Pin);  
  if (plus10 && plus15) switchOffDelay = 30000;
  else if (plus10) switchOffDelay = 15000;
  else if (plus15) switchOffDelay = 20000;
  else switchOffDelay = 5000; */
  
  laserOn = digitalRead(laserPin);                                          // reading the status of the laser
  if( !positiveLogic ) laserOn = !laserOn; 

  if ( laserOn && !valveOn ) {                                              // if the laser is going on but the air valve isn't
    valveOn = true;                                                         // switched on. It should be done at any time.
    digitalWrite  ( valvePin, HIGH );                                       // ??????? if the laser goes on during the switching off process
    digitalWrite  ( valveHoldPin, HIGH );                                   // *********** NEW
  }

  if ( !switchingOff && !laserOn && valveOn ) {                             // In case the laser goes off the stopwatch started and
    stopWatchStart = millis();                                              // the switching off      process begins.
    switchingOff = true;
  }
  
if( switchingOff && laserOn ) {
    switchingOff = false;
  }  

  if ( switchingOff && ( millis() - stopWatchStart ) > switchOffDelay ) {   // If the stopwatch is over the air valve switches off
    valveOn = false;                                                        // and the switching process ends
    digitalWrite( valvePin, LOW );
    digitalWrite( valveHoldPin, LOW );                                      // ************************************ NEW
    switchingOff = false;
  }

if ( !(millis() % 1000) ) {
  blinkStart = millis();
  digitalWrite( LED_BUILTIN, HIGH );
}

if ( (millis() - blinkStart) > blinkDuration) digitalWrite( LED_BUILTIN, LOW );
  
}
