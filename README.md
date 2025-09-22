# IP5108

[![Arduino Library Manager](https://img.shields.io/badge/Arduino-Library_Manager-00979D.svg?logo=arduino&logoColor=white)](https://docs.arduino.cc/libraries/IP5108/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub release](https://img.shields.io/github/v/release/milad-nikpendar/IP5108)](https://github.com/milad-nikpendar/IP5108/releases)
[![Author](https://img.shields.io/badge/Author-milad--nikpendar-blueviolet)](https://github.com/milad-nikpendar)

A high‑level Arduino library for the **IP5108** Power Management IC — the all‑in‑one Li‑ion charge/boost controller found in many power banks and portable devices.  
This library lets you control **charging**, **boost output**, **flashlight mode**, and **battery monitoring** over I²C, with clean APIs designe.

---

## ✨ Features

- Enable/disable:
  - Charger
  - Boost output
  - Flashlight / LED light
  - Auto‑power functions
- Configure:
  - Shutdown time
  - Light shutdown current
  - Battery type & charge voltage
  - Boost current limit
- Read:
  - Battery voltage, open‑circuit voltage, current
  - Charging state & status flags
  - Button press events (short, long, double)
  - Battery percentage
- Fully non‑blocking I²C interface (400 kHz)
- Minimal dependencies, Arduino‑IDE & PlatformIO ready

---

## 📦 Installation

### Arduino IDE
1. Download this repository as a ZIP.
2. In Arduino IDE: **Sketch → Include Library → Add .ZIP Library…**
3. Select the downloaded ZIP file.

### PlatformIO
Add to your `platformio.ini`:
```ini
lib_deps =
    https://github.com/milad-nikpendar/IP5108
```

---

## 🔌 Wiring

| IP5108 Pin | ESP32 Pin | Notes |
|------------|-------------------|-------|
| SDA        | GPIO21 (example)  | Any I²C‑capable pin |
| SCL        | GPIO22 (example)  | Any I²C‑capable pin |
| VCC        | 3.3 V / 5 V       | Per datasheet |
| GND        | GND               | Common ground |

**Default I²C address:** `0x75`  
**I²C speed:** up to 400 kHz

---

## 🚀 Quick Start

```cpp
#include <Wire.h>
#include <IP5108.h>

IP5108 pmic;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22, 400000); // SDA, SCL, freq
  if (!pmic.begin(0x75, &Wire)) {
    Serial.println("IP5108 not found!");
    while (1) delay(1000);
  }

  pmic.Charger(true);       // Enable charging
  pmic.Boost(true);         // Enable boost output
  pmic.FlashLight(true);    // Turn on flashlight
}

void loop() {
  Serial.printf("Battery: %u mV, %d mA, %d%%\n",
                pmic.BatteryVoltage(),
                pmic.BatteryCurrent(),
                pmic.battery_percentage());
  delay(1000);
}
```

---

## 📖 API Overview

| Method | Description |
|--------|-------------|
| `begin(addr, wire)` | Initialize with I²C address and Wire instance |
| `Charger(bool)` | Enable/disable battery charging |
| `Boost(bool)` | Enable/disable boost output |
| `FlashLight(bool)` | Enable/disable flashlight |
| `BatteryVoltage()` | Get battery voltage in mV |
| `BatteryCurrent()` | Get battery current in mA |
| `battery_percentage()` | Estimate battery percentage |
| `ChargingStatus()` | Get charging status flags |
| `setDoublePressAction(actionType_t, seconds)` | Configure button actions |

*(See `IP5108.h` for full API list.)*

---

## 🛠 Examples

- **Basic_Init** – Initialize and enable charger
- **Battery_Monitor** – Print battery voltage, current, and percentage

---

## 🧰 Development Notes

- Designed for **ESP32** under Arduino framework.
- Uses `TwoWire` for I²C — pass custom SDA/SCL pins if needed.
- All register definitions are based on the IP5108 datasheet.
- Includes helper functions for safe updates and bit‑mask writes.

---

## 🧾 License

This project is licensed under the **MIT License** – see [LICENSE](LICENSE) for details.

## ✍️ Author

**Milad Nikpendar**  
GitHub: [milad-nikpendar/IP5108](https://github.com/milad-nikpendar/IP5108)  
Email: milad82nikpendar@gmail.com  
