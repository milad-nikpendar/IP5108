#include <Arduino.h>

typedef struct
{
  bool FlashLight;
  bool Light;
  bool Boost;
  bool Charger;
  bool LightShutdown;
  bool AutoPowerOn;
  uint8_t LightShutdownCurrent;
  uint8_t LongButtonTime;
  bool DoublePressShutdown;
  uint8_t ShutdownTime;
  bool VinPullOutBOOST;
  bool NTC_Enable;
  bool FlashLightSWmode;
  bool ShutdownSWmode;
  double ChargingVOUT;
  double BatteryType;
  uint8_t BATChargingVoltage;
  bool BatteryTypeVset;
  float BATChargingCurrent;
} PowerSetup_t;

typedef struct
{
  uint16_t BatteryVoltage;
  uint16_t BatteryOCVoltage;
  int16_t BatteryCurrent;
  byte ChargingStatus;
  bool Charging;
  bool Chgop;
  bool ChargingEnd;
  bool ConstantVoltage;
  bool ChargingTimeout;
  bool TrickleChargeTimeout;
  int8_t percent100;
  int8_t percent10;
} PowerStatus_t;

typedef struct
{
  bool LightConnected;
  bool LightNormalCurrent;
  bool InputOvervoltage;
  bool ButtonCurrent;
  bool ButtonLongPress;
  bool ButtonShortPress;
} PowerInput_t;
