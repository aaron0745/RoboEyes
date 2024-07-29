This is for displaying eyes on an oled i2c display 126x64
The eyes do move towards Up,Left and Right and they do blink in all three cases 
When either 2 out of 3 IR Sensors are activated there will be a glitch sequeance as long as the IR Sensors are active when the IR Sensors are no longer active they display a "RECONNECTING..." message
The wiring of the setup is given inside the ino file
Components required are :
1. Arduino UNO
2. OLED I2C Display (128x64) (SH110X Driver)
3. IR Sensor x3
After wiring the setup just upload the code
