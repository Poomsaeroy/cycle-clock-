
#include "background.h"
#include "digit.h"
#include "drinkWT.h"
#include "component.h"
#include "graphic16x16.h"
#include "bedtime.h"
#include "pauseanimation.h"
#include <DHT.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <RTClib.h>
#include <string.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include <esp_task_wdt.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <TridentTD_LineNotify.h>

#define DHTPIN 23
#define DHTTYPE DHT11
#define WDT_TIMEOUT 18 //5

#define LINE_TOKEN  "aIjRsNSKTO5DUUS8L2Nxb3l3IFIKYaflSEtxMly20e8"

const char *ssid = "God_AP"; //Poomsaeroy
const char *password = "God123456";//0123456789

// const char *ssid = "CoEIoT";
// const char *password = "iot.coe.psu.ac.th";
RTC_DS1307 rtc;
int buzzerPin = 26;
int pirSensorPin = 39;
int soundSensorPin = 36;

DHT dht(DHTPIN, DHT11);
tmElements_t tm;
Graphic16x16 graphic;
HardwareSerial SerialPort(2);
unsigned int bright_LED = 20;
time_t sec_time;
uint8_t main_menu = 0;
uint8_t mode = 1;
unsigned long last_time, ignore_time;
uint8_t frame, behave;
uint8_t spec_behave, update_behave;
uint8_t background;
uint8_t digit_clock_color, bg_clock_color;
uint8_t calendar_mode;
uint8_t weather_status;

uint32_t Bcolor[] = {0x00000000};

void setup()
{
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.printf("WiFi connecting to %s\n",  ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  LINE.setToken(LINE_TOKEN);
  LINE.notify("connect to cycle clock!");


  //SDA SCL RTC
  //pin19  led เข
  //23 DHT11
  //14 16*16
  //27 old poten
  //26 buzzer
  //12 sw acknowledge
  //25 sw edit
  //2 sw set
  //34 swk2           
  //35 swk1           
  //33 sw k3          
  //32 sw k4         


  // กำหนดการทำงานของขาที่ใช้เป็นอินพุต
  pinMode(32, INPUT);  
  pinMode(33, INPUT);
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  pinMode(27, INPUT_PULLUP);
  pinMode(2, INPUT);
  pinMode(25, INPUT);
  pinMode(19, OUTPUT);
  pinMode(12, INPUT_PULLUP);
  pinMode(18, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pirSensorPin, INPUT);
  pinMode(soundSensorPin, INPUT);

  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // กำหนดค่าให้กับการเชื่อมต่อเเบบ Serial
  SerialPort.begin(115200, SERIAL_8N1, 16, 17);
  // กำหนด Seed ให้กับฟังก์ชัน random
  randomSeed((unsigned)time(&sec_time));
  // กำหนดค่าสถานะเริ่มต้นต่างๆเเละเปิดใช้งานไลบรารี่ให้กับโปรเเกรม
  graphic.setup();

  graphic.setBackground(background_data[0]);
  dht.begin();
  Wire.begin();
  // เริ่มต้นการใช้งานเเละอ่านค่าจาก EEPROM
  EEPROM.begin(512);
  uint8_t t = EEPROM.read(0);
  background = t >> 4;
  Serial.println(t, HEX);
  t = EEPROM.read(1);
  digit_clock_color = t & 0x0F;
  bg_clock_color = t >> 4;
  t = EEPROM.read(2);
  calendar_mode = t;
  // เริ่มต้นการใช้งานเเละกำหนดค่าให้ Watchdog
  esp_task_wdt_init(WDT_TIMEOUT, true); // WDT_TIMROUT = 5
  esp_task_wdt_add(NULL);
  // เริ่มเเสดงหน้าโหมด
  ignore_time = millis() + 10100;

}



void loop()
{
  // กำหนดค่าความสว่างให้กับ LED
  graphic.setBrightness(bright_LED);
  // เลือกการทำงานในโหมดต่างๆเเละเเสดงผล
  graphic.clear();
  if ((millis() - ignore_time) > 10000 || mode != 0)
  {
    esp_task_wdt_reset();
    switch (main_menu)
    {
    case 0:
      display_clock();
      break;
    case 1:
      display_calendar();
      break;
    case 2:
      display_work();
      break;
    case 3:
      display_temp();
      break;
    case 4:
      display_sleeptime();
      break;
    default:
      display_clock();
    }
  }
  else
  {
    esp_task_wdt_reset();
    if (mode == 0)
    {
      graphic.setBackground(Bcolor[0]);
      graphic.draw(menu_icon_data[main_menu], 0, 0);
    }
    graphic.display();
  }

  // อ่านค่าจากปุ่มกด 4 ปุ่ม
  if ((millis() - last_time) > 150)
  {
    last_time = millis();
    if (mode == 0)
    {
      if (!digitalRead(33))
      {
        if (mode == 0)
        {
          main_menu < 4 ? main_menu++ : main_menu = 0;
          ignore_time = millis();
          while (!digitalRead(33))
            ;
          Serial.println(main_menu);
        }
      }
      if (!digitalRead(34))
      {
        if (mode == 0)
        {
          main_menu > 0 ? main_menu-- : main_menu = 4;
          ignore_time = millis();
          while (!digitalRead(34))
            ;
          Serial.println(main_menu);
        }
      }
    }
    if (mode != 2)
    {
      if (!digitalRead(35))
      {
        if (mode != 0)
        {
          mode = 0;
          ignore_time = millis();
        }
        else if (mode == 0)
        {
          ignore_time += 10000;
          mode = 1;
          Serial.print("Current Page : ");
          Serial.println(main_menu);
        }
      }
      while (!digitalRead(35))
        ;
    }
  }
}
