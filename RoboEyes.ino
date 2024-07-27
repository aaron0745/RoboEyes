/* Wiring of OLED and IR Sensor to Arduino UNO

Components :
1. OLED I2C (128x64) - uses SH110X/SH1106 driver
2. IR Sensor
3. Arduino UNO

# OLED to UNO

vcc --> 5v
gnd --> gnd
sck/scl --> A5
sda --> A4

# IR SENSOR to UNO

gnd --> gnd
vcc --> 5v
out --> digital pin 2*/

#include <U8g2lib.h>

#define IR_SENSOR_PIN 2

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0); // Initialize the display SH1106

bool eyesUp = false; // Variable to track if eyes are in the "up" position
unsigned long lastUpdate = 0; // Variable to keep track of the last update time
int animationState = 0; // State variable to handle animation stages

void setup() 
{
  pinMode(IR_SENSOR_PIN, INPUT); // Set the IR sensor pin as an input
  u8g2.begin(); // Initialize the display
}

void loop() 
{
  int irValue = digitalRead(IR_SENSOR_PIN); // Read the IR sensor value
  unsigned long currentMillis = millis(); // Get the current time

  if (irValue == LOW) 
  {
    // IR sensor is active; show eyes up
    u8g2.clearBuffer(); // Clear the internal buffer
    u8g2.setFontMode(1); // Set font mode (typically for text rendering, may not be needed if not using text)
    u8g2.setBitmapMode(1); // Set bitmap mode (necessary for pixel graphics)
    u8g2.drawBox(24, 12, 35, 15); // Draws the rect for eyes up
    u8g2.drawBox(69, 12, 35, 15);
    u8g2.drawFilledEllipse(41, 12, 17, 10); // Draws the ellipse for eyes up
    u8g2.drawFilledEllipse(86, 12, 17, 10);
    u8g2.sendBuffer(); // Send buffer to display
    eyesUp = true; // Update the state to indicate eyes are up
    lastUpdate = currentMillis; // Reset the timer
  } 
  else 
  {
    if (eyesUp) 
    {
      // When IR sensor is not active, and eyes were previously up, start the animation
      eyesUp = false; // Reset the eyesUp state
      animationState = 0; // Start the animation
      lastUpdate = currentMillis; // Reset the timer
    }

    // Handle the animation
    if (animationState == 0) 
    {
      // Eyes open
      if (currentMillis - lastUpdate >= 350) 
      {
        u8g2.clearBuffer(); // Clear the internal buffer
        u8g2.setFontMode(1); // Set font mode (typically for text rendering, may not be needed if not using text)
        u8g2.setBitmapMode(1); // Set bitmap mode (necessary for pixel graphics)
        u8g2.drawRBox(24, 15, 35, 35, 7); // Draws the eyes open state
        u8g2.drawRBox(69, 15, 35, 35, 7);
        u8g2.sendBuffer(); // Send buffer to display
        lastUpdate = currentMillis; // Reset the timer
        animationState = 1; // Move to the next animation state
      }
    } 
    else if (animationState == 1) 
    {
      // Eyes closed
      if (currentMillis - lastUpdate >= 1700) 
      {
        u8g2.clearBuffer(); // Clear the internal buffer
        u8g2.setFontMode(1); // Set font mode (typically for text rendering, may not be needed if not using text)
        u8g2.setBitmapMode(1); // Set bitmap mode (necessary for pixel graphics)
        u8g2.drawBox(24, 33, 35, 1); // Draws the eyes closed state
        u8g2.drawBox(69, 33, 35, 1);
        u8g2.sendBuffer(); // Send buffer to display
        lastUpdate = currentMillis; // Reset the timer
        animationState = 0; // Move back to the open state
      }
    }
  }
}
