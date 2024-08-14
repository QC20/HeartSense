# Light Sensor Waveform Detection with NeoPixels

## Overview

This project demonstrates how to use a microcontroller with CircuitPython to read analog light sensor data, detect changes in light levels (waveforms), and visualize these changes using onboard NeoPixels. The code reads light intensity, calculates an average value, and detects when the light level crosses this average. When a full waveform cycle is detected, a NeoPixel briefly pulses red to indicate the event.

## Compatible Microcontrollers

This code is designed to work with any microcontroller that is compatible with CircuitPython and has onboard NeoPixels and an analog input pin for reading a light sensor. Some of the popular microcontrollers that meet these criteria include:

- **Adafruit Circuit Playground Bluefruit (CPB)**: This board comes with onboard NeoPixels and an analog light sensor, making it an ideal choice for this project.
- **Adafruit Circuit Playground Express (CPX)**: Similar to the CPB, this board also has onboard NeoPixels and an analog light sensor.
- **Adafruit Feather Series**: While not all Feathers have onboard NeoPixels or a light sensor, they can be used with external components to replicate this setup.
- **Adafruit Metro M0/M4 Express**: Like the Feather series, these boards can be used with external NeoPixels and light sensors.

### Prerequisites

- **CircuitPython**: Ensure that CircuitPython is installed on your microcontroller. You can download the latest version from the [CircuitPython website](https://circuitpython.org/).
- **CircuitPython Libraries**: You will need the `analogio` and `neopixel` libraries, which can be installed using the [Adafruit CircuitPython Library Bundle](https://circuitpython.org/libraries).

## How the Code Works

### Components:

- **Analog Light Sensor**: The light sensor reads the ambient light intensity as an analog signal. This signal is converted to a digital value by the microcontroller's ADC (Analog-to-Digital Converter).
- **NeoPixels**: The onboard NeoPixels provide visual feedback. One of the pixels is constantly lit green as a status indicator, while another pulses red when a full waveform cycle is detected.

### Process:

1. **Setup**:
    - The code initializes the NeoPixels and sets up the analog light sensor.
    - A green NeoPixel is turned on as a visual indicator that the system is active.

2. **Data Collection**:
    - The script continuously reads light intensity values from the sensor.
    - It oversamples by taking multiple readings in quick succession to reduce noise, and the average of these readings is stored in an array.

3. **Waveform Detection**:
    - The average light level is calculated from the stored samples.
    - The code detects when the light level crosses this average, indicating a zero crossing or a full waveform cycle (e.g., from above average to below average).
    - When such a crossing is detected, a NeoPixel briefly pulses red to indicate the event.

4. **LED Feedback**:
    - The feedback is visualized using the onboard NeoPixels. The red pulse provides a simple way to observe changes in light intensity in real time.

### Customization:

- **Sampling Rate**: You can adjust the `NUM_OVERSAMPLE`, `NUM_SAMPLES`, and `time.sleep()` values to change how frequently the sensor is read and how sensitive the waveform detection is.
- **NeoPixel Feedback**: The color and brightness of the NeoPixels can be customized to suit different applications.

## Applications

This project is useful for detecting periodic changes in light levels, such as in environmental monitoring, signal processing, or interactive installations. By visualizing these changes with NeoPixels, it becomes easier to see and respond to varying light conditions.

## Additional Resources

- [CircuitPython Documentation](https://learn.adafruit.com/welcome-to-circuitpython/overview): For a deeper understanding of CircuitPython and its capabilities.
- [Adafruit Circuit Playground Bluefruit Guide](https://learn.adafruit.com/adafruit-circuit-playground-bluefruit): To learn more about using the CPB with CircuitPython.
- [Adafruit Circuit Playground Express Guide](https://learn.adafruit.com/adafruit-circuit-playground-express): Similar guide for the CPX.

This project offers a simple yet effective way to work with analog sensors and visualize data on microcontrollers, making it a great learning tool for beginners and a useful component for more complex projects.
