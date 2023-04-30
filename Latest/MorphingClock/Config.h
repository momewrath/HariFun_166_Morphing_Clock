#ifndef CONFIG_H
#define CONFIG_H

/*

 Config 

 Reads and stores configuration information.

 This code is in the public domain.

 */

#include <Arduino.h>

#include "ClockDisplay.h"

class Config {

  public:
    Config();
    void setup(ClockDisplay *clock_display);
    bool load_config();
    bool save_config();
    char *get_timezone();
    char *get_military();
  private:
    char timezone[5] = "CHU";
    char military[3] = "N"; 
};

#endif