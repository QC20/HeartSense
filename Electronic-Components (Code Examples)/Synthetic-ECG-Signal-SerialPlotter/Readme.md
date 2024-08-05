# EKG Signal Simulation and Visualization

This subfolder contains various Arduino sketches for simulating and visualizing Electrocardiogram (EKG/ECG) signals using different output methods.

## Files Overview

### 1. Readme.md

This file provides an overview of the project and instructions for using the code.

### 2. Synthetic-ECG-SignalGenerator.ino

This sketch simulates the output of an Electrocardiogram (EKG/ECG) signal by printing a predefined array of EKG values to the Serial Monitor.

Key Features:
- Uses a 100-sample array representing an EKG waveform.
- Prints values with a 10 ms delay between each print.
- Useful for testing EKG data processing or simulating EKG signals.

How to Use:
1. Upload the code to your Arduino board.
2. Open the Serial Monitor (baud rate: 115200).
3. Observe the simulated EKG values printed one by one.

### 3. Synthetic-ECG-Signal-12LED-Ring-Mapping_V1.ino

This sketch simulates an ECG signal and maps the synthetic data to a NeoPixel LED ring.

Key Features:
- Maps ECG values to LED brightness.
- Updates the LED ring every 10ms.
- Sends current ECG values to the Serial Monitor.

How to Use:
1. Connect a NeoPixel LED ring to pin 6 of your Arduino.
2. Upload the code to your Arduino board.
3. Open the Serial Monitor (baud rate: 115200).
4. Observe the LED ring for visual representation of the ECG signal.

### 4. Synthetic-ECG-Signal-12LED-Ring-Mapping_V2.ino

This is an updated version of the V1 sketch with minor formatting changes and potentially slight modifications to the code logic (the core functionality remains the same).

### 5. Synthetic-ECG-Signal-Over-Entire-12LED-Ring.ino

This sketch visualizes a synthetic ECG signal across an entire 12-LED NeoPixel ring, providing a more comprehensive representation of the ECG waveform.

Key Features:
- Uses a 12-LED NeoPixel ring to display the ECG signal.
- Implements a sliding window approach to show a portion of the ECG waveform across all LEDs.
- Maps ECG values to LED brightness with emphasis on peaks.
- Updates the LED display every 10ms to match the synthetic data rate.
- Outputs current ECG values to the Serial Monitor for debugging.

How to Use:
1. Connect a 12-LED NeoPixel ring to pin 6 of your Arduino.
2. Fill the `ekg` array with synthetic ECG data representing P, QRS, T, and U waves.
3. Upload the code to your Arduino board.
4. Open the Serial Monitor (baud rate: 115200) to view the current ECG values.
5. Observe the LED ring for a visual representation of the ECG waveform moving across the entire ring.

This implementation provides a more dynamic visualization of the ECG signal, allowing you to see a larger portion of the waveform at once compared to the single-LED brightness approach in the previous versions.

## Additional Information

The ECG data used in these sketches is synthetic and represents a typical ECG waveform. You can modify the delay or array values to suit your specific needs.

<div align="center">
  
  <img src="assets/img/ECG-Graph.png" 
       alt="ECG Graph Display"
       title="This image shows a graph of ECG data."
       style="width: 50%; height: auto;">

</div>

Feel free to experiment with these sketches and adapt them to your specific requirements or hardware configurations.