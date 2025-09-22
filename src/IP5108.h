/*
   IP5108 library V1.1.0
   Created by Milad Nikpendar
*/
#ifndef IP5108_H
#define IP5108_H

#include <Arduino.h>
#include <Wire.h>
#include "Power_Value.h"

// IP5108 I2C description:

// 7-bit slave address IP5108 is 0x75 (byte with write bit = 0xEA; byte with read bit = 0xEB)
#define Slave_address_IP5108 0x75

// 400 Kbps speed of I2C supported
#define I2C_clock 400000

// 1 byte data = 8-bit data (bits from 7 to 0)

#define SYS_CTL0 0x01
#define SYS_CTL1 0x02
#define SYS_CTL2 0x0C
#define SYS_CTL3 0x03
#define SYS_CTL4 0x04
#define SYS_CTL5 0x07

#define CHARGER_CTL1 0x22
#define CHARGER_CTL2 0x24
#define CHG_DIG_CTL3 0x25
#define CHG_DIG_CTL4 0x26

#define Reg_READ0a 0x70
#define Reg_READ0b 0x71
#define Reg_READ1 0x72 // bit 7 (read only): 0 = WLED is off, 1 = WLED is on
// #define Reg_READ1  0x72 //bit 6 (read only): 0 = heavy load over 75 mA, 1 = light load less 75 mA
#define Reg_READ2 0x77

#define MFP_CTL0 0x51

#define GPIO_CTL2a 0x53
#define GPIO_CTL2b 0x54
#define GPIO_CTL3 0x55

#define BATVADC_DAT0 0xA2 // bits 7-0 (read only): LOW 8 bits (byte) of BATVADC
#define BATVADC_DAT1 0xA3 // bits 5-0 (read only): HIGH 6 bits of BATVADC

#define BATIADC_DAT0 0xA4 // bits 7-0 (read only): LOW 8 bits (byte) of BATIADC
#define BATIADC_DAT1 0xA5 // bits 5-0 (read only): HIGH 6 bits of BATIADC

#define BATOCV_DAT0 0xA8 // pins 7-0 (read only): LOW 8 bits (byte) of BATOCV
#define BATOCV_DAT1 0xA9 // pins 5-0 (read only): HIGH 6 bits of BATOCV

typedef enum
{
  Power,
  Light
} actionType_t;

class IP5108
{
public:
  // IP5108();
  bool begin(uint8_t i2caddr, TwoWire *myWire);
  bool ready();

  // Flashlight detection enable
  bool FlashLight();
  void FlashLight(bool status);

  // Light enable
  bool Light();
  void Light(bool status);

  // Boost enable
  bool Boost();
  void Boost(bool status);

  // Charger enable
  bool Charger();
  void Charger(bool status);

  // Light load shutdown enable
  bool LightShutdown();
  void LightShutdown(bool status);

  // Automatically turns on when loading
  bool AutoPowerOn();
  void AutoPowerOn(bool status);

  // Light load shutdown current threshold setting n*12mA
  // When the BAT current is less than the set threshold, it will shut down for 32 seconds.
  // Note: This current setting threshold needs to be greater than 100mA
  uint8_t LightShutdownCurrent();
  void LightShutdownCurrent(uint8_t current);

  // Shutdown time setting
  uint8_t ShutdownTime();
  void ShutdownTime(uint8_t time); // 8S, 16S, 32S, 64S

  // VIN pull out whether to turn on BOOST
  bool VinPullOutBOOST();
  void VinPullOutBOOST(bool status);

  // NTC function enable
  bool NTC_Enable();
  void NTC_Enable(bool status);

  // Set action for double press
  void setDoublePressAction(actionType_t action);

  //  // Set action for long press
  void setLongPressAction(actionType_t action, uint8_t seconds);

  // Charging undervoltage loop setting
  // (when charging, the output terminal VOUT Voltage)
  double ChargingVOUT();
  void ChargingVOUT(double voltage);

  // Battery type selection
  double BatteryType();
  void BatteryType(double voltage);

  // Battery charging voltage setting
  uint8_t BATChargingVoltage();
  void BATChargingVoltage(uint8_t voltage);

  // Battery type Vset selection
  bool BatteryTypeVset();
  void BatteryTypeVset(bool status);

  // Charging current setting (mA)
  float BATChargingCurrent();
  void BATChargingCurrent(float current);

  uint16_t BatteryVoltage();
  uint16_t BatteryOCVoltage();
  int16_t BatteryCurrent();

  // charging status: 0 = not charging, 1 = charging
  bool Charging();

  // Charging status indicator light
  uint8_t ChargingStatus();

  // Chgop
  bool Chgop();

  // Charging end sign
  bool ChargingEnd();

  // Constant voltage timeout flag
  bool ConstantVoltage();

  // Charging timeout
  bool ChargingTimeout();

  // Trickle charge timeout
  bool TrickleChargeTimeout();

  // Light LED Is there a light connected?
  bool LightConnected();

  // Light load flag
  bool LightNormalCurrent();

  // Input overvoltage status
  bool InputOvervoltage();

  // button button logo
  bool ButtonCurrent();

  // Button long press flag
  bool ButtonLongPress();

  // Button short press flag
  bool ButtonShortPress();

  // Battery percentage (0-100)
  int battery_percentage();

  // Power management status update
  bool update_setup(PowerSetup_t *setup);  // Initialize the power management status structure
  bool update_inputs(PowerInput_t *inputs); // Update the power management status inputs
  bool update_status(PowerStatus_t *status); // Update the power management status

private:
  TwoWire *_I2CWire;
  uint8_t _I2Caddr;

  bool writeRegister(uint8_t Register, uint8_t Data);
  uint8_t readRegister8(uint8_t Register, uint8_t length = 1);
  void printRegister(String name, uint8_t data);

protected:
  template <typename T>         // Update a value if it has changed
  void update(T &val, T source) // Update a value if it has changed
  {
    if (val != source)
    {
      val = source;
    }
  }

  // Button switch WLED flashlight mode
  // true : Short press the button twice
  // false: Long press for 2S
  bool FlashLightSWmode();
  void FlashLightSWmode(bool status);

  // Button shutdown mode
  // false: Short press the button twice
  // true : Long press for 2S
  bool ShutdownSWmode();
  void ShutdownSWmode(bool status);

  // Long press button time selection
  uint8_t LongButtonTime();
  void LongButtonTime(uint8_t time);

  // Shutdown by Short press twice in 1 second
  bool DoublePressShutdown();
  void DoublePressShutdown(bool status);
};

#endif
