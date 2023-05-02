#include <ArduinoJson.h>
#include <LittleFS.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <DoubleResetDetector.h>
//#include <WiFiUdp.h>

#include "Config.h"

//WiFiUDP udp; 

static const char wifi_manager_ap_name[] = "MorphClk";
static const char wifi_manager_ap_password[] = "booga12@";//This will change for final version

static const char *config_filename = "/config.json";
static const char *config_military = "military";
static const char *config_timezone = "timezone";

bool should_save_config = false;

#define DRD_TIMEOUT 10 // Second-reset must happen within 10 seconds of first reset to be considered a double-reset
#define DRD_ADDRESS 0 
DoubleResetDetector double_reset_detector(DRD_TIMEOUT, DRD_ADDRESS);

//callback notifying us of the need to save config
void save_config_callback () {
  Serial.println(F("Should save config"));
  should_save_config = true;
}


Config::Config(){
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount FS");
    return;
  }

  load_config();
}

char *Config::get_timezone(){
  return timezone;
}

char *Config::get_military(){
  return military;
}

/*void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());

   // We don't want the next time the board resets to be considered a double reset
  // so we remove the flag
  double_reset_detector.stop();
}*/

bool Config::load_config() {
  File config_file = LittleFS.open(config_filename, "r");
  if (!config_file) {
    Serial.println(F("Failed to open config file"));
    return false;
  }

  size_t size = config_file.size();
  if (size > 1024) {
    Serial.println(F("Config file size is too large"));
    return false;
  }

  std::unique_ptr<char[]> file_contents(new char[size]);

  config_file.readBytes(file_contents.get(), size);

  StaticJsonDocument<200> json_doc;
  auto error = deserializeJson(json_doc, file_contents.get());

  if (error) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    return false;
  }
  
  strcpy(timezone, json_doc[config_timezone]);
  strcpy(military, json_doc[config_military]);

  Serial.println(F("Config loaded successfully."));
  return true;
}

bool Config::save_config() {
  Serial.println(F("Saving config...."));
  Serial.print(config_timezone);
  Serial.print(F(": "));
  Serial.println(timezone);
  Serial.print(config_military);
  Serial.print(F(": "));
  Serial.println(military);

  StaticJsonDocument<200> json_doc;
  json_doc[config_timezone] = timezone;
  json_doc[config_military] = military;

  File config_file = LittleFS.open(config_filename, "w");
  if (!config_file) {
    Serial.println(F("Failed to open config file for writing"));
    return false;
  }

  serializeJson(json_doc, config_file);

  Serial.println(F("Save complete."));

  return true;
}

void Config::setup(ClockDisplay* clock_display)
{
  WiFiManager wifiManager;
  wifiManager.setSaveConfigCallback(save_config_callback);
  WiFiManagerParameter timezone_parameter(config_timezone, "Time Zone", get_timezone(), 5); 
  wifiManager.addParameter(&timezone_parameter);
  WiFiManagerParameter military_parameter(config_military, "24Hr", get_military(), 3); 
  wifiManager.addParameter(&military_parameter);

  Serial.println(F("Config::setup: Starting up..."));

  if (double_reset_detector.detectDoubleReset()) {
    Serial.println(F("Double Reset Detected"));
    digitalWrite(LED_BUILTIN, LOW);

    Serial.println(F("Displaying Wifi Info"));

    clock_display->display_network_info(wifi_manager_ap_name, wifi_manager_ap_password, "192.168.4.1");

    Serial.println(F("Starting Configuration Portal"));
    bool success = wifiManager.startConfigPortal(wifi_manager_ap_name, wifi_manager_ap_password);
    if (success){
      Serial.println(F("Portal started."));
    }
    else{
      Serial.println(F("Portal failed to start"));
    }
  } 
  else 
  {
    Serial.println(F("No Double Reset Detected"));
    digitalWrite(LED_BUILTIN, HIGH);

    clock_display->show_text("Connecting");

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name wifi_manager_ap_name
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect(wifi_manager_ap_name, wifi_manager_ap_password);
  }
  
  Serial.println(F("WiFi connected"));
  
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  strcpy(timezone, timezone_parameter.getValue());
  strcpy(military, military_parameter.getValue());
  
  clock_display->display_config_info(timezone, military);

  if (should_save_config) {
    save_config();
  }

  double_reset_detector.stop();
  
  delay(3000);
}

