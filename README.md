# HeartSense - Illuminating Your Heart's Rhythm

## Pseudo Code logic (should you stumble upon this project these are my preliminary ideas as to how this could be done)

Recieve ECG signal (options):

- Get HIGH / LOW signal from each heartbeat (and map it to a pre-defined simulation of a normal pulse waveform)
- Get the exact pulse waveform and map local minima and maxima to to some visualization (this option depends on the granularity of sensor)

Steady signal obtained, now what?

Normal case (steady resting heartrate)

- Set the the local minima really low, like a brightness of RGB(15, 0, 0,)
  - Local maxima will have slightly higher brightness levels depending on their relative wavesize RGB(30/20, 0, 0) (?)
  - Local minima will have slightly lower brightness levels depending om their relative wavesize (RGB5/10,0, 0) (?)

Showing rate of change (faster or slower)

- ???

Possible ideas:

- Brightness levels can be changed
- Pixel intensity levels can be changed
- The various LEDs can denote different things
  - If a LED ring with center LEDs is chosen it can have slightly higer values than the outer LEDs (or vice versa)
-

## Overview

HeartSense is an interactive LED ring that visually represents your heartbeats in real-time. The LED ring blinks to your heartbeats and displays the current BPM (beats per minute) using both numerical values and color-coded illumination.

## Features

- Real-time heart rate monitoring using ECG.
- LED ring that blinks in sync with your heartbeat.
- Displays BPM with color-coded LED illumination.
- Affective computing elements that visually convey heart rate changes.

## Components

- ECG sensor
- LED ring
- Microcontroller (e.g., Arduino, Raspberry Pi)
- Power supply
- Connecting wires and resistors

## Installation

### Hardware Setup

1. Connect the ECG sensor to the microcontroller.
2. Attach the LED ring to the microcontroller.
3. Ensure all connections are secure and powered correctly.

## Pulse Sensor: Technical Specifications

Here are the technical specifications:

| Parameter                     |      Value      |
| ----------------------------- | :-------------: |
| **VCC** (Voltage)             |   3.0 – 5.5V    |
| **IMax** (Maximum Current)    |      < 4mA      |
| **VOut** (Output Voltage)     |   0.3V to Vcc   |
| **LED Output** (Wavelength)   |      565nm      |
| **Sensor Input** (Wavelength) |      525nm      |
| **Dimensions** (L x W, PCB)   | 15.8mm (0.625") |
| **Lead Length**               |   20cm (7.8")   |

## How Does a Pulse Sensor Work?

The theory behind optical heart-rate sensors is straightforward. If you've ever shined a flashlight through your fingers and observed your heartbeat pulsing, the concept of optical heart-rate pulse sensors becomes clear.

<div align="center">
  ![Pulse Sensor Working](./assets/gif/Pulse-Sensor-Working.gif)
</div>

A pulse sensor, like any other optical heart-rate sensor, works by shining a green light (~ 550nm) on the finger and measuring the amount of reflected light with a [photosensor](https://en.wikipedia.org/wiki/Photodetector).

<div align="center">
  ![Photoplethysmogram](./assets/img/Photoplethysmogram.png)
</div>

This optical pulse detection technique is known as a [Photoplethysmogram](https://en.wikipedia.org/wiki/Photoplethysmogram).

The oxygenated hemoglobin in arterial blood absorbs green light. The redder the blood (the higher the hemoglobin), the greater the absorption of green light. With each heartbeat, blood is pumped through the finger, causing a change in the amount of reflected light, which produces a waveform at the photosensor’s output.

As you keep shining light and taking photosensor readings, you quickly begin to obtain a heartbeat pulse reading.

This signal from the photosensor is typically small and noisy; therefore, it is passed through an R/C filter network and then amplified with an [Operational Amplifier](https://en.wikipedia.org/wiki/Operational_amplifier) to create a signal that is significantly larger, cleaner, and easier to detect.

## The Human Heartbeat

<div align="center">
  <img src="assets/img/ECG-Graph.png" 
       alt="ECG Graph Display" 
       title="This graph shows ECG data."
       style="width: 50%; height: auto;">
</div>


**P wave** - It is the trailing wave on the left of the QRS complex.
**QRS complex** - It is an impulse generated by ventricular contraction.
**T wave** - It is a leading wave right to the QRS complex.
**U wave** - It is not always observed due to its low peak value.

I have created a [synthetic ECG signal](https://wokwi.com/projects/405107217963893761) for you to play around with to understand the type of input we can expect.
