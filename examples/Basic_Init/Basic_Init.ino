/*
   Basic_Init.ino
   Example for IP5108 Arduino Library
   Author: Milad Nikpendar
   Description:
     Initializes the IP5108 PMIC, enables charging and boost output,
     and prints battery voltage, current, and percentage every second.
*/

#include <Wire.h>
#include <IP5108.h>

// Create IP5108 instance
IP5108 pmic;

void setup() {
  Serial.begin(115200);
  delay(100);

  // Initialize I2C (adjust SDA/SCL pins for your board)
  Wire.begin(21, 22, I2C_clock);

  // Begin IP5108 with default address
  if (!pmic.begin(Slave_address_IP5108, &Wire)) {
    Serial.println("IP5108 not found! Check wiring and I2C address.");
    while (1) delay(1000);
  }

  Serial.println("IP5108 initialized successfully.");

  // Enable charger and boost output
  pmic.Charger(true);
  pmic.Boost(true);

  // Optional: turn on flashlight for testing
  // pmic.FlashLight(true);
}

void loop() {
  uint16_t voltage = pmic.BatteryVoltage();
  int16_t current = pmic.BatteryCurrent();
  int percent = pmic.battery_percentage();

  Serial.printf("Battery: %u mV | %d mA | %d%%\n", voltage, current, percent);

  delay(1000);
}
