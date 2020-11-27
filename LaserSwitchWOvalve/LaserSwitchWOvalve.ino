    /*
 * 1. The laser is switched off. The valve is switched off.  
 * 2. The laser is switched on -> the valve is switched on with no delay.
 * 3. 
 * 
 * laserPin - input of laser ON signal
 * logicPin - switch from negative to positive logic (laser ON/OFF)
 * valvePin - switch ON air valve
 * valveHoldPin - switch ON the ballast holding resistor
 */  


#define laserPin 7  
#define logicPin 8
#define valvePin 9
#define valveHoldPin 6
#define relePin 5
#define ledPin 4


bool laserOn = false, valveOn = false;
bool switchingOff = false;                    // Switching off procedure going on 
bool positiveLogic = true;                    // Switch on if the laser signal is high

int switchOffDelay = 5000;
int blinkDuration  = 25;
unsigned long stopWatchStart = 0;
unsigned long blinkStart = 0;


void setup() {  
  pinMode(laserPin, INPUT);
  pinMode(logicPin, INPUT);    
  pinMode(valvePin, OUTPUT);   
  pinMode(valveHoldPin, OUTPUT);   
  pinMode(relePin, OUTPUT);   
  pinMode(ledPin, OUTPUT);  
   
  digitalWrite  ( valvePin, LOW );  
  digitalWrite  ( valveHoldPin, LOW );  
  digitalWrite  ( relePin, HIGH );            // Rele is switched off (high signal level)
  digitalWrite  ( ledPin, HIGH );
  delay(1000);
}

void loop() {
  // *** Logic test, positive vs. negative imput signal ***
  laserOn = digitalRead(laserPin);                                          // laser status reading
  if( !positiveLogic ) laserOn = !laserOn; 

  // *** Air valve and copressor switching ON *** 
  if ( laserOn && !valveOn ) {                                              // if the laser is going on but the air valve isn't
                                                                            // switched on. It should be done at any time.
    Serial.print("laserOn&&!valveOn 0: valveHoldPin: "); Serial.print(digitalRead(valveHoldPin)); Serial.print(" valvePin: "); Serial.println(digitalRead(valvePin)); 
    digitalWrite  ( valveHoldPin, HIGH ); 
    digitalWrite  ( relePin, LOW );                 // Compressor rele is switched ON
    digitalWrite  ( ledPin, LOW );                  // LED is switched off
    Serial.print("laserOn&&!valveOn 0: valveHoldPin: "); Serial.print(digitalRead(valveHoldPin)); Serial.print(" valvePin: "); Serial.println(digitalRead(valvePin)); 
    delay(50);                                                              // 50 mS delay, first a low current is set, then the big one                                                    
    digitalWrite  ( valvePin, HIGH );                                       // ??????? if the laser goes on during the switching off process    
    Serial.print("laserOn&&!valveOn 0: valveHoldPin: "); Serial.print(digitalRead(valveHoldPin)); Serial.print(" valvePin: "); Serial.println(digitalRead(valvePin)); 
    delay(500);                                                             // The delay which let the valve to go open
    digitalWrite  ( valvePin, LOW );                                        // The only holding pin stayes on
    valveOn = true; 
    Serial.print("laserOn&&!valveOn 0: valveHoldPin: "); Serial.print(digitalRead(valveHoldPin)); Serial.print(" valvePin: "); Serial.println(digitalRead(valvePin));     
  }

  Serial.print("loop1: valveHoldPin: "); Serial.print(digitalRead(valveHoldPin)); Serial.print(" valvePin: "); Serial.println(digitalRead(valvePin)); 
  // *** End switching ON ***

  // *** Switching OFF process begins ***
  if ( !switchingOff && valveOn && !laserOn ) {                             // In case the laser goes off during the air valve is on 
    stopWatchStart = millis();                                              // the stopwatch started and
    switchingOff = true;                                                    // the switching off process begins.
  }
  
  if( switchingOff && laserOn ) {                                           // In case if during the switching off process the laser went on again
      switchingOff = false;
  }  

  // *** Switching OFF ***
  if ( switchingOff && ( millis() - stopWatchStart ) > switchOffDelay ) {   // If the stopwatch is over the air valve switches off
    valveOn = false;                                                        // and the switching process ends
    digitalWrite ( valveHoldPin, LOW );                                      // ************************************ NEW
    digitalWrite ( relePin, HIGH ); 
    digitalWrite  ( ledPin, HIGH );
    switchingOff = false;
  }
  // *** The valve and rele are switched OFF

  // *** Blinking with the builtin LED ***
  Serial.print("loop 2: valveHoldPin: "); Serial.print(digitalRead(valveHoldPin)); Serial.print(" valvePin: "); Serial.println(digitalRead(valvePin)); 
  if ( !(millis() % 1000) ) {
    blinkStart = millis();
    digitalWrite( LED_BUILTIN, HIGH );
  }  
  if ( (millis() - blinkStart) > blinkDuration) digitalWrite( LED_BUILTIN, LOW );
  // *** End blinking ***
  
    
}
