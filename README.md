# arduino-battery-voltage-tester
Battery Voltage Tester: Constant-Current Load with ADC Measurement and LCD Display

## About the Project

![Live Battery Testing Setup](images/systemInOperation.jpg)
This project is an Arduino-based AA battery tester designed to accurately measure the health of a 1.5V cell. Unlike standard multimeters that only measure open-circuit voltage—which can falsely show a depleted battery as fully charged at rest—this circuit applies a realistic constant load current of approximately 116 mA during testing. By measuring the terminal voltage while the battery does real work, it provides a highly accurate and honest assessment of the battery's remaining life.

## Features

- **Constant-Current Load:** The circuit continuously draws a stable 116 mA load, mimicking devices like TV remotes or small sensors, regardless of battery voltage fluctuations.
- **Isolated ADC Measurement:** The Arduino reads the battery terminal directly to avoid measuring distorted voltages caused by resistor drops inside the load circuit path.
- **Noise Reduction:** The software samples the voltage 60 times with a 2ms delay between each reading, averaging them out to eliminate electrical noise and achieve high accuracy.
- **User-Friendly LCD Display:** Outputs the voltage to three decimal places alongside a plain-language health label (GOOD, OK, LOW, DEAD) and a dynamic 16-block graphical health bar.

## Hardware Components

- **Microcontroller:** Arduino Uno
- **Op-Amp:** LM358 (acts as a constant-current controller)
- **Transistor:** BD139 NPN Power Transistor (acts as the variable load element)
- **Display:** 16x2 I2C Character LCD (JHD 2x16)
- **Resistor R1:** 10 kΩ (for the voltage divider)
- **Resistor R2:** 1 kΩ (for the voltage divider)
- **Resistor R3:** 1 kΩ (for base current limiting)
- **Resistor R4:** 100 kΩ (for ADC protection)
- **Resistor R5:** 3.9 Ω (for current sensing and load)

## How It Works

The circuit operates simultaneously in three integrated layers:

1. **Constant-Current Controller:** The R1 and R2 voltage divider provides a steady 0.455V reference to the non-inverting input of the LM358 op-amp. The op-amp continuously adjusts the base drive of the BD139 transistor to ensure exactly 0.455V drops across the 3.9 Ω sense resistor (R5), maintaining a tight 116 mA load current.
2. **Measurement Path:** A 100 kΩ resistor (R4) connects the battery's positive terminal directly to the Arduino's A0 pin. This high-impedance path prevents measurement distortion and protects the Arduino from potential voltage spikes.
3. **Display System:** The Arduino processes the ADC values and updates the I2C LCD approximately every 620 milliseconds over the SDA and SCL data lines.

## Status Thresholds

Because the battery is measured under load, the voltage thresholds are adjusted to reflect real-world performance (e.g., a "GOOD" battery reads >1.30V instead of 1.50V). The display maps the following states:

- **> 1.30 V:** GOOD (The cell is healthy and suitable for all normal applications)
- **1.10 V - 1.30 V:** OK (The cell still has usable charge and works in low-drain devices)
- **0.90 V - 1.10 V:** LOW (The cell is nearly empty and will fail soon)
- **0.10 V - 0.90 V:** DEAD (The cell is fully exhausted and should be replaced)
- **< 0.10 V:** No Cell (Nothing is connected to the test terminals)

## Software & Setup

1. Wire the components according to the provided schematic diagram.
2. Ensure you have the `Wire.h` and `LiquidCrystal_I2C.h` libraries installed in your Arduino environment.
3. Upload the provided source code to your Arduino Uno.
4. Connect a 1.5V AA battery to the terminals to see the live loaded voltage and health bar populate on the LCD screen.
```
