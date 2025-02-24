# LSControl Tower

## Overview
LSControl Tower is a microcontroller-based system developed for digital systems and microprocessor applications. It allows users to communicate with a control tower via a **3x4 matrix keyboard**, select ports, interact through an **LCD display**, and record/playback audio using a **Java interface**.

This project was developed as part of the **Digital Systems and Microprocessors** coursework (2022-2023).

## Features
- **Keyboard Input**: Users input a port code using a **3x4 matrix keyboard**.
- **LCD Display Navigation**: Menu options are displayed and navigated using a **joystick**.
- **Audio Recording & Playback**: Audio samples are recorded via a **microphone**, transmitted via serial communication, and played back through a **Java interface**.
- **Time Display & Adjustment**: The system keeps track of elapsed time since communication start and allows time adjustments.

## Microcontroller Used
- **Microcontroller**: PIC18F4321
- **Architecture**: 8-bit
- **Flash Memory**: 32 KB
- **RAM**: 1 KB
- **EEPROM**: 256 Bytes
- **ADC Resolution**: 10-bit
- **USART Communication**: Supports serial transmission
- **Timers & PWM**: Used for system timing, scrolling effects, and speaker control

## System Components
### Hardware
- **PIC18F4321 Microcontroller**
- **3x4 Matrix Keyboard**
- **LCD Display (2x16)**
- **Joystick**
- **Microphone**
- **Speaker**
- **EEPROM Memory**

### Software
- **Embedded C for microcontroller logic**
- **Java for the user interface**
- **Serial Communication Protocol for data transfer**

## Functional Overview
### 1. User Interaction
- The user selects a communication port using the **keyboard**.
- Menu navigation is performed using a **joystick**.
- Menu options may scroll using a **marquee effect** if text is too long.

### 2. Audio Recording & Playback
- Users can **record audio** with a microphone.
- Audio data is sampled and transmitted to the **Java interface**.
- Playback is initiated by selecting a recording from a list.

### 3. Time & System Functions
- The system maintains a **real-time clock**.
- Users can modify the system time.
- The elapsed time since communication start is displayed.

## Technical Details
### Pin Assignments
| Component     | Pins Used |
|--------------|------------|
| Keyboard Rows | RB[3..0] |
| Keyboard Columns | RB[6..4] |
| LCD Display | RC[5..0], RE0 |
| Speaker | RB7 |
| Joystick (Vertical Axis) | RA0 (AN0) |
| Microphone | RA1 (AN1) |
| Serial TX | RC6 (SIO_Tx) |
| Serial RX | RC7 (SIO_Rx) |

### Key Modules
- **EEPROM Motor**: Handles recording storage and retrieval.
- **Keyboard Motor**: Manages key scanning and debounce logic.
- **Speaker Motor**: Controls sound generation for feedback and alerts.
- **ADC Motor**: Converts analog signals from the joystick and microphone.
- **SIO Motor**: Manages serial communication with the Java interface.
- **Marquee Motor**: Implements scrolling text for long menu options.
- **Controller Motor**: Central logic unit managing user interaction and system states.

## Issues Encountered
- **Memory Constraints**: Optimized storage handling due to limited EEPROM space.
- **Debugging Challenges**: Serial debugging was complex due to tight memory usage.
- **Audio Quality**: Balancing baud rate and sound clarity required extensive testing.
- **Team Collaboration**: Initially challenging, but structured code organization improved workflow.

## Conclusion
The project successfully integrates **hardware and software components** for user interaction, audio recording, and system monitoring. It highlights the importance of **memory efficiency, structured programming, and debugging techniques** in embedded systems development.

## Authors
- **Alex Ferré** (`alex.fl`)
- **Kevin Eljarrat** (`kevin.eljarrat`)

## Submission Details
- **Course**: Digital Systems and Microprocessors
- **Year**: 2022-2023
- **Submission Date**: May 21, 2023
