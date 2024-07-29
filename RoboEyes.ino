/*Robotic eyes on I2C display (128x64)

Required Components:
1. Arduino UNO
2. IR Sensor x3
3. OLED I2C Display

Wiring of Components:

1.  OLED I2C Display to Arduino UNO

VCC --> 5V
GND --> GND
SCL/SCK --> A5
SDA --> A5


2. IR Sensor to Arduino UNO

VCC --> 5V
GND --> GND
OUT --> Digital PIN (2,4,6) --> (UP,LEFT,RIGHT)

*/
#include <U8g2lib.h>

#define IR_SENSOR_PIN2 2
#define IR_SENSOR_PIN4 4
#define IR_SENSOR_PIN6 6

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0); // Initialize the display SH1106

unsigned long lastUpdate = 0; // Variable to keep track of the last update time
unsigned long glitchStartTime = 0; // Variable to keep track of the glitch animation start time
bool glitchActive = false; // Flag to indicate if glitch animation is active
bool reconnecting = false; // Flag to indicate if reconnecting message should be displayed
unsigned long lastReconnectTime = 0; // Time when reconnecting started
int currentMode = 0; // Variable to keep track of the current mode: 0 = normal, 1 = left, 2 = right, 3 = up

const unsigned long EYE_OPEN_TIME = 1700; // Time for eyes open
const unsigned long EYE_CLOSED_TIME = 300; // Time for eyes closed
const unsigned long RECONNECT_DELAY = 1500; // Time to display "RECONNECTING..." message

void setup() 
{
  pinMode(IR_SENSOR_PIN2, INPUT); // Set the IR sensor (2) pin as an input
  pinMode(IR_SENSOR_PIN4, INPUT); // Set the IR sensor (4) pin as an input
  pinMode(IR_SENSOR_PIN6, INPUT); // Set the IR sensor (6) pin as an input
  u8g2.begin(); // Initialize the display
}

void loop() 
{
  int irValue2 = digitalRead(IR_SENSOR_PIN2); // Read the IR sensor (2) value
  int irValue4 = digitalRead(IR_SENSOR_PIN4); // Read the IR sensor (4) value
  int irValue6 = digitalRead(IR_SENSOR_PIN6); // Read the IR sensor (6) value
  unsigned long currentMillis = millis(); // Get the current time

  // Check if two sensors are active simultaneously for glitch animation
  if ((irValue2 == LOW && irValue4 == LOW) || 
      (irValue2 == LOW && irValue6 == LOW) || 
      (irValue4 == LOW && irValue6 == LOW)) 
  {
    glitchActive = true;
    glitchStartTime = currentMillis;
    reconnecting = false; // Reset reconnecting flag
  } 
  else if (glitchActive) 
  {
    // If glitch was active but no longer valid, start reconnecting process
    glitchActive = false;
    reconnecting = true;
    lastReconnectTime = currentMillis;
  }

  // Handle glitch animation if it's active
  if (glitchActive) 
  {
    if (currentMillis - glitchStartTime < 2000) 
    {
      u8g2.clearBuffer();
      for (int i = 0; i < 10; i++) 
      {
        int x = random(128);
        int y = random(64);
        int w = random(10, 30);
        int h = random(1, 5);
        u8g2.drawBox(x, y, w, h);
      }
      u8g2.sendBuffer();
      return; // Skip the rest of the loop to continue glitch animation
    } 
    else 
    {
      glitchActive = false; // Reset glitch flag after 2 seconds
    }
  }

  // Handle reconnecting message if glitch is not active
  if (reconnecting) 
  {
    if (currentMillis - lastReconnectTime < RECONNECT_DELAY) 
    {
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_t0_11_mf);
      u8g2.drawStr(20, 30, "RECONNECTING...");
      u8g2.sendBuffer();
      return; // Skip the rest of the loop to show reconnecting message
    } 
    else 
    {
      reconnecting = false; // Reset reconnecting flag after message is displayed
    }
  }

  // Determine current mode based on IR sensors
  if (irValue2 == LOW) 
  {
    currentMode = 3; // Eyes up mode
  }
  else if (irValue4 == LOW) 
  {
    currentMode = 1; // Eyes left mode
  }
  else if (irValue6 == LOW) 
  {
    currentMode = 2; // Eyes right mode
  }
  else
  {
    currentMode = 0; // Normal mode
  }

  // Handle the different modes
  switch (currentMode) 
  {
    case 3: // Eyes up
      if (currentMillis - lastUpdate < EYE_OPEN_TIME) 
      {
        // Eyes open
        u8g2.clearBuffer();
        u8g2.drawRBox(24, 12, 35, 15, 5); // Draws the rect for eyes up
        u8g2.drawRBox(69, 12, 35, 15, 5);
        u8g2.drawFilledEllipse(41, 12, 17, 10); // Draws the ellipse for eyes up
        u8g2.drawFilledEllipse(86, 12, 17, 10);
      } 
      else if (currentMillis - lastUpdate < (EYE_OPEN_TIME + EYE_CLOSED_TIME)) 
      {
        // Eyes closed
        u8g2.clearBuffer();
        u8g2.drawBox(24, 33, 35, 1); // Draws the eyes closed state
        u8g2.drawBox(69, 33, 35, 1);
      } 
      else 
      {
        lastUpdate = currentMillis; // Reset the timer
      }
      u8g2.sendBuffer();
      break;

    case 1: // Eyes left blinking
    case 2: // Eyes right blinking
      if (currentMillis - lastUpdate < EYE_OPEN_TIME) 
      {
        // Eyes open
        u8g2.clearBuffer();
        if (currentMode == 1) 
        {
          // Draw eyes left open
          u8g2.drawRBox(20, 13, 38, 38, 9); // Draw the custom shape
          u8g2.drawRBox(63, 15, 33, 33, 7); // Draw the custom shape
        } 
        else 
        {
          // Draw eyes right open
          u8g2.drawRBox(30, 15, 33, 33, 7); // Draw the custom shape
          u8g2.drawRBox(73, 13, 38, 38, 9); // Draw the custom shape
        }
      } 
      else if (currentMillis - lastUpdate < (EYE_OPEN_TIME + EYE_CLOSED_TIME)) 
      {
        // Eyes closed
        u8g2.clearBuffer();
        if (currentMode == 1) 
        {
          // Draw eyes left closed
          u8g2.drawBox(15, 28, 40, 1); // Draws the eyes left closed state
          u8g2.drawBox(61, 28, 38, 1);
        } 
        else 
        {
          // Draw eyes right closed
          u8g2.drawBox(36, 28, 40, 1); // Draws the eyes right closed state
          u8g2.drawBox(82, 28, 38, 1);
        }
      } 
      else 
      {
        lastUpdate = currentMillis; // Reset the timer
      }
      u8g2.sendBuffer();
      break;

    default: // Normal blinking animation
      if (currentMillis - lastUpdate < EYE_OPEN_TIME) 
      {
        // Eyes open
        u8g2.clearBuffer();
        u8g2.drawRBox(24, 15, 35, 35, 7); // Draws the eyes open state
        u8g2.drawRBox(69, 15, 35, 35, 7);
      } 
      else if (currentMillis - lastUpdate < (EYE_OPEN_TIME + EYE_CLOSED_TIME)) 
      {
        // Eyes closed
        u8g2.clearBuffer();
        u8g2.drawBox(24, 33, 35, 1); // Draws the eyes closed state
        u8g2.drawBox(69, 33, 35, 1);
      } 
      else 
      {
        lastUpdate = currentMillis; // Reset the timer
      }
      u8g2.sendBuffer();
      break;
  }
}
