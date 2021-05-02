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

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include "myimages.h"
#include "hud.h"

Hud hud = Hud();

void setup() {
  Serial.begin(9600);
  Serial.print("!");
  hud.init();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  Serial.write("*");
  hud.render();

  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
}
