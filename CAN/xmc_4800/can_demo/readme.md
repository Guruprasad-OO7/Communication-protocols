# CAN Demo – Infineon XMC4800

This project demonstrates a basic but complete **CAN transmission setup**
using the **Infineon XMC4800** microcontroller.

The focus of this demo is to observe how **real sensor data** is converted
into CAN frames and transmitted on a physical CAN bus, where it can be
verified externally using professional tools.

---

## Overview

The system reads data from physical sensors connected to the XMC4800,
packages the values into a CAN message, and transmits the message
periodically using the **MultiCAN+** peripheral.

The transmitted CAN frames are monitored on a PC using a
**Kvaser Hybrid 2xCAN/LIN** interface and **CAN King**.

**Data flow:**

Sensor → MCU → CAN controller → CAN transceiver → CAN bus → PC

---

## Hardware Used

- **MCU**: Infineon XMC4800
- **Sensors**:
  - Potentiometer (ADC input)
  - HC-SR04 ultrasonic sensor (timer capture)
- **CAN Interface**:
  - Kvaser Hybrid 2xCAN/LIN
- **PC Tool**:
  - Kvaser CAN King

---

## CAN Configuration

- **CAN ID**: `0x123` (Standard 11-bit)
- **Bitrate**: `500 kbps`
- **Payload Length**: 4 bytes

### Payload Layout

| Byte | Signal |
|----|------|
| 0–1 | Potentiometer ADC value |
| 2–3 | Distance in centimeters (ultrasonic sensor) |

---

## Implementation Details

- ADC is used to sample the potentiometer value
- Timer capture is used to measure ultrasonic echo pulse width with
  microsecond resolution
- Distance is calculated from the measured echo pulse duration
- CAN message objects are configured
- CAN frames are transmitted periodically

---

## Validation

CAN traffic is verified on a PC using **Kvaser CAN King**.
The observed frames confirm:
- Correct CAN ID
- Correct payload mapping
- Continuous transmission at the expected rate

---

## Notes

This demo is intentionally kept simple to focus on the fundamentals of:
- CAN message transmission
- CAN bit timing and bus behavior
- End-to-end validation using real hardware

---

## Possible Extensions

- CAN receive (RX) node implementation
- Signal scaling and message counters
- Raspberry Pi CAN decoding
- Error handling and diagnostics
