
**Orbittrack** 
- https://github.com/ultimaterobotics/orbitrack_esp32
- this program receives data from uECG device (auto connects to a nearby device) and draws them on attached screen. It can be quite easily adjusted to print ECG stream into serial monitor instead




The device has two radio modes:

Pink LED: nRF24 mode (the mode you want).
Blue LED: BLE mode (the default mode at startup).
After the initial startup blinks, the LED will blink three times to indicate the current mode. To switch to Pink (nRF24) mode, you'll likely need to follow a specific procedure, such as pressing a button or changing a setting.