#ifndef _HUD_H
#define _HUD_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "myimages.h"    // needed for Position Struct etc

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 //4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C //0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32


class Hud {
    public:
        Hud();
        void init();
        void render();

    private:
        Adafruit_SSD1306 display;

        int reticle_counter = 0;
        int reticle_length = 14;
        int inner_reticle_counter = 0;
        int inner_reticle_length=7;

        int draw_position = 0;
        int i = 1;

        PositionStruct reticule_data[52];
        PositionStruct inner_reticule_data[28];

};


#endif // _HUD_H