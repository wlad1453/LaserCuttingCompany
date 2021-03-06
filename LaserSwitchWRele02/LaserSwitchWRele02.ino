/*
 * 1. The laser is switched off. The valve is switched off.  
 * 2. The laser is switched on -> the valve is switched on immediately.
 * 3. The "Wind" signal become low -> switching OFF procedure started
 * 4. After a givem delay time the air valve and the compressor rele go OFF
 * 5. Every 1000 mS a short blinking shows that the /uC is working
 * 
 * laserPin - input of laser ON signal ("Wind" - negative logic)
 * logicPin - switch from negative to positive logic if necessary (laser ON/OFF)
 * valvePin - switch ON air valve
 * valveHoldPin - switch ON the ballast holding resistor
 * relePin - Compressor rele pin (negative logic)
 */

#define laserPin 7  
#define logicPin 8
#define valvePin 9
#define valveHoldPin 6
#define relePin 10

bool laserOn = false, valveOn = false;
bool switchingOff = false;                    // Switching off procedure going on
bool positiveLogic = true;                    // Switch on if the laser signal is high

int switchOffDelay = 5000;                    // Delay 5 Sec between LaserOn signal became LOW and switching OFF of the air valve and compressor rele
int blinkDuration  = 20;
int blinkPeriod  = 1000;
unsigned long stopWatchStart = 0;
unsigned long blinkStart = 0;
unsigned long blinkPeriodStart = 0;

void setup() {  
  pinMode(laserPin, INPUT);
  pinMode(logicPin, INPUT);
  pinMode(valvePin, OUTPUT);
  pinMode(valveHoldPin, OUTPUT);
  pinMode(relePin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT); // Pin 13

  positiveLogic = digitalRead(logicPin);      // Check the logic of the imput signal ("Wind" on Laser Machine)
  
  Serial.begin(9600); 
  
  Serial.print(" *** Logic: "); Serial.println(digitalRead(positiveLogic)); 

  digitalWrite  ( valvePin, LOW );            // Both valve control signals are low
  digitalWrite  ( valveHoldPin, LOW );  
  digitalWrite  ( relePin, HIGH );            // Rele is switched off (high signal level)
  delay(1000);
}

void loop() {
  // *** Logic test, positive vs. negative imput signal ***
  laserOn = digitalRead(laserPin);                                          // laser status reading (Wind)
  if( !positiveLogic ) laserOn = !laserOn; 

  // *** Air valve and copressor switching ON *** 
  if ( laserOn && !valveOn ) {                                              // if the laser is going on but the air valve isn't
                                                                            // switched on. It should be done at any time.
    Serial.print("laserOn&&!valveOn 0: valveHoldPin: "); Serial.print(digitalRead(valveHoldPin)); Serial.print(" valvePin: "); Serial.println(digitalRead(valvePin)); 
    digitalWrite  ( valveHoldPin, HIGH ); 
    digitalWrite  ( relePin, LOW );                 // Compressor rele is switched ON
    Serial.print("laserOn&&!valveOn 0: valveHoldPin: "); Serial.print(digitalRead(valveHoldPin)); Serial.print(" valvePin: "); Serial.println(digitalRead(valvePin)); 
    delay(50);                                                              // 50 mS delay, first a low current is set, then the bigger one                                                    
    digitalWrite  ( valvePin, HIGH );                                       // The main current value (??? if the laser goes on during the switching off process?)    
    Serial.print("laserOn&&!valveOn 0: valveHoldPin: "); Serial.print(digitalRead(valveHoldPin)); Serial.print(" valvePin: "); Serial.println(digitalRead(valvePin)); 
    delay(500);                                                             // This delay let the valve to become open
    digitalWrite  ( valvePin, LOW );                                        // The only holding pin stayes on (only low level holding current flows through the valve)
    valveOn = true; 
    Serial.print("laserOn&&!valveOn 0: valveHoldPin: "); Serial.print(digitalRead(valveHoldPin)); Serial.print(" valvePin: "); Serial.println(digitalRead(valvePin));     
  }

  Serial.print("loop1: valveHoldPin: "); Serial.print(digitalRead(valveHoldPin)); Serial.print(" valvePin: "); Serial.println(digitalRead(valvePin)); 
  // *** End switching ON ***

  // *** Switching OFF procedure begins ***
  if ( !switchingOff && valveOn && !laserOn ) {                             // In case the laser goes off during the air valve is on 
    stopWatchStart = millis();                                              // the stopwatch started and
    switchingOff = true;                                                    // the switching off process begins.
  }
  
  if( switchingOff && laserOn ) {                                           // In case if during the switching off procedure the laser went on again
      switchingOff = false;
  }  

  // *** Switching OFF ***
  if ( switchingOff && ( millis() - stopWatchStart ) > switchOffDelay ) {   // If the stopwatch is over the air valve switches off
    valveOn = false;                                                        // and the switching process ends
    digitalWrite ( valveHoldPin, LOW );                                      // ************************************ NEW
    digitalWrite ( relePin, HIGH );
    switchingOff = false;
  } 
  Serial.print("loop2: valveHoldPin: "); Serial.print(digitalRead(valveHoldPin)); Serial.print(" valvePin: "); Serial.println(digitalRead(valvePin)); 
  // *** The valve and rele are switched OFF

  // *** Blinking with the builtin LED ***
if ( (millis() - blinkPeriodStart) > blinkPeriod) {
    blinkStart = millis();
    blinkPeriodStart = millis();
    digitalWrite( LED_BUILTIN, HIGH );
  }  
  if ( (millis() - blinkStart) > blinkDuration) {    
    digitalWrite( LED_BUILTIN, LOW );
  }  
  // *** End blinking ***    
}
