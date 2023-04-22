#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <DoubleResetDetector.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "NTPClient.h"
#include "ClockDisplay.h"

//=== WIFI MANAGER ===
static const char wifi_manager_ap_name[] = "MorphClk";
char wifi_manager_ap_password[] = "booga12@";//This will change for final version

DoubleResetDetector drd(10, 0);

//== SAVING CONFIG ==
bool should_save_config = false; // flag for saving data

//callback notifying us of the need to save config
void save_config_callback () {
  Serial.println("Should save config");
  should_save_config = true;
}


//=== NTP CLIENT ===
#define DEBUG 0
static const unsigned long ask_frequency = 60*60*1000; // How frequent should we get current time? in miliseconds. 60minutes = 60*60s = 60*60*1000ms
unsigned long time_to_ask;
unsigned long time_to_read;
unsigned long lastEpoch; 
unsigned long lastEpochTimeStamp; // What was millis() when asked server for Epoch we are currently using?
unsigned long nextEpochTimeStamp; // What was millis() when we asked server for the upcoming epoch
unsigned long currentTime;
char timezone[5] = "CHU";
char military[3] = "N"; // 24 hour mode? Y/N

const char* ntpServerName = "time.google.com"; // NTP google server
IPAddress timeServerIP; // time.nist.gov NTP server address
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
WiFiUDP udp; // A UDP instance to let us send and receive packets over UDP
unsigned int localPort = 2390;      // local port to listen for UDP packets

bool error_getTime = false;

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());

   // We don't want the next time the board resets to be considered a double reset
  // so we remove the flag
  drd.stop();
}

bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  configFile.readBytes(buf.get(), size);

  StaticJsonDocument<200> jsonDoc;
  auto error = deserializeJson(jsonDoc, buf.get());

  if (error) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    return false;
  }
  
  strcpy(timezone, jsonDoc["timezone"]);
  strcpy(military, jsonDoc["military"]);

  Serial.println("Config loaded successfully.");
  return true;
}

bool saveConfig() {
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["timezone"] = timezone;
  jsonDoc["military"] = military;

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  Serial.print("timezone=");
  Serial.println(timezone);

  Serial.print("military=");
  Serial.println(military);

  serializeJson(jsonDoc, configFile);
  return true;
}

NTPClient::NTPClient()
{
}

void NTPClient::setup(ClockDisplay* clock_display)
{
  //-- Config --
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount FS");
    return;
  }
  loadConfig();

  //-- WiFiManager --
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  wifiManager.setSaveConfigCallback(save_config_callback);
  WiFiManagerParameter timeZoneParameter("timeZone", "Time Zone", timezone, 5); 
  wifiManager.addParameter(&timeZoneParameter);
  WiFiManagerParameter militaryParameter("military", "24Hr", military, 3); 
  wifiManager.addParameter(&militaryParameter);

  //-- Double-Reset --
  if (drd.detectDoubleReset()) {
    Serial.println("Double Reset Detected");
    digitalWrite(LED_BUILTIN, LOW);

    Serial.println("Displaying Wifi Info");

    clock_display->display_network_info(wifi_manager_ap_name, wifi_manager_ap_password, "192.168.4.1");

    Serial.println("Starting Configuration Portal");
    wifiManager.startConfigPortal(wifi_manager_ap_name, wifi_manager_ap_password);
    
    clock_display->clear_display();
  } 
  else 
  {
    Serial.println("No Double Reset Detected");
    digitalWrite(LED_BUILTIN, HIGH);

    clock_display->show_text("Connecting");

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name wifiManagerAPName
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect(wifi_manager_ap_name, wifi_manager_ap_password);
  }
  
  //-- Status --
  Serial.println("WiFi connected");
  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

  //-- Timezone --
  strcpy(timezone,timeZoneParameter.getValue());
  
  //-- Military --
  strcpy(military,militaryParameter.getValue());
  
  clock_display->display_config_info(timezone, military);

  if (should_save_config) {
    saveConfig();
  }

  drd.stop();
  
  delay(3000);
}


// send an NTP request to the time server at the given address
void NTPClient::send_ntp_packet(IPAddress& address)
{
  if (DEBUG) Serial.println("sending NTP packet...");
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

void NTPClient::ask_current_epoch()
{
  if (DEBUG) Serial.println("AskCurrentEpoch called");
  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP);

  send_ntp_packet(timeServerIP); // send an NTP packet to a time server
}

void NTPClient::read_current_epoch()
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

unsigned long NTPClient::get_current_time()
{
  //if (DEBUG) Serial.println("GetCurrentTime called");
  unsigned long timeNow = millis();

  if (timeNow > time_to_ask || !error_getTime) { // Is it time to ask server for current time?
    if (DEBUG) Serial.println(" Time to ask");
    time_to_ask = timeNow + ask_frequency; // Don't ask again for a while
    if (time_to_read == 0) { // If we have not asked...
      time_to_read = timeNow + 1000; // Wait one second for server to respond
      ask_current_epoch(); // Ask time server what is the current time?
      nextEpochTimeStamp  = millis(); // next epoch we receive is for "now".
    } 
  }

  if (time_to_read>0 && timeNow > time_to_read) // Is it time to read the answer of our AskCurrentEpoch?
  {
    // Yes, it is time to read the answer. 
    read_current_epoch(); // Read the server response
    time_to_read = 0; // We have read the response, so reset for next time we need to ask for time.
  }
    
  if (lastEpoch != 0) {  // If we don't have lastEpoch yet, return zero so we won't try to display millis on the clock
    unsigned long zoneOffset = String(timezone).toInt() * 3600;
    unsigned long elapsedMillis = millis() - lastEpochTimeStamp;
    currentTime =  lastEpoch + zoneOffset + (elapsedMillis / 1000);
  }
  return currentTime;
}

byte NTPClient::get_hours()
{
  int hours = (currentTime  % 86400L) / 3600;
  
  // Convert to AM/PM if military time option is off (N)
  if (military[0] == 'N') {
    if (hours == 0) hours = 12; // Midnight in military time is 0:mm, but we want midnight to be 12:mm
    if (hours > 12) hours -= 12; // After noon 13:mm should show as 01:mm, etc...
  }
  return hours;
}

bool NTPClient::get_is_pm()
{
  int hours = (currentTime  % 86400L) / 3600;
  
  if(hours > 12)
    return true;
  
  return false;
}

byte NTPClient::get_minutes()
{
  return (currentTime  % 3600) / 60;
}

byte NTPClient::get_seconds()
{
  return currentTime % 60;
}

bool NTPClient::get_is_military(){
  if (!strncmp(military, "N", strlen(military))){
    return false;
  }
  
  return true;
}

void NTPClient::print_time()
{
  if (DEBUG) 
  {
    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    byte hh = get_hours();
    byte mm = get_minutes();
    byte ss = get_seconds();

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
