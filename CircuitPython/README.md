# Circuit Playground Bluefruit BLE Heart Rate Display

## Overview

This project showcases how to use the Circuit Playground Bluefruit (CPB) to create a heart rate display that visually represents the heart rate of a user in real-time using Bluetooth Low Energy (BLE). The CPB connects to a BLE-enabled heart rate monitor, reads the heart rate data, and then uses its onboard NeoPixels to blink in sync with the user’s heartbeat.

## Project Description

The heart rate monitor continuously sends heart rate data to the CPB via BLE. Once the CPB receives the data, it calculates the beats per minute (BPM) and visualizes the heartbeat by blinking its NeoPixels. The blinking pattern and speed are directly linked to the BPM received, creating a visually engaging and informative display.

This project is perfect for anyone interested in wearable electronics, fitness monitoring, or simply learning more about BLE and CircuitPython.

### Features

- **BLE Connectivity**: The CPB connects to a BLE-enabled heart rate monitor.
- **Real-Time Heart Rate Visualization**: The onboard NeoPixels blink in sync with the user's heartbeat.
- **Device Information Display**: The script fetches and displays information about the connected heart rate device.
- **Configurable Animation**: The animation speed and pattern are based on the BPM, creating a dynamic display.

## How the Code Works

The code is structured to perform the following tasks:

1. **Setup**: The code initializes the CPB's hardware, including the onboard red LED for status indication and the NeoPixels for visual feedback.

2. **BLE Initialization**: The BLE radio is set up to scan for heart rate service advertisements. If a device offering this service is found, the CPB connects to it.

3. **Heart Rate Data Processing**: Once connected, the code continuously reads the heart rate data from the device. If the data is valid, it calculates the time interval between beats and controls the NeoPixels to blink accordingly.

4. **Visualization**: The NeoPixels are configured to display two shades of red, one bright and one dim. The transition between these colors simulates a heartbeat, with the timing directly tied to the user’s BPM.

### Code Breakdown

- **Initialization**: The script starts by configuring the NeoPixels and setting up the BLE radio. It also prepares the onboard red LED to indicate status changes (e.g., when scanning for devices or when connected).

- **BLE Scanning and Connection**: The script scans for BLE devices that offer the Heart Rate Service. Once a suitable device is found, the CPB connects to it and stops scanning.

- **Data Handling**: The code then enters a loop where it continuously reads the heart rate data from the connected device. The heart rate (BPM) is used to control the timing of the LED blinking.

- **LED Control**: Based on the BPM, the script calculates how long the LEDs should be on and off to match the user's heart rate, providing a real-time visual representation of the heartbeat.

## Getting Started

To replicate this project, you can follow these steps:

1. **Hardware Setup**:
    - Circuit Playground Bluefruit
    - A BLE-enabled heart rate monitor

2. **Software Setup**:
    - Install CircuitPython on your CPB.
    - Download the necessary libraries for BLE and NeoPixel control.

3. **Code**:
    - You can find the code for this project in the [GitHub Repository](https://github.com/adafruit/Adafruit_Learning_System_Guides/blob/main/CPB_heart_rate_display/code.py).

4. **Run the Project**:
    - Once the code is uploaded to the CPB, power it on and it will start scanning for BLE heart rate monitors. The NeoPixels will begin to blink in sync with the detected heart rate.

## Additional Resources

For more information and detailed tutorials, refer to the following resources:

- [Heart Rate Pendant in Action](https://learn.adafruit.com/ble-heart-rate-display-pendant/heart-rate-pendant-in-action): This page shows how the project works in action, with videos and explanations.
- [Code the Heart Rate Display on CircuitPython](https://learn.adafruit.com/ble-heart-rate-display-pendant/code-the-heart-rate-display-on-circuitpython): A detailed guide on how to set up and code the heart rate display using CircuitPython.
- [GitHub Repository: CPB Heart Rate Display Code](https://github.com/adafruit/Adafruit_Learning_System_Guides/blob/main/CPB_heart_rate_display/code.py): The official GitHub repository where the full code for this project can be found.

This project not only helps you understand how to work with BLE and CircuitPython but also opens up possibilities for creating interactive and wearable electronics that can be both fun and functional.
