/*
 * uint8_t last_status                - Статус последней передачи записи I²C. Обратитесь к документации Wire.endTransmission() для получения возвращаемых значений.
 * VL53L0X(void)                      - Конструктор
 * void setAddress(uint8_t new_addr)  - Изменяет адрес ведомого устройства I²C VL53L0X на заданное значение (7-разрядное, 7 бит).
 * uint8_t getAddress(void)           - Возвращает текущий адрес I²C.
 * bool init(bool io_2v8 = true)      - Инициализирует и настраивает датчик. Если необязательный аргумент io_2v8 равен true (по умолчанию, если не указан), датчик настроен на режим 2V8 (вход / выход 2,8 В); если false, датчик остается в режиме 1V8. Возвращаемое значение является логическим значением, указывающим, успешно ли завершена инициализация.
 * void writeReg(uint8_t reg, uint8_t value) -  Записывает 8-битный регистр датчика с заданным значением.
 *                                              Константы адреса регистра определяются типом перечисления regAddr в VL53L0X.h.
 *                                              Пример использования: sensor.writeReg(VL53L0X::SYSRANGE_START, 0x01);
 * void writeReg16Bit(uint8_t reg, uint16_t value) - Записывает 16-битный регистр датчика с заданным значением.
 * void writeReg32Bit(uint8_t reg, uint32_t value) - Записывает 32-битный регистр датчика с заданным значением.
 * uint8_t readReg(uint8_t reg)       - Считывает 8-битный регистр датчика и возвращает прочитанное значение.
 * uint16_t readReg16Bit(uint8_t reg) - Считывает 16-битный регистр датчика и возвращает прочитанное значение.
 * uint32_t readReg32Bit(uint8_t reg) - Считывает 32-битный регистр датчика и возвращает прочитанное значение.
 * void writeMulti(uint8_t reg, uint8_t const * src, uint8_t count) - Записывает произвольное количество байтов из заданного массива в датчик, начиная с заданного регистра.
 * void readMulti(uint8_t reg, uint8_t * dst, uint8_t count)        - Считывает произвольное количество байтов с датчика, начиная с данного регистра, в данный массив.
 * bool setSignalRateLimit(float limit_Mcps)       -  Устанавливает ограничение скорости обратного сигнала до данного значения в единицах MCPS (мегачип в секунду). Это минимальная амплитуда сигнала, отраженного от цели и полученного датчиком, необходимого для того, чтобы он мог сообщить правильное значение. Установка нижнего предела увеличивает потенциальный диапазон датчика, но также увеличивает вероятность получения неточного показания из-за отражений от объектов, отличных от намеченной цели. Этот лимит по умолчанию инициализируется до 0,25 MCPS. Возвращаемое значение является логическим значением, указывающим, был ли запрошенный предел действительным.
 * float getSignalRateLimit(void)     -  Возвращает текущий предел скорости обратного сигнала в MCPS.
 * bool setMeasurementTimingBudget(uint32_t budget_us)  - Устанавливает временной интервал измерения для данного значения в микросекундах. Это время, разрешенное для одного измерения диапазона; более длительный тайминг позволяет проводить более точные измерения. По умолчанию составляет около 33000 микросекунд или 33 мс; минимум 20 мс. Возвращаемое значение является логическим значением, указывающим, было ли запрошенное значение действительным.
 * uint32_t getMeasurementTimingBudget(void)            - Возвращает текущий тайминг измерения в микросекундах.
 * bool setVcselPulsePeriod(vcselPeriodType type, uint8_t period_pclks) - Устанавливает период импульса VCSEL (лазер с поверхностным излучением с вертикальной полостью) для данного типа периода (VL53L0X::cselPeriodPreRange или VL53L0X::VcselPeriodFinalRange) равным данному значению (в PCLK). Более длительные периоды увеличивают потенциальный диапазон датчика. Допустимые значения (только четные числа):
 *                                                                        - Предварительно: от 12 до 18 (по умолчанию инициализируется до 14)
 *                                                                        - Итоговое: от 8 до 14 (по умолчанию инициализируется до 10)
 *                                                                        Возвращаемое значение является логическим значением, указывающим, был ли запрошенный период действительным.
 * uint8_t getVcselPulsePeriod(vcselPeriodType type)    - Возвращает текущий период импульса VCSEL для данного типа периода.
 * void startContinuous(uint32_t period_ms = 0)         - Начинает непрерывное измерение дальности. Если необязательный аргумент period_ms равен 0 (по умолчанию, если не указан), используется непрерывный режим (датчик проводит измерения как можно чаще); если он не нулевой, используется непрерывный синхронизированный режим с указанным периодом между измерениями в миллисекундах, определяющим, как часто датчик выполняет измерение.
 * void stopContinuous(void)                            - Останавливает непрерывный режим.
 * uint16_t readRangeContinuousMillimeters(void)        - Возвращает показание диапазона в миллиметрах, когда активен непрерывный режим.
 * uint16_t readRangeSingleMillimeters(void)            - Выполняет одиночное измерение дальности и возвращает показания в миллиметрах.
 * void setTimeout(uint16_t timeout)                    - Устанавливает период ожидания в миллисекундах, после которого операции чтения будут прерваны, если датчик не готов. Значение 0 отключает тайм-аут.
 * uint16_t getTimeout(void)                            - Возвращает текущую настройку периода ожидания.
 * bool timeoutOccurred(void)                           - Указывает, произошел ли тайм-аут чтения с момента последнего вызова timeoutOccurred().
 */



#include <Wire.h>
#include <VL53L0X.h>
// #include <math.h>

VL53L0X sensor;

uint8_t filterN   = 10;
uint8_t sN (0);
const int samplNumber (20);
float Dist[samplNumber] = {};

float meanDist (0), standDev (0);

void setup() {
  uint32_t timingBudget  = 50000; // /uS
  uint16_t timeout       = 500;  
  
  Serial.begin(9600);
  Wire.begin();

  sensor.setTimeout(timeout);
  Serial.println(sensor.init());
  
  if (!sensor.init())
  {
    Serial.println("Не удалось обнаружить и инициализировать датчик!");
    while (1) {}
  }

  Serial.print("Address:                0x"); Serial.println( sensor.getAddress(), HEX );                       // i2c address
  Serial.print("Back signal vel:        "); Serial.println( sensor.getSignalRateLimit() );                      // Maximal back signal velocity 
  Serial.print("Current timing budget:  "); Serial.println( sensor.getMeasurementTimingBudget() );              // Current timing budget
  Serial.print("Pulse period (pre):     "); Serial.println( sensor.getVcselPulsePeriod( sensor.VcselPeriodPreRange ));    // Pulse period
  Serial.print("Timout set value:       "); Serial.println( sensor.getTimeout() );                              // Current timeout value

  sensor.setVcselPulsePeriod(sensor.VcselPeriodPreRange, 12);   // 12 - 18, default 14
  sensor.setVcselPulsePeriod(sensor.VcselPeriodFinalRange, 8);  // 8 - 14, default 10
  Serial.print("Set pulse period (pre): "); Serial.println( sensor.getVcselPulsePeriod( sensor.VcselPeriodPreRange ));        // Pulse period
  Serial.print("Set pulse period (fin): "); Serial.println( sensor.getVcselPulsePeriod( sensor.VcselPeriodFinalRange ));      // Pulse period
  
  sensor.setMeasurementTimingBudget(timingBudget);
  Serial.print("Set up timing budget:   "); Serial.println( sensor.getMeasurementTimingBudget() );              // Set up timing budget

  
  sensor.startContinuous();
  
  // Запустите непрерывный режим (снимайте показания 
  // как можно быстрее). Чтобы вместо этого использовать непрерывный синхронизированный режим,
  // укажите желаемый период между измерениями в мс 
  // (например, sensor.startContinuous(100)).
}

void loop() {
  unsigned long int measTime_mS(millis());
  float distance_mm(0);

  if ( filterN > 1 ) { 
    for ( uint8_t i = 0; i < filterN; i++ ) {
      distance_mm += sensor.readRangeContinuousMillimeters();    
      }
    distance_mm /= filterN;
  } else {  
    distance_mm = sensor.readRangeContinuousMillimeters();
  }
  
  standDev += distance_mm;        // Sum of samples
  Dist[sN] = distance_mm;         // Array of samples
  sN++;
  if ( sN == samplNumber ) {      // End of samples count
    sN = 0;
    meanDist = standDev / samplNumber;    // Mean value
    standDev = standDeviation( Dist, meanDist );
    }

  Serial.print("Distance: ");  Serial.print(distance_mm);  Serial.print(" mm, mS: "); Serial.print(millis() - measTime_mS); 
  Serial.print("  pro Sec: "); Serial.print(1000 / ((float)millis() - (float)measTime_mS) );
  
  if ( sensor.timeoutOccurred() ) { Serial.print(" Timeout fault"); }
  if ( sN == 0 ) {
    Serial.print(" sDev: "); Serial.print(standDev); 
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
