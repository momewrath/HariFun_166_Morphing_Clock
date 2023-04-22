/*

 Udp NTP Client

 Get the time from a Network Time Protocol (NTP) time server
 Demonstrates use of UDP sendPacket and ReceivePacket
 For more on NTP time servers and the messages needed to communicate with them,
 see http://en.wikipedia.org/wiki/Network_Time_Protocol

 created 4 Sep 2010
 by Michael Margolis
 modified 9 Apr 2012
 by Tom Igoe
 updated for the ESP8266 12 Apr 2015 
 by Ivan Grokhotkov
 Refactored into NTPClient class by Hari Wiguna, 2018
 Refactored by Kristen Elliott 2023

 This code is in the public domain.

 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "ClockDisplay.h"

 class NTPClient {
  public:
    NTPClient();
    void setup(ClockDisplay* clock_display);
    unsigned long get_current_time();
    byte get_hours();
    byte get_minutes();
    byte get_seconds();
    bool get_is_pm();
    bool get_is_military();
    void print_time();
    
  private:
    void send_ntp_packet(IPAddress& address);
    void ask_current_epoch();
    void read_current_epoch();
 };

