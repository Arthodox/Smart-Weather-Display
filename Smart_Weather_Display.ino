#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const char *ssid     = "wifi";
const char *password = "password";
const char *api_key = "40fe2e801e7a48e0b66273e82e69e090";
RTC_DATA_ATTR int check = 0;

void call_api();


void setup(void) {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  if (check == 0){
    lcd.print("====================");
    lcd.setCursor(2,1);
    lcd.print("INITIALIZING");
    lcd.setCursor(4,2);
    lcd.print("Please Wait");
    lcd.setCursor(0,3);
    lcd.print("====================");
    lcd.setCursor(14,1);
    delay(175*100); // Delay Total = 70 detik untuk menginisialisasi Sensor agar bekerja dengan baik
    lcd.print(".");
    delay(175*100);
    lcd.print(".");
    delay(175*100);
    lcd.print(".");
    delay(175*100);
    check = 1;
    lcd.clear();
  }
  pinMode(button,INPUT);
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay (500);
    Serial.print ( "." );
  }
  
  lcd.setCursor(1,1);
  lcd.print("Hello There,");
  lcd.setCursor(3,2);
  lcd.print("Please Wait...");
  req_api();

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_4,1);
  lcd.noBacklight();
  esp_deep_sleep_start();
 
}

void loop() {
  
}

void call_api() {
  HTTPClient http;
  String url = "http://api.openweathermap.org/data/2.5/weather?q=Jakarta&units=metric&appid=";
  url += api_key;

  http.begin(url);
  int httpCode = http.GET();
  String payload = http.getString();

  DynamicJsonDocument doc (2048);
  DeserializationError error = deserializeJson(doc, payload);
  JsonObject results = doc["main"];
  float suhu = results["temp"];
  float suhu_min = results["temp_min"];
  float suhu_max = results["temp_max"];
  float tekanan = results["pressure"];
  float kelembaban = results["humidity"];
  results = doc["wind"];
  float kec_angin = results["speed"];
  results = doc["weather"][0];
  const char* weather = results["main"];
  const char* desc = results["description"];

  if(error.c_str() == "EmptyInput"){
    Serial.println(" Reinitializing JSON File");
    req_api();
  }
  else {
    lcd.clear();

    lcd.setCursor(1,0);
    lcd.print("Cuaca : ");
    lcd.setCursor(3,1);
    lcd.print(weather);
    lcd.setCursor(1,2);
    lcd.print("Detail : ");
    lcd.setCursor(3,3);
    lcd.print(desc);
    delay(2000);
    lcd.clear();
    
    lcd.setCursor(1,1);
    lcd.print("Suhu : ");
    lcd.print(suhu);
    lcd.setCursor(1,2);
    lcd.print("Kec. Angin : ");
    lcd.print(kec_angin);
    delay(2000);
    lcd.clear();
    
    lcd.setCursor(1,1);
    lcd.print("Suhu Min: ");
    lcd.print(suhu_min);
    lcd.setCursor(1,2);
    lcd.print("Suhu Maks: ");
    lcd.print(suhu_max);
    delay(2000);
    lcd.clear();
  
    lcd.setCursor(1,1);
    lcd.print("Tekanan : ");
    lcd.print(tekanan);
    lcd.setCursor(1,2);
    lcd.print("Kelembaban : ");
    lcd.print(kelembaban);
    delay(2000);
    lcd.clear(); 
  }

  /*if (error) {
    lcd.print(F("deserializeJson() failed: "));
    lcd.print(error.c_str());
    return;
  }*/
}
