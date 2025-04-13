# ⏱️ Speed Chess Clock – Arduino + FreeRTOS

A real-time **Speed Chess Clock** built on **Arduino Uno** with **FreeRTOS**, enabling accurate timekeeping, responsive player switches, and modular task management. This project demonstrates embedded system design using multitasking, hardware interrupts, and LCD display handling.

---

## 🚀 Features

- 🧠 **FreeRTOS-based** multitasking
- 🔁 **Preemptive task scheduling** (button input always responsive)
- ⌛ **Accurate 1-second countdown**
- 🔘 **Event-driven turn switching** using semaphores
- 📟 **LCD time display** for both players
- 💡 **LED indicators** to show active player
- 🔧 Modular and scalable design

---

## 🛠️ Hardware Components

| Component            | Description                     |
|---------------------|---------------------------------|
| Arduino Uno R3       | Main microcontroller board      |
| 16x2 I2C LCD Display | For showing timers              |
| 2x Push Buttons      | For white/black clock control   |
| 2x LEDs              | To indicate current player      |
| Resistors (optional) | For LEDs                        |
| Breadboard + Wires   | For prototyping setup           |

---

## 📚 Libraries Used

### ➤ **Software Libraries**

| Library               | Purpose                              |
|------------------------|--------------------------------------|
| `Arduino_FreeRTOS.h`  | FreeRTOS kernel on AVR microcontrollers |
| `Wire.h`              | I2C communication with LCD           |
| `LiquidCrystal_I2C.h` | I2C LCD control (16x2 screen)        |

You can install these via:
- **Library Manager** in Arduino IDE, or
- Add to `libraries/` folder of your sketchbook.

---

## 📁 Folder Structure

