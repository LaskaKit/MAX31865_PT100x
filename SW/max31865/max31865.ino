// LaskaKit MAX31865 converter for PT100 / PT1000
// https://github.com/LaskaKit/MAX31865_PT100x
//
// Library: Adafruit MAX31865 (https://github.com/adafruit/Adafruit_MAX31865)
//
// The module ships configured for PT100 and a 4-wire sensor.
// The settings below must match the jumpers on the board:
//   SENSOR - JP1 + JP3 (PT100 / PT1000)
//   WIRES  - JP2, JP4, JP5 (2 / 3 / 4-wire)
// See README for the jumper table (v2.0 and v1.x).
//
// Power and logic: 3.3 V only!

#include <Adafruit_MAX31865.h>

// ---------------- Board selection ----------------
#define LaskaKitESP32DEV
//#define LaskaKitESP32S3
//#define LaskaKitESPink426

// ---------------- Sensor type (must match JP1 + JP3) ----------------
#define SENSOR_PT100
//#define SENSOR_PT1000

// ---------------- Number of wires (must match JP2, JP4, JP5) ----------------
#define SENSOR_WIRES  MAX31865_4WIRE   // MAX31865_2WIRE, MAX31865_3WIRE or MAX31865_4WIRE

// ---------------- Pins ----------------
#if defined(LaskaKitESP32DEV)
  #define PIN_MOSI   13
  #define PIN_MISO   12
  #define PIN_SCK    14
  #define PIN_CS     15
  #define PIN_POWER  2    // uSup power enable
#elif defined(LaskaKitESP32S3)
  #define PIN_MOSI   11
  #define PIN_MISO   13
  #define PIN_SCK    12
  #define PIN_CS     10
  #define PIN_POWER  47   // uSup power enable
#elif defined(LaskaKitESPink426)
  #define PIN_MOSI   37
  #define PIN_MISO   21
  #define PIN_SCK    14
  #define PIN_CS     46
  #define PIN_POWER  47   // ePaper and uSup power enable
#else
  #error "Select a board"
#endif

// ---------------- Reference resistor (R3 = 430R for PT100, R4 = 4k3 for PT1000) ----------------
#if defined(SENSOR_PT100) && defined(SENSOR_PT1000)
  #error "Select only one sensor type"
#elif defined(SENSOR_PT100)
  #define RREF      430.0
  #define RNOMINAL  100.0
#elif defined(SENSOR_PT1000)
  #define RREF      4300.0
  #define RNOMINAL  1000.0
#else
  #error "Select a sensor type: SENSOR_PT100 or SENSOR_PT1000"
#endif

Adafruit_MAX31865 thermo = Adafruit_MAX31865(PIN_CS, PIN_MOSI, PIN_MISO, PIN_SCK);

void setup() {
  Serial.begin(115200);

  pinMode(PIN_POWER, OUTPUT);
  digitalWrite(PIN_POWER, HIGH);   // enable power for uSup connector
  delay(500);

  Serial.println("LaskaKit MAX31865 PT100/PT1000 test");
#if defined(SENSOR_PT100)
  Serial.println("Sensor: PT100, RREF = 430 ohm");
#else
  Serial.println("Sensor: PT1000, RREF = 4300 ohm");
#endif

  if (!thermo.begin(SENSOR_WIRES)) {
    Serial.println("MAX31865 not found - check wiring and 3.3 V supply");
    while (1) delay(10);
  }
}

void loop() {
  uint16_t rtd = thermo.readRTD();

  float ratio = rtd;
  ratio /= 32768;

  Serial.print("RTD value: ");   Serial.println(rtd);
  Serial.print("Ratio: ");       Serial.println(ratio, 8);
  Serial.print("Resistance: ");  Serial.print(RREF * ratio, 3); Serial.println(" ohm");
  Serial.print("Temperature: "); Serial.print(thermo.temperature(RNOMINAL, RREF), 2); Serial.println(" C");

  // Check and print any faults
  uint8_t fault = thermo.readFault();
  if (fault) {
    Serial.print("Fault 0x"); Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) Serial.println("RTD High Threshold");
    if (fault & MAX31865_FAULT_LOWTHRESH)  Serial.println("RTD Low Threshold");
    if (fault & MAX31865_FAULT_REFINLOW)   Serial.println("REFIN- > 0.85 x Bias");
    if (fault & MAX31865_FAULT_REFINHIGH)  Serial.println("REFIN- < 0.85 x Bias - FORCE- open");
    if (fault & MAX31865_FAULT_RTDINLOW)   Serial.println("RTDIN- < 0.85 x Bias - FORCE- open");
    if (fault & MAX31865_FAULT_OVUV)       Serial.println("Under/Over voltage");
    Serial.println("Check: sensor wiring, sensor type (PT100/PT1000) in code vs. JP1+JP3, wire setting vs. JP2/JP4/JP5.");
    thermo.clearFault();
  }

  Serial.println();
  delay(1000);
}
