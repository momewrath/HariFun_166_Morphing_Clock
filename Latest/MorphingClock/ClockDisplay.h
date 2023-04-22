#ifndef CLOCKDISPLAY_H
#define CLOCKDISPLAY_H

#include <PxMatrix.h> 
#include <Ticker.h>

#include "Digit.h"

class ClockDisplay{
  public:
    ClockDisplay();
    void display_network_info(const char access_point_name[], const char access_point_password[], const char access_point_ip[]);
    void display_config_info(const char timezone[], const char time_format[]);
    void clear_display();
    void show_text(const char text[]);
    void show_time(int hh, int mm, int ss, bool is_pm, bool military);
    void morph_time(int hh, int mm, int ss, bool is_pm, bool military);
    void show_weather(float current_temp, float min_temp);

  private:
    uint16_t time_colour;
    Digit digit0;
    Digit digit1;
    Digit digit2;
    Digit digit3;
    Digit digit4;
    Digit digit5;  
    Ticker display_ticker;

    void show_ampm(bool is_pm); 
};
#endif