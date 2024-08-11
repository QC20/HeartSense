# SPDX-FileCopyrightText: 2022 Isaac Wellish for Adafruit Industries
# SPDX-License-Identifier: MIT

"""
Circuit Playground Bluefruit BLE Heart Rate Display
This script reads heart rate data from a heart rate peripheral using
the standard BLE heart rate service.
The heart rate monitor connects to the Circuit Playground Bluefruit (CPB) via BLE.
The LEDs on the CPB blink to the heart rate of the user.
"""

import time  # Importing the time module for sleep and timing functions
import board  # Importing the board module to access board-specific hardware
import neopixel  # Importing the neopixel module to control the NeoPixels
import adafruit_ble  # Importing the Adafruit BLE module for Bluetooth functionality
from adafruit_ble.advertising.standard import ProvideServicesAdvertisement  # To handle BLE service advertisements
from adafruit_ble.services.standard.device_info import DeviceInfoService  # For accessing device information
from adafruit_ble_heart_rate import HeartRateService  # For accessing heart rate service
from digitalio import DigitalInOut, Direction  # To control the digital I/O pins

# Set up the onboard red LED (on pin D13) for status indication
red_led = DigitalInOut(board.D13)
red_led.direction = Direction.OUTPUT
red_led.value = True  # Turn on the LED initially to indicate the script is running

# Set up the NeoPixels on the CPB (Circuit Playground Bluefruit)
pixels = neopixel.NeoPixel(board.NEOPIXEL, 10, brightness=0.2, auto_write=False)  # Configure 10 NeoPixels with 20% brightness
RED = (255, 0, 0)  # Define the color red
LIGHTRED = (20, 0, 0)  # Define a dimmer red color for the heartbeat effect

def color_chase(color, wait):
    """
    Function to create a 'color chase' effect on the NeoPixels.
    """
    for i in range(10):  # Loop through each of the 10 NeoPixels
        pixels[i] = color  # Set the pixel to the specified color
        time.sleep(wait)  # Wait for the specified time before updating the next pixel
        pixels.show()  # Display the updated pixel
    time.sleep(0.5)  # Wait half a second before ending the animation

# Run a color chase animation to indicate that the program has started successfully
color_chase(RED, 0.1)  # Run the color chase with red color and 0.1 second delay

# Initialize the starting BPM (beats per minute) value
bpm = 60

# Initialize the BLE (Bluetooth Low Energy) radio
# PyLint might not recognize the BLERadio class, so it's disabled in the linting checks
ble = adafruit_ble.BLERadio()  # pylint: disable=no-member

hr_connection = None  # Initialize the heart rate connection as None

# Disconnect any existing heart rate service connections
if ble.connected:
    time.sleep(1)  # Wait a second to ensure stability
    for connection in ble.connections:  # Iterate through any existing connections
        if HeartRateService in connection:  # If a connection with HeartRateService exists
            connection.disconnect()  # Disconnect from it
        break  # Exit the loop after handling the first connection

# Main loop to manage the BLE connection and control the LEDs
while True:
    print("Scanning...")  # Print a message indicating that scanning has started
    red_led.value = True  # Turn on the red LED during scanning
    time.sleep(1)  # Wait a second

    # Scan for BLE devices offering the HeartRateService
    for adv in ble.start_scan(ProvideServicesAdvertisement, timeout=5):
        if HeartRateService in adv.services:  # If a heart rate service advertisement is found
            print("found a HeartRateService advertisement")
            hr_connection = ble.connect(adv)  # Connect to the device
            time.sleep(2)  # Wait 2 seconds to establish the connection
            print("Connected")
            red_led.value = False  # Turn off the red LED to indicate successful connection
            break  # Exit the scanning loop

    # Stop scanning regardless of connection success
    ble.stop_scan()
    print("Stopped scan")
    red_led.value = False  # Turn off the red LED
    time.sleep(0.5)  # Wait half a second

    if hr_connection and hr_connection.connected:  # If a connection is established
        print("Fetch connection")
        if DeviceInfoService in hr_connection:  # Try to get device information
            dis = hr_connection[DeviceInfoService]
            try:
                manufacturer = dis.manufacturer  # Get the device manufacturer
            except AttributeError:
                manufacturer = "(Manufacturer Not specified)"
            try:
                model_number = dis.model_number  # Get the device model number
            except AttributeError:
                model_number = "(Model number not specified)"
            print("Device:", manufacturer, model_number)
        else:
            print("No device information")

        # Access the heart rate service of the connected device
        hr_service = hr_connection[HeartRateService]
        print("Location:", hr_service.location)  # Print the sensor location (e.g., chest, wrist)

        while hr_connection.connected:  # While the BLE connection remains active
            values = hr_service.measurement_values  # Fetch the heart rate measurement values
            print(values)  # Print the full heart rate data set
            if values:
                bpm = values.heart_rate  # Update BPM value with the heart rate data
                if bpm == 0:  # If the heart rate is zero (could be a signal issue)
                    print("-")
                else:
                    time.sleep(0.1)  # Short delay before printing BPM
                    print(bpm)
            if bpm != 0:  # Prevent division by zero in case BPM is zero
                # Calculate the interval between heartbeats (time for one beat)
                bps = bpm / 60  # Convert BPM to beats per second
                period = 1 / bps  # The time period for one heartbeat
                time_on = 0.375 * period  # Time the LED should stay on
                time_off = period - time_on  # Time the LED should stay off

                # Blink LEDs in sync with the heartbeat
                pixels.fill(RED)  # Set all pixels to red
                pixels.show()  # Display the red color
                time.sleep(time_on)  # Keep LEDs on for the calculated time
                pixels.fill(LIGHTRED)  # Dim the LEDs
                pixels.show()  # Display the dimmed color
                time.sleep(time_off)  # Keep LEDs off for the remaining time
