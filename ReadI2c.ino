///*
// * CreateDate: 2020-08-21
// * Author: Alex Schmaus
// * Description: Inital trip into reading i2C sensor data
//*/

/*
 * TO DO:
 * [ ] Everything!
 * [ ] That one other thing
 */

// built in i2c library
#include <Wire.h>
// Import the adafruit driver for this sensor
#include "Adafruit_MCP9808.h"
// lib for screen
#include <Adafruit_SSD1306.h>
// lib for neopixel
#include <Adafruit_NeoPixel.h>
// lib for RTC
#include <SparkFun_RV8803.h>

// Create the sensor obj
Adafruit_MCP9808 sensor = Adafruit_MCP9808();

// RTC obj
RV8803 rtc;

// Temp limits
float highTemp = 72.0;
float lowTemp = 70.0;


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// declare the neopixel
Adafruit_NeoPixel rgb(1, 6, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


//===================================================================
// End Vars
//===================================================================

// the setup function runs once when you press reset or power the board
void setup() {
  
  //Wire.begin();
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);

  //setup the neopixel and clear it
  rgb.begin();
  rgb.clear();

  Serial.begin(9600);
  //while (!Serial); //Wait for serial termincal to open

  Serial.println("Hello World");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  //Send error message if can't begin the sensor
  if (!sensor.begin(0x18)){
    Serial.println("Can't find Sensor");
    while(1);
  } else {
    Serial.println("Sensor found");
    sensor.setResolution(3);
      // Mode Resolution SampleTime
      //  0    0.5°C       30 ms
      //  1    0.25°C      65 ms
      //  2    0.125°C     130 ms
      //  3    0.0625°C    250 ms
  }

  rtc.begin();
  
  display.display();
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.cp437(true);
  
  // End Setup
}

// the loop function runs over and over again forever
void loop() {
  sensor.wake(); //wake up sensor
  float c = sensor.readTempF();

  rgb.clear();

  

  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);

  
  Serial.print("Temp is: "); 
  Serial.println(c, 4);
  
  char result[6];

  //Updates the time variables from RTC 
  if (rtc.updateTime() == false) {
    Serial.print("RTC failed to update");
  }

  String curTime = rtc.stringTime();
  String curDate = rtc.stringDateUSA();
  Serial.println(curTime);
  
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println(curDate);
  display.setCursor(0,12);
  display.println(curTime);

  
  display.setTextSize(2);
  display.setCursor(32, 32);
  display.write(dtostrf(c, 6, 3, result));
  
  display.display();

  if (c < lowTemp){
    //digitalWrite(11, HIGH);
    rgb.setPixelColor(0, rgb.Color(0,0,128));
  }
  else if (c > highTemp){
   // digitalWrite(10, HIGH);
    rgb.setPixelColor(0, rgb.Color(128,0,0));
  }
  else {
    //digitalWrite(12, HIGH);
    rgb.setPixelColor(0, rgb.Color(0,128,0));
  }

  rgb.show();


  
  


  // TIME
  //  if (rtc.updateTime() == false) //Updates the time variables from RTC
  //{
  //  Serial.print("RTC failed to update");
  //}

  //String currentDate = rtc.stringDateUSA(); //Get the current date in mm/dd/yyyy format (we're weird)
  ////String currentDate = rtc.stringDate(); //Get the current date in dd/mm/yyyy format
  //String currentTime = rtc.stringTime(); //Get the time
  //Serial.print(currentDate);
  //Serial.print(" ");
  //Serial.println(currentTime);


  delay(250);                       // wait for a second
}
