# STM32-Captor-Display
STM32L476RG firmware — distance, gyroscope, temperature sensors, I2C/UART/ADC, custom PCB (EasyEDA) and 3D-printed enclosure

Embedded firmware project on **STM32L476RG (Nucleo)** — multi-sensor acquisition and real-time display, integrated into a custom PCB and 3D-printed enclosure.

---

## Overview

This project implements a standalone embedded measurement system capable of acquiring data from multiple sensors and displaying results on an LCD screen in real time. The system runs on a custom PCB designed around the STM32L476RG microcontroller, housed in a 3D-printed enclosure.

**Key achievement:** End-to-end embedded system — from firmware development to PCB layout and physical integration.

---

## Hardware

| Component | Role | Interface |
|---|---|---|
| STM32L476RG (Nucleo) | Main microcontroller | — |
| HC-SR04 | Ultrasonic distance sensor | GPIO + TIM (echo timing) |
| MPU6050 (GY-521) | Accelerometer / Gyroscope | I2C |
| Temperature sensor | Ambient temperature acquisition | ADC |
| LCD 16×2 | Real-time data display | I2C |
| Custom PCB (EasyEDA) | Nucleo integration + connectors | — |
| 3D-printed enclosure | Physical housing | — |

---

## Firmware — STM32CubeIDE

Developed in **C** using STM32CubeIDE and the HAL library.

### Peripherals configured

- **I2C** — MPU6050 (accelerometer/gyroscope) and LCD 16×2 display
- **UART** — Debug logging and serial communication
- **ADC** — Temperature sensor acquisition
- **TIM** — Microsecond timing for HC-SR04 echo measurement
- **GPIO** — HC-SR04 trigger, general I/O

### Firmware architecture

```
Core/
├── App/
│   ├── app.c        # State machine — button handling, display logic
│   └── app.h
├── Drivers/
│   ├── hcsr04.c     # HC-SR04 ultrasonic distance driver (TIM µs timing)
│   ├── hcsr04.h
│   ├── lcd_i2c.c    # LCD 16x2 I2C driver (PCF8574, 4-bit mode)
│   ├── lcd_i2c.h
│   ├── mpu6050.c    # MPU6050 accelerometer/gyroscope driver (I2C, no lib)
│   └── mpu6050.h
├── main.c           # Entry point — peripheral init, main loop
└── main.h           # Pin definitions, HAL handles
```

### Key implementation details

- **HC-SR04 distance measurement** — 10µs trigger pulse, echo duration captured via hardware timer for µs-precision ranging (no `HAL_Delay`)
- **MPU6050 via I2C** — register-level communication (WHO_AM_I, ACCEL_XOUT_H, GYRO_XOUT_H), raw data converted to m/s² and °/s
- **ADC temperature acquisition** — continuous conversion, averaged over N samples to reduce noise
- **LCD display** — I2C mode (PCF8574 expander), 4-bit protocol, refreshed at fixed rate with distance, acceleration, and temperature values
- **UART logging** — debug output for sensor values during development and validation

---

## PCB Design — EasyEDA

Custom PCB designed to host the Nucleo board and route all sensor connectors cleanly.

- Nucleo L476RG footprint with dedicated headers
- Connector blocks for each sensor (HC-SR04, MPU6050, LCD, temperature)
- Power supply routing (3.3V / 5V)
- Designed and exported with **EasyEDA**

> PCB schematic and Gerber files not included in this repository.

---

## 3D-Printed Enclosure

Custom enclosure designed to house the PCB and expose:
- Sensor openings (HC-SR04 front-facing)
- LCD window
- USB port for programming and UART debug

---

## Tools & Environment

| Tool | Usage |
|---|---|
| STM32CubeIDE | Firmware development and flashing |
| STM32CubeMX | Peripheral configuration (I2C, UART, ADC, TIM) |
| EasyEDA | PCB schematic and layout |
| 3D printer | Enclosure fabrication |
| PuTTY / CoolTerm | UART debug monitoring |

---

## Skills Demonstrated

- Embedded firmware development in C on STM32 (HAL)
- Multi-protocol integration : I2C, UART, ADC, GPIO/TIM
- Sensor driver implementation from datasheet (no external libraries)
- PCB design and hardware integration
- Full product cycle : firmware → PCB → physical enclosure

