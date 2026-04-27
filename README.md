# PIC32 PWM Melody Generator

This university project uses the **Basys MX3** development board and the **PIC32MX3** microcontroller to generate a melody using **PWM (Pulse Width Modulation)**.

## Project Overview
The project was designed to generate the melody **"Happy Birthday to You"** by producing a square-wave PWM signal on the **RB14** pin, using **Timer3** and **OC1**. Timing between notes is handled with **Timer2**, while note progression is managed through interrupt service routines.

## Main Features
- Melody generation using PWM
- Basys MX3 development board
- PIC32MX3 microcontroller
- Timer3 + OC1 for PWM signal generation
- Timer2 for note timing
- Interrupt-based note switching
- Sequential-state based solution

## Technical Elements
- PWM signal generation
- Timer configuration
- Interrupt Service Routines (ISR)
- Note frequency storage in arrays
- Embedded systems programming

## Project Files
- `SMP Project.pdf` – project presentation and explanation
- source code files (if available)

## What I Learned
- How to configure timers and PWM on a PIC32 microcontroller
- How to use interrupts for timing and control
- How to structure an embedded project for hardware-based sound generation
- How to troubleshoot timing and frequency issues in embedded systems

## Author
Adela Ortan
