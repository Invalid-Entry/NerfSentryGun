/**************************************************************************
 Sentry Gun HUD code 
 
 Uses libraries from Adafruit
 
 Assumes:
 1. 128x64 SSD1306 OLED screen is plugged in to I2C on 0x3C
 2. MPU6050 is plugged into I2C on default pin

 Code is laid out as a mini-state machine. Large bitmaps are moved out into
 myimages.h just for code maintainability
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include "myimages.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 //4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C //0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int reticle_counter = 0;
int reticle_length = 10;
int inner_reticle_counter = 0;
int inner_reticle_length = 7;

int draw_position = 0;
int i = 0;

PositionStruct reticule_data[52];
PositionStruct inner_reticule_data[28];

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  setup_reticule_data(reticule_data);
  setup_inner_reticule_data(inner_reticule_data);


  display.clearDisplay();
  
  display.display();

}

void loop() {
  display.clearDisplay();

  // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
  // Outer reticule animation 
  reticle_counter += 1;
  if(reticle_counter == 52){
    reticle_counter = 0;
  }

  draw_position = reticle_counter;

  for(i = 0; i <= reticle_length; i++){
    PositionStruct x = reticule_data[draw_position];
    display.drawPixel(x.xVal, x.yVal, 1);
    
    draw_position -= 1;
    if(draw_position < 0){
      draw_position = 51;
    }
  }

  draw_position = reticle_counter + 26;
  while(draw_position >= 52){
    draw_position -= 52;
  }

  for(i = 0; i <= reticle_length; i++){
    PositionStruct x = reticule_data[draw_position];
    display.drawPixel(x.xVal, x.yVal, 1);
    
    draw_position -= 1;
    if(draw_position < 0){
      draw_position = 51;
    }
  }

  // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
  // inner
  inner_reticle_counter -= 1;
  if(inner_reticle_counter <0){
    inner_reticle_counter = 27;
  }

  draw_position = inner_reticle_counter;

  for(i = 0; i <= inner_reticle_length; i++){
    PositionStruct x = inner_reticule_data[draw_position];
    display.drawPixel(x.xVal, x.yVal, 1);
    
    draw_position += 1;
    if(draw_position == 28){
      draw_position = 0;
    }
  }

  draw_position = inner_reticle_counter + 14;
  while(draw_position >= 28){
    draw_position -= 28;
  }

  for(i = 0; i <= inner_reticle_length; i++){
    PositionStruct x = inner_reticule_data[draw_position];
    display.drawPixel(x.xVal, x.yVal, 1);
    
    draw_position += 1;
    if(draw_position == 28){
      draw_position = 0;
    }
  }

  // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
  // Mid dot 
  display.drawPixel(63, 31, 1);
  display.drawPixel(63, 32, 1);
  display.drawPixel(64, 31, 1);
  display.drawPixel(64, 32, 1);

  // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
  // Outer reticule animation 
  display.display();
  delay(100);
}
