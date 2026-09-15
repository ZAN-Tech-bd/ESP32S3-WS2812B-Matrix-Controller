# ESP32-S3 WS2812B (8x8) Matrix Controller

A safe and reliable way to control an 8x8 WS2812B RGB LED Matrix using an ESP32-S3 microcontroller. This project uses a 74HCT125 level shifter to properly convert the 3.3V logic signal from the ESP32 to the 5V logic required by the WS2812B LEDs, preventing flickering and data corruption.

## Table of Contents

- [Features](#features)
- [Hardware Components](#hardware-components)
- [Wiring & Connections](#wiring--connections)
- [Software Setup](#software-setup)
- [Installation & Usage](#installation--usage)
- [Code Examples](#code-examples)
- [Troubleshooting](#troubleshooting)
- [Safety Considerations](#safety-considerations)
- [License](#license)

## Features

✅ **8x8 RGB LED Matrix Control** - Full 64 LED addressable control  
✅ **Level Shifting** - Proper 3.3V to 5V signal conversion  
✅ **Power Protection** - Capacitor and resistor protection circuits  
✅ **FastLED Compatible** - Industry-standard LED control library  
✅ **Safe Operation** - Built-in brightness limiting and startup delays  
✅ **Well-Documented** - Clear wiring diagrams and code comments  

## Hardware Components

| Component | Specification | Purpose |
| :--- | :--- | :--- |
| **Microcontroller** | ESP32-S3 | Main control unit |
| **LED Matrix** | 8x8 WS2812B (64 LEDs) | RGB display output |
| **Level Shifter** | 74HCT125 (Quad Buffer) | 3.3V → 5V signal conversion |
| **Power Capacitor** | 1000µF 50V | Power surge protection |
| **Data Resistor** | 220Ω | Data line protection |
| **Power Supply** | External 5V, 3A-4A min | LED matrix power |

## Wiring & Connections

### ⚠️ CRITICAL: Common Ground

**The ESP32, Level Shifter, LED Matrix, and External Power Supply MUST share a Common Ground (GND).** Failure to do this will result in data corruption and flickering.

### Pin Mapping

| ESP32-S3 Pin | 74HCT125 Pin | Signal | Function |
| :--- | :--- | :--- | :--- |
| GPIO 5 | Pin 2 (1A) | Data Input | ESP32 data signal |
| GND | Pin 1 (1OE) | Output Enable | Ground OE for active output |
| 5V Power | Pin 14 (VCC) | Power | Level shifter supply |
| GND | Pin 7 (GND) | Ground | Common ground |

### LED Matrix Connections

| 74HCT125 Pin | LED Matrix Pin | Component | Function |
| :--- | :--- | :--- | :--- |
| Pin 3 (1Y) | DIN | 220Ω Resistor | Data input (protected) |
| 5V Power | VCC | - | Power supply (5V) |
| GND | GND | - | Common ground |

### Capacitor Placement

**Place the 1000µF capacitor in parallel across the 5V and GND terminals of the LED Matrix** to protect the first LED from power surges. This is especially important when the external power supply is first connected.

### Circuit Diagram (Text)

```
ESP32-S3 (3.3V Logic)
    |
    ├─ GPIO 5 ─────────────────> 74HCT125 Pin 2 (1A Input)
    |                                    |
    |                           74HCT125 Pin 3 (1Y Output)
    |                                    |
    |                                 220Ω Resistor
    |                                    |
    |                                    └─> WS2812B DIN
    |
    ├─ GND ─────────┬──────────> 74HCT125 Pin 1 (1OE)
    |               |
    |               ├──────────> 74HCT125 Pin 7 (GND)
    |               |
    |               └──────────> WS2812B GND
    |
5V Power Supply
    |
    ├─ 5V ──────────┬──────────> 74HCT125 Pin 14 (VCC)
    |               |
    |               └──────────> WS2812B VCC
    |
    └─ GND ─────────┬──────────> 74HCT125 Pin 7 (GND)
                    |
                    └──────────> WS2812B GND
                                   |
                    1000µF Capacitor (Parallel)
                                   |
                                   └─ GND
```

## Software Setup

### Requirements

- **Arduino IDE** (v1.8.0+) or **VS Code with PlatformIO**
- **FastLED Library** (v3.4.0+)
- **USB-C Cable** (for ESP32-S3 programming)
- **USB Driver** (CH340 or CP2102, depending on your board variant)

### Installing FastLED

#### Arduino IDE Method:

1. Open Arduino IDE
2. Go to **Sketch → Include Library → Manage Libraries**
3. Search for `FastLED`
4. Install the latest version by Daniel Garcia

#### PlatformIO Method:

Add to `platformio.ini`:
```ini
[env:esp32-s3-devkitc-1]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
lib_deps = fastled/FastLED@^3.4.0
```

### Board Configuration (Arduino IDE)

1. Go to **Tools → Board → Boards Manager**
2. Search for `ESP32` and install "esp32 by Espressif Systems"
3. Select **Board:** `ESP32S3 Dev Module`
4. Select **Upload Speed:** `921600`
5. Select **USB CDC On Boot:** `Enabled`

## Installation & Usage

### Step 1: Clone the Repository

```bash
git clone https://github.com/ZAN-Tech-bd/ESP32S3-WS2812B-Matrix-Controller.git
cd ESP32S3-WS2812B-Matrix-Controller
```

### Step 2: Open the Project

**Arduino IDE:**
1. Open `main.cpp` in Arduino IDE
2. The sketch will auto-load

**PlatformIO:**
1. Open the project folder in VS Code
2. PlatformIO will auto-detect the configuration

### Step 3: Configure Settings (if needed)

Edit `main.cpp` to customize:

```cpp
#define LED_PIN     5        // GPIO pin for data line (change if different)
#define NUM_LEDS    64       // Number of LEDs (8x8 = 64)
#define BRIGHTNESS  50       // Initial brightness (0-255, keep low for USB)
```

### Step 4: Compile & Upload

**Arduino IDE:**
- Click **Sketch → Upload** or press `Ctrl+U`

**PlatformIO:**
- Click **PlatformIO: Upload** or run:
  ```bash
  pio run -t upload
  ```

### Step 5: Test

Once uploaded, the LED matrix should cycle through:
- **Blue** (1 second)
- **Red at 100% brightness** (1 second)
- **Orange** (1 second)

Then the loop repeats.

## Code Examples

### Basic Solid Color

```cpp
// Fill entire matrix with a solid color
fill_solid(leds, NUM_LEDS, CRGB::Blue);
FastLED.show();
```

### Custom RGB Color

```cpp
// Set all LEDs to a custom color (R, G, B values 0-255)
for(int i = 0; i < NUM_LEDS; i++) {
  leds[i] = CRGB(255, 100, 0); // Orange
}
FastLED.show();
```

### Brightness Control

```cpp
// Set master brightness (0-255)
FastLED.setBrightness(150);
FastLED.show();
```

### Individual LED Control

```cpp
// Set individual LED colors (x, y coordinates for 8x8 grid)
int index = (y * 8) + x;  // Calculate linear index from 2D position
leds[index] = CRGB(255, 0, 0);  // Red
FastLED.show();
```

### Fade Effect

```cpp
// Fade colors from one to another
CRGB startColor = CRGB::Red;
CRGB endColor = CRGB::Blue;

for(int brightness = 0; brightness <= 255; brightness++) {
  fill_solid(leds, NUM_LEDS, startColor);
  FastLED.setBrightness(brightness);
  FastLED.show();
  delay(10);
}
```

### Clear Matrix

```cpp
// Turn off all LEDs
FastLED.clear();
FastLED.show();
```

## Troubleshooting

### LEDs Not Lighting Up

- **Check Power:** Verify external 5V supply is connected and delivering power
- **Check Wiring:** Confirm GPIO 5 is connected through level shifter to LED matrix DIN
- **Check Ground:** Verify common ground connection between all components
- **Check Code:** Ensure `LED_PIN` is set to `5` (or your chosen GPIO)
- **Check Brightness:** Increase `BRIGHTNESS` value (default is 50 for safety)

### Flickering LEDs

- **Level Shifter Issue:** Verify 74HCT125 is properly powered at Pin 14 (VCC)
- **Data Line:** Check 220Ω resistor is in series with data line
- **Power Supply:** Use a dedicated 5V supply with at least 3A capacity
- **Cable Quality:** Use good quality wires to avoid signal degradation

### LEDs Show Wrong Colors

- **Color Order:** Verify `COLOR_ORDER` is set to `GRB` (WS2812B standard)
- **Correction:** Ensure `.setCorrection(TypicalLEDStrip)` is called
- **Check Addressing:** Verify LED indices are correct (0-63 for 8x8 matrix)

### Upload Fails

- **Driver Issue:** Install CH340 or CP2102 driver for your board
- **Port Selection:** Select correct COM port in **Tools → Port**
- **Board Selection:** Confirm **ESP32S3 Dev Module** is selected
- **Cable:** Try a different USB-C cable (some are data-only)

### Program Crashes/Resets

- **Memory Issue:** Ensure sufficient heap memory (FastLED requires ~200 bytes minimum)
- **Power:** Inadequate power supply can cause instability
- **Delay:** Ensure startup delay (1000ms) is present in `setup()`

## Safety Considerations

### Power Safety

⚠️ **ALWAYS use an external 5V power supply rated for at least 3A-4A capacity.**

- Do NOT power the LED matrix via USB alone
- USB can only provide ~500mA, which may damage the first LED
- Use the 1000µF capacitor for power surge protection
- Double-check all power connections before first use

### Wiring Safety

- Verify common ground connection before powering on
- Ensure no loose wires that could short the circuit
- Use appropriate wire gauges for power lines (16-18 AWG recommended)
- Keep the level shifter away from heat sources

### Brightness Safety

- Start with `BRIGHTNESS 50` for testing
- Increase gradually to avoid eye strain and power issues
- Maximum safe brightness without additional cooling is typically 100-150

## Project Structure

```
ESP32S3-WS2812B-Matrix-Controller/
├── main.cpp              # Arduino sketch
├── README.md             # This file
├── LICENSE               # Project license
└── docs/                 # Additional documentation
    ├── WIRING.md         # Detailed wiring guide
    └── TROUBLESHOOTING.md # Troubleshooting guide
```

## Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Support & Contact

For issues, questions, or suggestions:

- Open an issue on GitHub
- Check the Troubleshooting section above
- Review FastLED documentation: https://fastled.io

## Acknowledgments

- **FastLED Library** - Daniel Garcia and contributors
- **ESP32-S3** - Espressif Systems
- **WS2812B LEDs** - WorldSemi

---

**Last Updated:** September 2026  
**Tested On:** ESP32-S3 Dev Module, Arduino IDE 2.x, FastLED 3.4.0+
