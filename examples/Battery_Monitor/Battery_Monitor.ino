/*
   Battery_Monitor.ino
   Example for IP5108 Arduino Library using PowerSetup_t, PowerInput_t, PowerStatus_t
   Author: Milad Nikpendar
   Description:
     Demonstrates how to read all setup, input, and status data from the IP5108 PMIC
     using the provided update_*() methods and data structures.
*/

#include <Wire.h>
#include <IP5108.h>
// Create IP5108 instance
IP5108 pmic;

// Structures to hold PMIC data
PowerSetup_t  setupData;
PowerInput_t  inputData;
PowerStatus_t statusData;

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
}

void loop() {
  // Update all structures
  pmic.update_setup(&setupData);
  pmic.update_inputs(&inputData);
  pmic.update_status(&statusData);

  Serial.println(F("------ Battery Monitor ------"));

  // From PowerStatus_t
  Serial.printf("Battery Voltage:      %u mV\n", statusData.BatteryVoltage);
  Serial.printf("Open-Circuit Voltage: %u mV\n", statusData.BatteryOCVoltage);
  Serial.printf("Battery Current:      %d mA\n", statusData.BatteryCurrent);
  Serial.printf("Charging Status:      0x%02X\n", statusData.ChargingStatus);
  Serial.printf("Charging:             %s\n", statusData.Charging ? "Yes" : "No");
  Serial.printf("Charge Operation:     %s\n", statusData.Chgop ? "Active" : "Inactive");
  Serial.printf("Charging End:         %s\n", statusData.ChargingEnd ? "Yes" : "No");
  Serial.printf("Const Voltage Timeout:%s\n", statusData.ConstantVoltage ? "Yes" : "No");
  Serial.printf("Charging Timeout:     %s\n", statusData.ChargingTimeout ? "Yes" : "No");
  Serial.printf("Trickle Timeout:      %s\n", statusData.TrickleChargeTimeout ? "Yes" : "No");
  Serial.printf("Percent (100):        %d %%\n", statusData.percent100);
  Serial.printf("Percent (10):         %d %%\n", statusData.percent10);

  // From PowerInput_t
  Serial.printf("Light Connected:      %s\n", inputData.LightConnected ? "Yes" : "No");
  Serial.printf("Light Normal Current: %s\n", inputData.LightNormalCurrent ? "Yes" : "No");
  Serial.printf("Input Overvoltage:    %s\n", inputData.InputOvervoltage ? "Yes" : "No");
  Serial.printf("Button Current:       %s\n", inputData.ButtonCurrent ? "Yes" : "No");
  Serial.printf("Button Long Press:    %s\n", inputData.ButtonLongPress ? "Yes" : "No");
  Serial.printf("Button Short Press:   %s\n", inputData.ButtonShortPress ? "Yes" : "No");

  // From PowerSetup_t (static config values)
  Serial.printf("Boost Enabled:        %s\n", setupData.Boost ? "Yes" : "No");
  Serial.printf("Charger Enabled:      %s\n", setupData.Charger ? "Yes" : "No");
  Serial.printf("Light Shutdown:       %s\n", setupData.LightShutdown ? "Yes" : "No");
  Serial.printf("Auto Power On:        %s\n", setupData.AutoPowerOn ? "Yes" : "No");
  Serial.printf("Shutdown Time:        %u s\n", setupData.ShutdownTime);
  Serial.printf("Light Shutdown Curr:  %u mA\n", setupData.LightShutdownCurrent * 12);
  Serial.printf("Battery Type Voltage: %.2f V\n", setupData.BatteryType);
  Serial.printf("Charge Voltage Set:   %u\n", setupData.BATChargingVoltage);
  Serial.printf("Charge Current Set:   %.0f mA\n", setupData.BATChargingCurrent);

  Serial.println();
  delay(1000);
}
