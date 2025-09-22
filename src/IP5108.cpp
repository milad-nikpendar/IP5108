#include "IP5108.h"

bool IP5108::writeRegister(uint8_t Register, uint8_t Data)
{
  _I2CWire->beginTransmission(_I2Caddr);
  _I2CWire->write(Register);
  _I2CWire->write(Data);
  return (_I2CWire->endTransmission() == 0) ? true : false;
}
uint8_t IP5108::readRegister8(uint8_t Register, uint8_t length)
{
  _I2CWire->beginTransmission(_I2Caddr);
  _I2CWire->write(Register);
  _I2CWire->endTransmission();
  _I2CWire->requestFrom(_I2Caddr, length);
  uint8_t data_byte = _I2CWire->read();
  return data_byte;
}

void IP5108::printRegister(String name, uint8_t data)
{
  Serial.print(name + ": ");
  Serial.print(data, BIN);
  Serial.print("\t=>");
  Serial.println(data);
}

// IP5108();
bool IP5108::begin(uint8_t i2caddr, TwoWire *myWire)
{
  _I2Caddr = i2caddr;
  _I2CWire = myWire;
  return ready();
}
bool IP5108::ready()
{
  _I2CWire->beginTransmission(_I2Caddr);
  return (_I2CWire->endTransmission() == 0) ? true : false;
}

// Flashlight detection enable
bool IP5108::FlashLight()
{
  uint8_t data = readRegister8(SYS_CTL0);
  return bitRead(data, 4);
}
void IP5108::FlashLight(bool status)
{
  uint8_t data = readRegister8(SYS_CTL0);
  bitWrite(data, 4, status);
  writeRegister(SYS_CTL0, data);
}

// Light enable
bool IP5108::Light()
{
  uint8_t data = readRegister8(SYS_CTL0);
  return bitRead(data, 3);
}
void IP5108::Light(bool status)
{
  uint8_t data = readRegister8(SYS_CTL0);
  bitWrite(data, 3, status);
  writeRegister(SYS_CTL0, data);
}

// Boost enable
bool IP5108::Boost()
{
  uint8_t data = readRegister8(SYS_CTL0);
  return bitRead(data, 2);
}
void IP5108::Boost(bool status)
{
  uint8_t data = readRegister8(SYS_CTL0);
  bitWrite(data, 2, status);
  writeRegister(SYS_CTL0, data);
}

// Charger enable
bool IP5108::Charger()
{
  uint8_t data = readRegister8(SYS_CTL0);
  return bitRead(data, 1);
}
void IP5108::Charger(bool status)
{
  uint8_t data = readRegister8(SYS_CTL0);
  bitWrite(data, 1, status);
  writeRegister(SYS_CTL0, data);
}

// Light load shutdown enable
bool IP5108::LightShutdown()
{
  uint8_t data = readRegister8(SYS_CTL1);
  return bitRead(data, 1);
}
void IP5108::LightShutdown(bool status)
{
  uint8_t data = readRegister8(SYS_CTL1);
  bitWrite(data, 1, status);
  writeRegister(SYS_CTL1, data);
}

// Automatically turns on when loading
bool IP5108::AutoPowerOn()
{
  uint8_t data = readRegister8(SYS_CTL1);
  return bitRead(data, 0);
}
void IP5108::AutoPowerOn(bool status)
{
  uint8_t data = readRegister8(SYS_CTL1);
  bitWrite(data, 0, status);
  writeRegister(SYS_CTL1, data);
}

uint8_t IP5108::LightShutdownCurrent()
{
  uint8_t data = readRegister8(SYS_CTL2);
  uint8_t response = (data >> 3) & 0b00011111;
  return response;
}
void IP5108::LightShutdownCurrent(uint8_t current)
{
  uint8_t lowerBits = readRegister8(SYS_CTL2) & 0b00000111;
  uint8_t data = (current << 3) | lowerBits;
  writeRegister(SYS_CTL2, data);
}

// Long press button time selection
uint8_t IP5108::LongButtonTime()
{
  uint8_t data = readRegister8(SYS_CTL3);
  uint8_t response = ((data >> 6) & 0b00000011);
  printRegister("SYS_CTL3", data);
  printRegister("BTN_Time", response);
  response++;
  return response;
}
void IP5108::LongButtonTime(uint8_t time)
{
  uint8_t lowerBits = readRegister8(SYS_CTL3) & 0b00000011;
  time--;
  uint8_t data = (time << 6) | lowerBits;
  writeRegister(SYS_CTL3, data);
}

// Shutdown by Short press twice in 1 second
bool IP5108::DoublePressShutdown()
{
  uint8_t data = readRegister8(SYS_CTL3);
  return bitRead(data, 5);
}
void IP5108::DoublePressShutdown(bool status)
{
  uint8_t data = readRegister8(SYS_CTL3);
  bitWrite(data, 5, status);
  writeRegister(SYS_CTL3, data);
}

// Shutdown time setting
uint8_t IP5108::ShutdownTime()
{
  uint8_t data = readRegister8(SYS_CTL4);
  uint8_t response = ((data >> 6) & 0b00000011);

  if (response == 0b11)
    return 64; // 64S
  if (response == 0b10)
    return 32; // 32S
  if (response == 0b01)
    return 16; // 16S
  if (response == 0b00)
    return 8; // 8S
  return 0;
}
void IP5108::ShutdownTime(uint8_t time)
{
  uint8_t lowerBits = readRegister8(SYS_CTL4) & 0b00000011;
  uint8_t data;
  if (time == 64)
    data = (0b11 << 6) | lowerBits; // 64S
  else if (time == 32)
    data = (0b10 << 6) | lowerBits; // 32S
  else if (time == 16)
    data = (0b01 << 6) | lowerBits; // 16S
  else if (time == 8)
    data = (0b00 << 6) | lowerBits; // 8S
  else
    data = (0b10 << 6) | lowerBits; // 32S
  writeRegister(SYS_CTL4, data);
}

// VIN pull out whether to turn on BOOST
bool IP5108::VinPullOutBOOST()
{
  uint8_t data = readRegister8(SYS_CTL4);
  return bitRead(data, 5);
}
void IP5108::VinPullOutBOOST(bool status)
{
  uint8_t data = readRegister8(SYS_CTL4);
  bitWrite(data, 5, status);
  writeRegister(SYS_CTL4, data);
}

// NTC function enable
bool IP5108::NTC_Enable()
{
  uint8_t data = readRegister8(SYS_CTL5);
  return bitRead(data, 6);
}
void IP5108::NTC_Enable(bool status)
{
  uint8_t data = readRegister8(SYS_CTL5);
  bitWrite(data, 6, status);
  writeRegister(SYS_CTL5, data);
}

// Button switch WLED flashlight mode
bool IP5108::FlashLightSWmode()
{
  uint8_t data = readRegister8(SYS_CTL5);
  return bitRead(data, 1);
}
void IP5108::FlashLightSWmode(bool status)
{
  uint8_t data = readRegister8(SYS_CTL5);
  bitWrite(data, 1, status);
  writeRegister(SYS_CTL5, data);
}

// Button shutdown mode
bool IP5108::ShutdownSWmode()
{
  uint8_t data = readRegister8(SYS_CTL5);
  return bitRead(data, 0);
}
void IP5108::ShutdownSWmode(bool status)
{
  uint8_t data = readRegister8(SYS_CTL5);
  bitWrite(data, 0, status);
  writeRegister(SYS_CTL5, data);
}

// Set action for double press
void IP5108::setDoublePressAction(actionType_t action)
{

  switch (action)
  {
  case actionType_t::Power:
    ShutdownSWmode(false);
    DoublePressShutdown(false);
    break;
  case actionType_t::Light:
    FlashLightSWmode(true);
    break;
  default:
    break;
  }
}

void IP5108::setLongPressAction(actionType_t action, uint8_t seconds)
{
  seconds = constrain(seconds, 1, 4); // Ensure seconds is between 1 and 4
  LongButtonTime(seconds);

  switch (action)
  {
  case actionType_t::Power:
    ShutdownSWmode(true);
    DoublePressShutdown(true);
    break;
  case actionType_t::Light:
    FlashLightSWmode(false);
    break;
  default:
    break;
  }
}

// Charging undervoltage loop setting
double IP5108::ChargingVOUT()
{
  uint8_t data = readRegister8(CHARGER_CTL1);
  uint8_t response = ((data >> 2) & 0b00000011);

  if (response == 0b11)
    return 4.83; // 4.83V
  if (response == 0b10)
    return 4.73; // 4.73V
  if (response == 0b01)
    return 4.63; // 4.63V
  if (response == 0b00)
    return 4.53; // 4.53V
  return 0;
}
void IP5108::ChargingVOUT(double voltage)
{
  uint8_t maskByte = readRegister8(CHARGER_CTL1) & 0b00000011;
  uint8_t data;
  if (voltage == 4.83)
    data = (0b11 << 2) | maskByte; // 4.83V
  else if (voltage == 4.73)
    data = (0b10 << 2) | maskByte; // 4.73V
  else if (voltage == 4.63)
    data = (0b01 << 2) | maskByte; // 4.63V
  else if (voltage == 4.53)
    data = (0b00 << 2) | maskByte; // 4.53V
  else
    data = (0b01 << 2) | maskByte; // 4.63V
  writeRegister(CHARGER_CTL1, data);
}

// Battery type selection
double IP5108::BatteryType()
{
  uint8_t data = readRegister8(CHARGER_CTL2);
  uint8_t response = ((data >> 5) & 0b00000011);

  if (response == 0b10)
    return 4.35; // 4.35V
  if (response == 0b01)
    return 4.30; // 4.30V
  if (response == 0b00)
    return 4.20; // 4.20V
  return 0;
}
void IP5108::BatteryType(double voltage)
{
  uint8_t maskByte = readRegister8(CHARGER_CTL2) & 0b10011111;
  uint8_t data;
  if (voltage == 4.35)
    data = (0b10 << 5) | maskByte; // 4.35V
  else if (voltage == 4.3)
    data = (0b01 << 5) | maskByte; // 4.3V
  else if (voltage == 4.2)
    data = (0b00 << 5) | maskByte; // 4.2V
  else
    data = (0b00 << 5) | maskByte; // 4.2V
  writeRegister(CHARGER_CTL2, data);
}

// Battery charging voltage setting
uint8_t IP5108::BATChargingVoltage()
{
  uint8_t data = readRegister8(CHARGER_CTL2);
  uint8_t response = ((data >> 1) & 0b00000011);

  if (response == 0b11)
    return 42; // 42mV
  if (response == 0b10)
    return 28; // 28mV
  if (response == 0b01)
    return 14; // 14mV
  if (response == 0b00)
    return 0; // 0V
  return 0;
}
void IP5108::BATChargingVoltage(uint8_t voltage)
{
  uint8_t maskByte = readRegister8(CHARGER_CTL2) & 0b11111001;
  uint8_t data;
  if (voltage == 42)
    data = maskByte | (0b11 << 1); // 42mV
  else if (voltage == 28)
    data = maskByte | (0b10 << 1); // 28mV
  else if (voltage == 14)
    data = maskByte | (0b01 << 1); // 14mV
  else if (voltage == 0)
    data = maskByte | (0b00 << 1); // 0V
  else
    data = maskByte | (0b10 << 1); // 28mV
  writeRegister(CHARGER_CTL2, data);
}

// Battery type internal selection
bool IP5108::BatteryTypeVset()
{
  uint8_t data = readRegister8(CHG_DIG_CTL4);
  return bitRead(data, 6);
}
void IP5108::BatteryTypeVset(bool status)
{
  uint8_t data = readRegister8(CHG_DIG_CTL4);
  bitWrite(data, 6, status);
  writeRegister(CHG_DIG_CTL4, data);
}

// Charging current setting (A)
float IP5108::BATChargingCurrent()
{
  uint8_t raw = readRegister8(CHG_DIG_CTL3) & 0b00011111;
  const float weight[5] = {0.1f, 0.2f, 0.4f, 0.8f, 1.6f};
  float result = 0.0f;

  for (int i = 0; i < 5; ++i)
  {
    if (raw & (1 << i))
      result += weight[i];
  }
  return result;
}
void IP5108::BATChargingCurrent(float current)
{
  // uint8_t maskByte = readRegister8(CHG_DIG_CTL3) & 0b11100000;
  // uint8_t data = (current / 100) | maskByte;
  // writeRegister(CHG_DIG_CTL3, data);

  if (current < 100)
    current = 100;
  if (current > 3100)
    current = 3100;

  float remaining = current; // / 1000.0f; // Convert current to Amperes
  const float weights[5] = {0.1f, 0.2f, 0.4f, 0.8f, 1.6f};
  uint8_t bits = 0;

  for (int i = 4; i >= 0; --i)
  {
    if (remaining >= weights[i])
    {
      bits |= (1 << i);
      remaining -= weights[i];
    }
  }

  uint8_t mask = readRegister8(CHG_DIG_CTL3) & 0b11100000;
  writeRegister(CHG_DIG_CTL3, bits | mask);
}

uint16_t IP5108::BatteryVoltage()
{
  uint16_t BATVOL;
  uint8_t BATOCVADC_VALUE_low = readRegister8(BATVADC_DAT0);  // low 8bit
  uint8_t BATOCVADC_VALUE_high = readRegister8(BATVADC_DAT1); // high 6bit
  if ((BATOCVADC_VALUE_high & 0x20) == 0x20)
  {
    BATVOL = 2600 - ((~BATOCVADC_VALUE_low) + (~(BATOCVADC_VALUE_high & 0x1F)) * 256 + 1) * 0.26855;
  }
  else
  {
    BATVOL = 2600 + (BATOCVADC_VALUE_low + BATOCVADC_VALUE_high * 256) * 0.26855;
  }
  if (BATVOL == 4868)
    BATVOL = 0;
  return BATVOL;
}
uint16_t IP5108::BatteryOCVoltage()
{
  uint16_t BATVOL;
  uint8_t BATOCVADC_VALUE_low = readRegister8(BATOCV_DAT0);  // low 8bit
  uint8_t BATOCVADC_VALUE_high = readRegister8(BATOCV_DAT1); // high 6bit
  if ((BATOCVADC_VALUE_high & 0x20) == 0x20)
  {
    BATVOL = 2600 - ((~BATOCVADC_VALUE_low) + (~(BATOCVADC_VALUE_high & 0x1F)) * 256 + 1) * 0.26855;
  }
  else
  {
    BATVOL = 2600 + (BATOCVADC_VALUE_low + BATOCVADC_VALUE_high * 256) * 0.26855;
  }
  if (BATVOL == 4868)
    BATVOL = 0;
  return BATVOL;
}
int16_t IP5108::BatteryCurrent()
{
  int16_t BATCUR;
  uint8_t BATIADC_VALUE_low = readRegister8(BATIADC_DAT0);
  uint8_t BATIADC_VALUE_high = readRegister8(BATIADC_DAT1);
  if ((BATIADC_VALUE_high & 0x20) == 0x20)
  { // negative value
    char a = ~BATIADC_VALUE_low;
    char b = (~(BATIADC_VALUE_high & 0x1F) & 0x1f);
    int c = b * 256 + a + 1;
    BATCUR = -c * 0.745985;
    // BATCUR=-(int)(((~BATIADC_VALUE_low)+(~(BATIADC_VALUE_high & 0x1F))*256+1)*0.745985);
  }
  else
  {
    BATCUR = (BATIADC_VALUE_high * 256 + BATIADC_VALUE_low) * 0.745985; // mA
  }
  return BATCUR;
}

// charging status: 0 = not charging, 1 = charging
bool IP5108::Charging()
{
  uint8_t data = readRegister8(Reg_READ0a);
  return bitRead(data, 3);
}

// Charging status indicator light
uint8_t IP5108::ChargingStatus()
{
  uint8_t data = readRegister8(Reg_READ0b);
  uint8_t response = ((data >> 5) & 0b00000111);
  return response;
}

// Chgop
bool IP5108::Chgop()
{
  uint8_t data = readRegister8(Reg_READ0b);
  return bitRead(data, 4);
}

// Charging end sign
bool IP5108::ChargingEnd()
{
  uint8_t data = readRegister8(Reg_READ0b);
  return bitRead(data, 3);
}

// Constant voltage timeout flag
bool IP5108::ConstantVoltage()
{
  uint8_t data = readRegister8(Reg_READ0b);
  return bitRead(data, 2);
}

// Charging timeout
bool IP5108::ChargingTimeout()
{
  uint8_t data = readRegister8(Reg_READ0b);
  return bitRead(data, 1);
}

// Trickle charge timeout
bool IP5108::TrickleChargeTimeout()
{
  uint8_t data = readRegister8(Reg_READ0b);
  return bitRead(data, 0);
}

// Light LED Is there a light connected?
bool IP5108::LightConnected()
{
  uint8_t data = readRegister8(Reg_READ1);
  return bitRead(data, 7);
}

// Light load flag
bool IP5108::LightNormalCurrent()
{
  uint8_t data = readRegister8(Reg_READ1);
  return bitRead(data, 6);
}

// Input overvoltage status
bool IP5108::InputOvervoltage()
{
  uint8_t data = readRegister8(Reg_READ1);
  return bitRead(data, 5);
}

// button button logo
bool IP5108::ButtonCurrent()
{
  uint8_t data = readRegister8(Reg_READ2);
  return bitRead(data, 3);
}

// Button long press flag
bool IP5108::ButtonLongPress()
{
  uint8_t data = readRegister8(Reg_READ2);
  return bitRead(data, 1);
}

// Button short press flag
bool IP5108::ButtonShortPress()
{
  uint8_t data = readRegister8(Reg_READ2);
  return bitRead(data, 0);
}

int IP5108::battery_percentage()
{
  if (!ready())
    return -1;

  uint16_t voltage = BatteryOCVoltage(); // Get the real battery voltage
  float percentage = 0.0;
  int minVoltage = 3000;
  int maxVoltage = 4200;
  int thresholdVoltage = 4140;

  if (voltage < minVoltage)
    return -1;

  switch (ChargingStatus())
  {
  case 0: // Idle
    percentage = constrain(map(voltage, 3170, 4100, 1, 100), 1, 100);
    break;
  case 1: // Trickle Charging
    percentage = 100;
    break;
  case 2: // Constant Current Charging
    percentage = constrain(map(voltage, minVoltage, thresholdVoltage, 10, 80), 10, 80);
    break;
  case 3: // Constant Voltage Charging
    percentage = constrain(map(voltage, thresholdVoltage, maxVoltage, 80, 100), 80, 100);
    break;
  case 4: // Constant Voltage Stop Charging Detection
    percentage = 100;
    break;
  case 5: // Full Charge
    percentage = 100;
    break;
  case 6: // Charging Timeout
    percentage = -1;
    break;
  default:
    percentage = 0;
    break;
  }
  return percentage;
}

// Power management update setup
bool IP5108::update_setup(PowerSetup_t *setup)
{
  if (!ready())
    return false;

  update(setup->FlashLight, FlashLight());                     // flashlight detection enable
  update(setup->Light, Light());                               // light enable
  update(setup->Boost, Boost());                               // boost enable
  update(setup->Charger, Charger());                           // charger enable
  update(setup->LightShutdown, LightShutdown());               // light load shutdown enable
  update(setup->AutoPowerOn, AutoPowerOn());                   // automatically turns on when loading
  update(setup->LightShutdownCurrent, LightShutdownCurrent()); // light load shutdown current threshold
  update(setup->LongButtonTime, LongButtonTime());             // long press button time selection
  update(setup->DoublePressShutdown, DoublePressShutdown());   // shutdown by Short press
  update(setup->ShutdownTime, ShutdownTime());                 // shutdown time setting
  update(setup->VinPullOutBOOST, VinPullOutBOOST());           // VIN pull out whether to turn on BOOST
  update(setup->NTC_Enable, NTC_Enable());                     // NTC function enable
  update(setup->FlashLightSWmode, FlashLightSWmode());         // Button switch WLED flashlight mode
  update(setup->ShutdownSWmode, ShutdownSWmode());             // Button shutdown mode
  update(setup->BatteryType, BatteryType());                   // battery type selection
  update(setup->BatteryTypeVset, BatteryTypeVset());           // battery type voltage setting
  update(setup->BATChargingVoltage, BATChargingVoltage());     // battery charging voltage setting
  update(setup->BATChargingCurrent, BATChargingCurrent());     // charging current setting
  update(setup->ChargingVOUT, ChargingVOUT());                 // charging undervoltage loop setting

  return true;
}

// Power management update status
bool IP5108::update_status(PowerStatus_t *status)
{
  if (!ready())
    return false;

  update(status->BatteryVoltage, BatteryVoltage());             // battery charger voltage
  update(status->BatteryOCVoltage, BatteryOCVoltage());         // battery real voltage
  update(status->BatteryCurrent, BatteryCurrent());             // battery current
  update(status->ChargingStatus, ChargingStatus());             // charging status
  update(status->Charging, Charging());                         // charging status
  update(status->Chgop, Chgop());                               // charging status
  update(status->ChargingEnd, ChargingEnd());                   // charging end sign
  update(status->ConstantVoltage, ConstantVoltage());           // constant voltage timeout
  update(status->ChargingTimeout, ChargingTimeout());           // charging timeout
  update(status->TrickleChargeTimeout, TrickleChargeTimeout()); // trickle charge timeout
  status->percent100 = battery_percentage();                    // battery percentage
  status->percent10 = status->percent100 / 10;                   // battery percentage in 10s

  return true;
}

// Power management update inputs
bool IP5108::update_inputs(PowerInput_t *input)
{
  if (!ready())
    return false;

  update(input->LightConnected, LightConnected());         // light LED Is there a light connected?
  update(input->LightNormalCurrent, LightNormalCurrent()); // light load flag
  update(input->InputOvervoltage, InputOvervoltage());     // input overvoltage status
  update(input->ButtonCurrent, ButtonCurrent());           // button current
  update(input->ButtonLongPress, ButtonLongPress());       // button long press
  update(input->ButtonShortPress, ButtonShortPress());     // button short press

  return true;
}
