# Smart Irrigation System using Arduino Uno and Soil Moisture Sensor

## Description

This project implements a **Smart Irrigation System** on the **ATmega328P microcontroller** using **AVR programming in C**. The system automatically monitors soil moisture using an analog sensor and controls a water pump through a relay module. Key features include:

* **Analog-to-Digital Conversion (ADC)** to read soil moisture levels.
* **Timer1 with interrupts** for periodic sensor reading (every 1 second).
* **Relay control** for automated water pump switching.
* **UART serial communication** for debugging and real-time monitoring via the serial monitor.
* **Air detection logic** to prevent the pump from activating if the sensor is not in soil.

This system is designed to be energy-efficient, cost-effective, and a great introduction to embedded systems programming using AVR microcontrollers.

## Team Members

* Jabez Ian S. Macasero
* Jasfer Levin L. Eltanal

## Instructions

### 🧰 Hardware Requirements

* Arduino Uno (ATmega328P)
* Soil moisture sensor (analog output)
* Relay module
* Water pump (DC, compatible with relay)
* Breadboard and jumper wires
* Power supply (USB or external 5V/12V as required)

### 🔌 Wiring Overview

| Component            | Arduino Uno Pin |
| -------------------- | --------------- |
| Soil Moisture Signal | A0 (PC0)        |
| Relay IN             | D9 (PB1/OC1A)   |
| VCC & GND            | 5V and GND      |

> **Note:** Relay is active LOW, meaning it turns ON when the pin is set to 0.

### ⚙️ Software Setup

1. Clone or download this repository to your local machine.
2. Ensure that [WinAVR](http://winavr.sourceforge.net/) is installed and configured.
3. Open the project folder in **Visual Studio Code (VS Code)**.
4. In the terminal, compile the project using:

   ```bash
   make
   ```
5. Flash the compiled program to your Arduino Uno using:

   ```bash
   make flash
   ```

Make sure your Arduino is connected and the correct COM port is defined in the Makefile.

### 🖥️ Serial Monitor Setup

1. In **VS Code**, install the **"Serial Monitor"** extension by Microsoft.

   * Go to Extensions (Ctrl+Shift+X)
   * Search for `Serial Monitor`
   * Click Install on the one published by Microsoft
2. Plug in your Arduino Uno via USB.
3. Launch the Serial Monitor from the status bar or Command Palette.
4. Set baud rate to **9600 bps**.
5. Observe real-time messages: moisture readings, pump status, and air detection notices.
