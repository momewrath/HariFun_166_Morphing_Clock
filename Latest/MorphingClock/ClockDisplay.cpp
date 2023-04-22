#include <Ticker.h>
#include <PxMatrix.h>

#include "ClockDisplay.h"
#include "TinyFont.h"
#include "Digit.h"

static const byte row0 = 2+0*10;
static const byte row1 = 2+1*10;
static const byte row2 = 2+2*10;

static const uint8_t p_lat = 16;
static const uint8_t p_a = 5;
static const uint8_t p_b = 4;
static const uint8_t p_c = 15;
static const uint8_t p_d = 12;
static const uint8_t p_e = 0;
static const uint8_t p_oe = 2;

// Pins for LED MATRIX
PxMATRIX display(64, 32, p_lat, p_oe, p_a, p_b, p_c, p_d, p_e);

ClockDisplay::ClockDisplay(){
  Serial.println("Initializing Clock Display");
  time_colour = display.color565(0, 255, 255);

  display.begin(16);

  digit0 = Digit(&display, 0, 63 - 16*1, 14, true, time_colour);
  digit1 = Digit(&display, 0, 63 - 11*2, 14, true, time_colour);
  digit2 = Digit(&display, 0, 63 - 4 - 9*3, 8, false, time_colour);
  digit3 = Digit(&display, 0, 63 - 4 - 9*4, 8, false, time_colour);
  digit4 = Digit(&display, 0, 63 - 7 - 9*5, 8, false, time_colour);
  digit5 = Digit(&display, 0, 63 - 7 - 9*6, 8, false, time_colour);

  display_ticker.attach_ms(2, [](PxMATRIX *display_to_use) {
    display_to_use->display(70);
  }, &display);
  
  display.setTextColor(time_colour);

  display.drawPixel(10, 10, time_colour);
}

void ClockDisplay::display_network_info(const char access_point_name[], const char access_point_password[], const char access_point_ip[]){
  clear_display();
  display.setCursor(1, row0); 
  display.print("AP");
  display.setCursor(1+10, row0); 
  display.print(":");
  display.setCursor(1+10+5, row0); 
  display.print(access_point_name);

  display.setCursor(1, row1); 
  display.print("Pw");
  display.setCursor(1+10, row1); 
  display.print(":");
  display.setCursor(1+10+5, row1); 
  display.print(access_point_password);

  display.setCursor(1, row2); 
  display.print("IP");
  display.setCursor(1+10, row2); 
  display.print(":");
  TFDrawText (&display, access_point_ip, 1+10+5, row2 + 1, time_colour);
}

void ClockDisplay::display_config_info(const char timezone[], const char time_format[]){
  clear_display();
  display.setCursor(1, row0); 
  display.print("Connected!");

  display.setCursor(1, row1); 
  display.print("Zone:");
  display.print(timezone);

  display.setCursor(1, row2); 
  display.print("Military:");
  display.print(time_format);
}

void ClockDisplay::clear_display(){
  display.fillScreen(display.color565(0, 0, 0));
}

void ClockDisplay::show_text(const char *text){
  clear_display();
  display.setCursor(2, row0);
  display.print(text);
}

void ClockDisplay::show_time(int hh, int mm, int ss, bool is_pm, bool military){
  clear_display();
  digit0.draw(ss % 10);
  digit1.draw(ss / 10);
  digit2.draw(mm % 10);
  digit3.draw(mm / 10);
  digit3.draw_colon(time_colour);
  digit4.draw(hh % 10);
  digit5.draw(hh / 10);

  if (!military){
    
    show_ampm(is_pm);
  }
}

void ClockDisplay::morph_time(int hh, int mm, int ss, bool is_pm, bool military){
  
  int s0 = ss % 10;
  int s1 = ss / 10;
  if (s0!=digit0.value()) digit0.morph(s0);
  if (s1!=digit1.value()) digit1.morph(s1);
        
  int m0 = mm % 10;
  int m1 = mm / 10;
  if (m0!=digit2.value()) digit2.morph(m0);
  if (m1!=digit3.value()) digit3.morph(m1);
      
  int h0 = hh % 10;
  int h1 = hh / 10;
  if (h0!=digit4.value()) digit4.morph(h0);
  if (h1!=digit5.value()) digit5.morph(h1);

  if (military){
    show_ampm(is_pm);
  }
}

void ClockDisplay::show_ampm(bool is_pm){
  if (is_pm){
    TFDrawText (&display, "PM", 44, 19, time_colour);
  }
  else{
    TFDrawText (&display, "AM", 44, 19, time_colour);
  }
}

int xo = 1, yo = 26;

void ClockDisplay::show_weather(float current_temp, float min_temp){
  int cc_wht = display.color565 (255, 255, 255);
  int cc_red = display.color565 (255, 0, 0);
  int cc_grn = display.color565 (0, 255, 0);
  int cc_blu = display.color565 (0, 0, 255);
  int cc_dgr = display.color565 (30, 30, 30);
  
  Serial.println ("showing the weather");
  xo = 0; yo = 1;
  TFDrawText (&display, String("                "), xo, yo, cc_dgr);
  
  if (current_temp == -10000)
  {
    Serial.println ("draw_weather: No weather data available");
  }
  else
  {
    int lcc = cc_red;
    
    if (current_temp < 26)
      lcc = cc_grn;
    if (current_temp < 18)
      lcc = cc_blu;
    if (current_temp < 6)
      lcc = cc_wht;
    
    //
    int current_temp_round = round(current_temp);
    String lstr = String(current_temp_round) + String("C");
    Serial.print("temperature: ");
    Serial.println (lstr);
    TFDrawText(&display, lstr, xo, yo, lcc);
    
    xo = 0*TF_COLS; yo = 26;
    TFDrawText(&display, "   ", xo, yo, 0);
    int min_temp_round = round(min_temp);
    lstr = String(min_temp_round);// + String((*u_metric=='Y')?"C":"F");
    //blue if negative
    int ct = cc_dgr;
    if (min_temp < 0)
    {
      ct = cc_blu;
      lstr = String(-min_temp);// + String((*u_metric=='Y')?"C":"F");
    }
    Serial.print("temp min: ");
    Serial.println(lstr);
    TFDrawText(&display, lstr, xo, yo, ct);

    //weather conditions
    //-humidity
    /*lcc = cc_red;
    if (humiM < 65)
      lcc = cc_grn;
    if (humiM < 35)
      lcc = cc_blu;
    if (humiM < 15)
      lcc = cc_wht;
    lstr = String (humiM) + "%";
    xo = 8*TF_COLS;
    TFDrawText (&display, lstr, xo, yo, lcc);
    //-pressure
    lstr = String (presM);
    xo = 12*TF_COLS;
    TFDrawText (&display, lstr, xo, yo, cc_blu);
    //draw temp min/max
    if (tempMin > -10000)
    {
      xo = 0*TF_COLS; yo = 26;
      TFDrawText (&display, "   ", xo, yo, 0);
      lstr = String (tempMin);// + String((*u_metric=='Y')?"C":"F");
      //blue if negative
      int ct = cc_dgr;
      if (tempMin < 0)
      {
        ct = cc_blu;
        lstr = String (-tempMin);// + String((*u_metric=='Y')?"C":"F");
      }
      Serial.print ("temp min: ");
      Serial.println (lstr);
      TFDrawText (&display, lstr, xo, yo, ct);
    }
    if (tempMax > -10000)
    {
      TFDrawText (&display, "   ", 13*TF_COLS, yo, 0);
      //move the text to the right or left as needed
      xo = 14*TF_COLS; yo = 26;
      if (tempMax < 10)
        xo = 15*TF_COLS;
      if (tempMax > 99)
        xo = 13*TF_COLS;
      lstr = String (tempMax);// + String((*u_metric=='Y')?"C":"F");
      //blue if negative
      int ct = cc_dgr;
      if (tempMax < 0)
      {
        ct = cc_blu;
        lstr = String (-tempMax);// + String((*u_metric=='Y')?"C":"F");
      }
      Serial.print ("temp max: ");
      Serial.println (lstr);
      TFDrawText (&display, lstr, xo, yo, ct);
    }
    //weather conditions
    draw_weather_conditions ();*/
  }
}
