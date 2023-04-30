//#include <DNSServer.h>
//#include <ESP8266WebServer.h>
//
#include <DoubleResetDetector.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>

#include "NTPClient.h"
#include "ClockDisplay.h"
#include "Config.h"

//=== NTP CLIENT ===
#define DEBUG 0
const unsigned long askFrequency = 60*60*1000; // How frequent should we get current time? in miliseconds. 60minutes = 60*60s = 60*60*1000ms
unsigned long timeToAsk;
unsigned long timeToRead;
unsigned long lastEpoch; // We don't want to continually ask for epoch from time server, so this is the last epoch we received (could be up to an hour ago based on askFrequency)
unsigned long lastEpochTimeStamp; // What was millis() when asked server for Epoch we are currently using?
unsigned long nextEpochTimeStamp; // What was millis() when we asked server for the upcoming epoch
unsigned long currentTime;


const char* ntpServerName = "time.google.com"; // NTP google server
IPAddress timeServerIP; // time.nist.gov NTP server address
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
WiFiUDP udp; // A UDP instance to let us send and receive packets over UDP
unsigned int localPort = 2390;      // local port to listen for UDP packets

bool error_getTime = false;

Config config;


NTPClient::NTPClient()
{

}

void NTPClient::Setup(ClockDisplay* clockDisplay)
{
  config.setup(clockDisplay);
  
  Serial.println(F("Starting UDP"));
  udp.begin(localPort);
  Serial.print(F("Local port: "));
  Serial.println(udp.localPort());
}


// send an NTP request to the time server at the given address
void NTPClient::sendNTPpacket(IPAddress& address)
{
  if (DEBUG) Serial.println(F("sending NTP packet..."));
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

void NTPClient::AskCurrentEpoch()
{
  if (DEBUG) Serial.println("AskCurrentEpoch called");
  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP);

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
}

void NTPClient::ReadCurrentEpoch()
{
  if (DEBUG) Serial.println("ReadCurrentEpoch called");
  
  int cb = udp.parsePacket();
  
  if (!cb) {
    error_getTime = false;
    if (DEBUG) Serial.println("no packet yet");
  }
  else {
    error_getTime = true;
    if (DEBUG) Serial.print("packet received, length=");
    if (DEBUG) Serial.println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    if (DEBUG) Serial.print("Seconds since Jan 1 1900 = " );
    if (DEBUG) Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    if (DEBUG) Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    lastEpoch = secsSince1900 - seventyYears; // 1530082740=Fake 6:59:00, 1530795595=Fake 12:59:55, 1530835195=fake 23:59:55
    lastEpochTimeStamp = nextEpochTimeStamp; // Now that we have a new epoch, finally update lastEpochTimeStamp so all time calculations would be offset by the time we ask for this new epoch.

    if (DEBUG) Serial.println(lastEpoch);
    //return lastEpoch;
  }
}

unsigned long NTPClient::GetCurrentTime()
{
  //if (DEBUG) Serial.println("GetCurrentTime called");
  unsigned long timeNow = millis();
  if (timeNow > timeToAsk || !error_getTime) { // Is it time to ask server for current time?
    if (DEBUG) Serial.println(" Time to ask");
    timeToAsk = timeNow + askFrequency; // Don't ask again for a while
    if (timeToRead == 0) { // If we have not asked...
      timeToRead = timeNow + 1000; // Wait one second for server to respond
      AskCurrentEpoch(); // Ask time server what is the current time?
      nextEpochTimeStamp  = millis(); // next epoch we receive is for "now".
    } 
  }

  if (timeToRead>0 && timeNow > timeToRead) // Is it time to read the answer of our AskCurrentEpoch?
  {
    // Yes, it is time to read the answer. 
    ReadCurrentEpoch(); // Read the server response
    timeToRead = 0; // We have read the response, so reset for next time we need to ask for time.
  }
    
  if (lastEpoch != 0) {  // If we don't have lastEpoch yet, return zero so we won't try to display millis on the clock
    unsigned long zoneOffset = String(config.get_timezone()).toInt() * 3600;
    unsigned long elapsedMillis = millis() - lastEpochTimeStamp;
    currentTime =  lastEpoch + zoneOffset + (elapsedMillis / 1000);
  }
  return currentTime;
}

byte NTPClient::GetHours()
{
  int hours = (currentTime  % 86400L) / 3600;
  
  // Convert to AM/PM if military time option is off (N)
  if (config.get_military()[0] == 'N') {
    if (hours == 0) hours = 12; // Midnight in military time is 0:mm, but we want midnight to be 12:mm
    if (hours > 12) hours -= 12; // After noon 13:mm should show as 01:mm, etc...
  }
  return hours;
}

bool NTPClient::GetIsPM()
{
  int hours = (currentTime  % 86400L) / 3600;
  
  if(hours > 12)
    return true;
  
  return false;
}

byte NTPClient::GetMinutes()
{
  return (currentTime  % 3600) / 60;
}

byte NTPClient::GetSeconds()
{
  return currentTime % 60;
}

bool NTPClient::GetIsMilitary(){
  const char *military = config.get_military();
  if (!strncmp(military, "N", strlen(military))){
    return false;
  }
  
  return true;
}

void NTPClient::PrintTime()
{
  if (DEBUG) 
  {
    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    byte hh = GetHours();
    byte mm = GetMinutes();
    byte ss = GetSeconds();

    Serial.print(hh); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if ( mm < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print(mm); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ( ss < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(ss); // print the second
  }
}
