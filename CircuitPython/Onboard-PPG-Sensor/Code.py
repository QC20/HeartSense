import time  # Importing the time module for delays and timing functions
import analogio  # Importing the analogio module to read analog inputs
import board  # Importing the board module to access board-specific hardware
import neopixel  # Importing the neopixel module to control the NeoPixels

def sign(value):
    """
    Helper function to return the sign of a value.
    Returns 1 if positive, -1 if negative, and 0 if zero.
    """
    if value > 0:
        return 1
    if value < 0:
        return -1
    return 0

# Set up the NeoPixels on the board
pixels = neopixel.NeoPixel(board.NEOPIXEL, 10, brightness=1.0)  # Initialize 10 NeoPixels with maximum brightness

# Set up the analog light sensor
light = analogio.AnalogIn(board.LIGHT)  # Initialize the light sensor (analog input)

# Turn on the second NeoPixel (index 1) with green color
pixels[1] = (0, 255, 0)  # RGB color (0, 255, 0) corresponds to green

# Define the number of light readings per sample (oversampling)
NUM_OVERSAMPLE = 10

# Define the number of samples used to calculate the average light value
NUM_SAMPLES = 20

# Initialize a list to store the sampled light values
samples = [0] * NUM_SAMPLES

# Store the current time (used for timing purposes)
lasttime = time.monotonic()

# Main loop to continuously read light values and control the NeoPixels
while True:
    for i in range(NUM_SAMPLES):  # Loop through each sample slot
        # Take multiple (NUM_OVERSAMPLE) light readings quickly to reduce noise
        oversample = 0
        for s in range(NUM_OVERSAMPLE):
            oversample += float(light.value)  # Add each light reading to the oversample sum

        # Calculate the average of the oversampled readings
        samples[i] = oversample / NUM_OVERSAMPLE  # Store the averaged light value in the samples list

        # Calculate the mean (average) of all the sampled light values
        mean = sum(samples) / float(len(samples))  
        # Print the difference between the current sample and the mean, centering the reading
        print((samples[i] - mean,))  

        if i > 0:  # Check if we are not at the first sample (to avoid index error)
            # Check if the sign of the difference from the mean has changed
            # A sign change indicates a zero crossing (a complete waveform cycle)
            if sign(samples[i]-mean) <= 0 and sign(samples[i-1]-mean) > 0:
                pixels[9] = (200, 0, 0)  # If a waveform cycle is detected, briefly turn on the last NeoPixel (red pulse)
            else:
                pixels[9] = (0, 0, 0)  # Otherwise, turn off the last NeoPixel

        time.sleep(0.025)  # Small delay before the next sample (adjust this to control the speed of sampling)
