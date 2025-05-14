
**Orbittrack** 
- https://github.com/ultimaterobotics/orbitrack_esp32
- this program receives data from uECG device (auto connects to a nearby device) and draws them on attached screen. It can be quite easily adjusted to print ECG stream into serial monitor instead


In fact rf-nano is a rather good option here - you likely will get more clean data with it, without any additional effort (example provided with uECG arduino library works with it just fine - you only need to set proper EN, CS pins in the code). But BLE mode via ESP32 would work too



### Device Radio Modes:

1. **Pink LED (nRF24 Mode)**:  
   - The LED blinks pink three times after startup to indicate this mode.
   - This is the mode required for nRF24 communication.
   - Switch to this mode by pressing a button or changing a setting if the device is in a different mode.


2. **Blue LED (BLE Mode)**:  
   - Default mode at startup for Bluetooth Low Energy.
   - Indicated by three blue blinks after startup.
   - Switch from BLE mode as needed by following the procedure.

3. **Green LED (USB Receiver Mode)**:  
   - Required for connecting to the USB receiver.
   - Press the button until the LED blinks green three times.

*Note: If the LED blinks three red times in addition to the expected mode blinks, the battery is low. If using a CR2032 coin cell, it can be used until it stops working. If using a LiPo battery, recharge it before use.*
