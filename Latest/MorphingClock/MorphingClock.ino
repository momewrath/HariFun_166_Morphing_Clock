// Morphing Clock - Another Remix
//_________________________________________________________________
// **Thanks to:**
// Harifun for his original 6 digit morphing clock
// lmirel for their remix of the clock
// timz3818's remix of the clock
// Dominic Buchstaller for PxMatrix
// Tzapu for WifiManager
// ropg for ezTime
// Stephen Denne aka Datacute for DoubleResetDetector
// Brian Lough aka WitnessMeNow for tutorials on the matrix and WifiManager


#include <ArduinoJson.h>
#include <ezTime.h>
#include <ESP8266WiFi.h>

#include "ClockDisplay.h"
#include "Digit.h"
#include "Config.h"

Config config;
ClockDisplay clock_display;

static const uint8_t weather_conditions_len = 50;

Timezone timezone;

void setup() {
  Serial.begin(19200);
  while (!Serial) { ; }	

  clock_display = ClockDisplay();

  config.setup(&clock_display);
  setDebug(INFO);
  waitForSync();

	timezone.setLocation(F("America/Toronto"));
	Serial.print(F("Toronto:     "));
	Serial.println(timezone.dateTime());

  show_time(false);
  show_weather();
}

void loop(){

  events();

  if (secondChanged()){
    show_time(true);

    if (timezone.second()%60 == 0){
      Serial.println(F("Updating weather..."));
      show_weather();
    }
  }

/*if ((cm - last) > 150)
  {
    //Serial.println(millis() - last);
    last = cm;
    i++;
      
    draw_animations (i);
      
  }*/
}

void show_time(bool morph){
  uint8_t hours;

  bool is_military = config.get_military();
  
  if (is_military){
    hours = timezone.hour();
  }
  else{
    hours = timezone.hourFormat12();
  }

  uint8_t mins = timezone.minute();
  uint8_t seconds = timezone.second();
  bool is_pm = timezone.isPM();
  
  if (morph){
    clock_display.morph_time(hours, mins, seconds, is_pm, is_military);
  }
  else{
    clock_display.clear_display();
    clock_display.show_time(hours, mins, seconds, is_pm, is_military);
  }
}

char weather_api_key[] = "ca1a7bdba9574adefbba2361ba980c6e";
char weather_location[] = "Toronto,CA"; 
char weather_service[] = "api.openweathermap.org";

WiFiClient client;

String condS = "";

void show_weather()
{
  Serial.print(F("getWeather: Connecting to weather server "));
  Serial.println(weather_service);
  
  if (client.connect  (weather_service, 80))
  { 
    Serial.print(F("getWeather: Connected to serice."));
    Serial.println(weather_service); 
    // tryin getting rid of + by using a second print/println
    client.print("GET /data/2.5/weather?"); 
    client.print("q=");
    client.print(weather_location); 
    client.print("&appid=");
    client.print(weather_api_key); 
    client.println("&cnt=1&units=metric"); 
    client.print("Host: ");
    client.println(weather_service); 
    client.println("Connection: close");
    client.println(); 
  } 
  else 
  { 
    Serial.println (F("getWeather: Unable to connect."));
    return;
  } 
  
  delay(1000);
  
  String weather_info = client.readStringUntil('\n');
  if (!weather_info.length ())
    Serial.println (F("getWeather: unable to retrieve weather data"));
  else
  {
    Serial.println(F("getWeather: Weather response:")); 
    Serial.println(weather_info); 

    DynamicJsonDocument weather_json(1024);
    auto error = deserializeJson(weather_json, weather_info);

    if (error) {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(error.c_str());
      return;
    }

//TODO: may want to make sure that the values are there - could crash

    float weather_current_temp = weather_json[F("main")][F("temp")];
    float weather_max_temp = weather_json[F("main")][F("temp_max")];
    float weather_min_temp = weather_json[F("main")][F("temp_min")];
    float weather_feels_like_temp = weather_json[F("main")][F("feels_like")];
    float weather_wind_speed = weather_json[F("wind")][F("speed")];
    const char *weather_conditions = weather_json[F("weather")][0][F("main")];
    
    Serial.print(F("Weather: "));
    Serial.println(weather_conditions);
    Serial.print(F("Temp: "));
    Serial.println(weather_current_temp);
    Serial.print(F("Temp Max: "));
    Serial.println(weather_max_temp);
    Serial.print(F("Temp Min: "));
    Serial.println(weather_min_temp);
    Serial.print(F("Feels Like: "));
    Serial.println(weather_feels_like_temp);
    Serial.print(F("Wind Speed: "));
    Serial.println(weather_wind_speed);

    clock_display.show_weather(weather_current_temp, weather_min_temp, weather_max_temp, weather_feels_like_temp, weather_conditions);
  }

}




