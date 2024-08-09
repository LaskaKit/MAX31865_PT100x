// LaskaKIT MAX31865

#include <Adafruit_MAX31865.h>

//#define LaskaKitESP32DEV
//#define LaskaKitESP32S3
#define LaskaKitESPink426

#ifdef LaskaKitESP32DEV
  #define MOSI  13
  #define MISO  12
  #define SCK   14
  #define CS    15
  #define POWER 2
#elif defined LaskaKitESP32S3
  #define MOSI  11
  #define MISO  13
  #define SCK   12
  #define CS    10
  #define POWER 47
#elif defined LaskaKitESPink426
  #define MOSI  37
  #define MISO  21
  #define SCK   14
  #define CS    46
  #define POWER 47
#endif

Adafruit_MAX31865 thermo = Adafruit_MAX31865(CS, MOSI, MISO, SCK);

#define PT1000  // select sensor type: PT100 or PT1000

#if defined(PT1000)
  #define RREF      4300.0
  #define RNOMINAL  1000.0
#elif defined(PT100)
  #define RREF      430.0
  #define RNOMINAL  100.0
#endif

void setup() {
  Serial.begin(115200);
  pinMode(POWER, OUTPUT); 
  digitalWrite(POWER, HIGH); // enable power supply for ePaper and uSup
  delay(500);

  Serial.println("MAX31865 PT100/PT1000 Sensor Test!");

  thermo.begin(MAX31865_2WIRE);  // set to 2WIRE, 3WIRE or 4WIRE as necessary
}


void loop() {
  uint16_t rtd = thermo.readRTD();

  Serial.print("RTD value: "); Serial.println(rtd);
  float ratio = rtd;
  ratio /= 32768;
  Serial.print("Ratio = "); Serial.println(ratio,8);
  Serial.print("Resistance = "); Serial.println(RREF*ratio,8);
  Serial.print("Temperature = "); Serial.println(thermo.temperature(RNOMINAL, RREF));

  // Check and print any faults
  uint8_t fault = thermo.readFault();
  if (fault) {
    Serial.println("Mas spravne zapojeny senzor? Mas spravne zvoleny typ senzoru - PT100/PT100? Nebo spatne nastavene propojky?");
    Serial.print("Fault 0x"); Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold"); 
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold"); 
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias"); 
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open - "); 
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage"); 
    }
    thermo.clearFault();
  }
  Serial.println();
  delay(1000);
}
