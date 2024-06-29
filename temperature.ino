#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

// // #include <TridentTD_LineNotify.h>
// #define LINE_TOKEN  "aIjRsNSKTO5DUUS8L2Nxb3l3IFIKYaflSEtxMly20e8"

uint32_t temp_color_array[] = {0xffbf5f00, 0xffbf0000, 0xffbf005f, 0xff6e00bc, 0xff2828e2, 0xff0b0bea};
uint8_t temp_color = 0;

String openWeatherMapApiKey = "e441fd11460ad6f64b150974e71bbeb6";

String latti = "7.19882";
String lon = "100.5951";

static uint8_t startframe;
JSONVar myObject;
String weatherMain;

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;
unsigned long Hour = 0;
unsigned long refreshtime = 0;

String jsonBuffer;
// โหมดอุณหภูมิเเละสภาพอากาศ
void display_temp()
{
  // LINE.notify("MODE-Weather");

  // Send an HTTP GET request
  if ((millis() - lastTime) > timerDelay)
  {

    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {

      if ((millis() - Hour) > refreshtime)
      {
        Hour = millis();
        refreshtime = 3600000;
        String serverPath = "http://api.openweathermap.org/data/3.0/onecall?lat=" + latti + "&lon=" + lon + "&appid=" + openWeatherMapApiKey;

        jsonBuffer = httpGETRequest(serverPath.c_str());
        myObject = JSON.parse(jsonBuffer);

        // JSON.typeof(jsonVar) can be used to get the type of the var
        if (JSON.typeof(myObject) == "undefined")
        {
          Serial.println("Parsing input failed!");
          return;
        }

        Serial.print("weather: ");
        Serial.println(myObject["current"]["weather"][0]["main"]);
      }
    }

    else
    {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  
  JSONVar currentWeather = myObject["current"]["weather"][0]["main"];
  String weatherString = JSON.stringify(currentWeather);

  // Serial.println(weatherString);
  // LINE.notify("testttttt");
  if (weatherString == "\"Clear\"")
  {
    startframe = clears;
    // LINE.notify("WEATHER : Clear");
  }
  else if (weatherString == "\"Thunderstorm\"")
  {
    startframe = thunderstorm;
    // LINE.notify("WEATHER : Thunderstorm");
  }
  else if (weatherString == "\"Drizzle\"")
  {
    startframe = drizzle;
    // LINE.notify("WEATHER : Drizzle");
  }
  else if (weatherString == "\"Rain\"")
  {
    startframe = rain;
    // LINE.notify("WEATHER : Rain");
  }
  else if (weatherString == "\"Snow\"")
  {
    startframe = snow;
    // LINE.notify("WEATHER : Snow");
  }
  else if (weatherString == "\"Atmosphere\"")
  {
    startframe = atmosphere;
    // LINE.notify("WEATHER : Atmosphere");
  }
  else if (weatherString == "\"Clouds\"")
  {
    startframe = clouds;
    LINE.notify("WEATHER : Clouds");
  }

  // อ่านค่าอุณหภูมิจากมอดูล DHT11
  int t = (int)dht.readTemperature();
  // รับค่าตำเเหน่งสีพื้นหลัง
  temp_color = temp_color_selector(t);
  // เเสดงผล
  graphic.setBackground(temp_color_array[temp_color]);
  graphic.drawWithColor(number3x5_data[t / 10], 0xffeaeaea, 5, 3, 2, 2);
  graphic.drawWithColor(number3x5_data[t % 10], 0xffeaeaea, 5, 3, 6, 2);
  if ((millis() - lasttime) > 1000)
  {
    lasttime = millis();
    blink = !blink;
  }
  graphic.draw(blink ? 0xffeaeaea : temp_color_array[temp_color], 10, 2);
  graphic.drawWithColor(alphabet3x5_data[2], 0xffeaeaea, 5, 3, 12, 2);

  // เลือกอนิเมชันสัญลักษณ์เเทนสภาพอากาศตามค่า weather_status
  graphic.draw(weather_data[(frame % 3) + startframe], 8, 13, 2, 8);
  frame++;
  delay(250);
  graphic.display();

  
}

// ฟังก์ชันกำหนดตำเเหน่งค่าสีตามช่วงอุณหภูมิต่างๆ
uint8_t temp_color_selector(int temp)
{
  uint8_t tmp;
  if (temp > -20 && temp <= 10)
    tmp = 0;
  else if (temp > 10 && temp <= 20)
    tmp = 1;
  else if (temp > 20 && temp <= 29)
    tmp = 2;
  else if (temp > 29 && temp <= 35)
    tmp = 3;
  else if (temp > 35 && temp <= 40)
    tmp = 4;
  else
    tmp = 5;
  return tmp;
}

String httpGETRequest(const char *serverName)
{
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
