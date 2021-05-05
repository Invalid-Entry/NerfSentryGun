#include "hud.h"

Hud::Hud() {
    // Constructor
    
    display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

}

void Hud::init() {
    // This is evil in a constructor? 
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }

    setup_reticule_data(reticule_data);
    setup_inner_reticule_data(inner_reticule_data);

    // Naughty in a constructor too!
    display.clearDisplay();
    display.display();
}

void Hud::set_horizon(float pitch, float roll){
    this->pitch = pitch;
    this->roll = roll;
}

void Hud::render() {
    // run a render cycle (in the loop of the main arduino)
    display.clearDisplay();

    // Draw horizon
    int height = 32 + 3.5 * pitch;
    int left_height = height - 3.5*roll;
    int right_height = height + 3.5*roll;
    display.drawLine(48, left_height, 79, right_height, SSD1306_WHITE);
    display.fillRect(56,24, 16, 16, SSD1306_BLACK); // start, w and h

    // FIXME: pitch only

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
}