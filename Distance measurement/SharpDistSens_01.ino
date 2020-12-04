/*
 * Sharp GP2Y0A21YK0F distance sensor
 * 
 * A0 - analog signal pin
 * 
 * Formula 1.
 * d = 5461 / (ADC - 17) - 2, cm
 * http://home.roboticlab.eu/ru/examples/sensor/ir_distance 
 * 
 * Formula 2.
 * https://robotclass.ru/tutorials/arduino-distance-optical-sensor/
 * 
 * Formula 3.
 * value1 = analogRead(IRpin);
 * float volts = analogRead(IRpin)*0.0048828125;  // Convert the analog value into the voltage
 * int distance=32*pow(volts,-1.10);              // then the voltage into the distance in cm * 
 * https://3d-diy.ru/wiki/arduino-datchiki/infrakrasnyj-datchik-rasstojanija/
 * 
 */

// #include <math.h>

uint8_t filterN   = 5;
uint8_t sN (0);
const int samplNumber (20);
float Dist[samplNumber] = {};

float meanDist (0), standDev (0), mDev (0);

void setup() {
 
  
  Serial.begin(38400);
 
 
}

void loop() {
  unsigned long int measTime_mS(millis());
  float distance_mm(0);

  if ( filterN > 1 ) { 
    for ( uint8_t i = 0; i < filterN; i++ ) {
      distance_mm += 54610 / (analogRead(A0) - 17) - 2;
      delay(50);  
      }
    distance_mm /= filterN;
  } else {  
    distance_mm = 54610 / (analogRead(A0) - 17) - 2;
  }
  delay(50);  
  
  standDev += distance_mm;        // Sum of samples
  Dist[sN] = distance_mm;         // Array of samples
  sN++;
  if ( sN == samplNumber ) {      // End of samples count
    sN = 0;
    meanDist = standDev / samplNumber;    // Mean value
    standDev = standDeviation( Dist, meanDist );
    mDev = Deviation( Dist, meanDist );
    }

  Serial.print("Distance:\t");  Serial.print(distance_mm);  Serial.print(" mm\t"); /*Serial.print(millis() - measTime_mS); 
  Serial.print("  pro Sec: ");*/ Serial.print(1000 / ((float)millis() - (float)measTime_mS) ); Serial.print(" 1/S");
 
  if ( sN == 0 ) {
    Serial.print("\tsDev: "); Serial.print(standDev); Serial.print("\tmDev: "); Serial.print(mDev); 
    standDev = 0;
    }

  Serial.println();

}

float standDeviation(float D[samplNumber], float mean ) {
  float Dev[samplNumber] = {};                
  float sDev(0);                                  // Satndart deviation
  for ( uint8_t i = 0; i < samplNumber; i++ ) {
    Dev[i] = D[i] - mean;                         // Deviation from the mean
    Dev[i] *= Dev[i];                             // Squer of the deviation
    sDev += Dev[i];                               // Sum of squers
    }
    return sqrt( sDev /= samplNumber );           // Squer root from the sum
  }

float Deviation(float D[samplNumber], float mean ) {
  float Dev[samplNumber] = {};                
  float mDev(0);                                   // Satndart deviation
  for ( uint8_t i = 0; i < samplNumber; i++ ) {
    Dev[i] = D[i] - mean;                         // Deviation from the mean
    if ( Dev[i] < 0 ) Dev[i] = - Dev[i];
    mDev += Dev[i];                                // Sum of positive deviation values
    }
    return mDev /= samplNumber;                    // mean Deviation
  }
