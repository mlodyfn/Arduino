// ############## LCD ##############
#include <LiquidCrystal_I2C.h>
#define LCD_Lines 2
#define LCD_i2c_ADDR 0x20
#define LCD_Chars 16

// ##############  wifi ##############
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#ifndef STASSID
#define STASSID "szatan"
#define STAPSK  "dupadupa666"
#endif
const char * ssid = STASSID; // your network SSID (name)
const char * pass = STAPSK;  // your network password
unsigned int localPort = 2390;      // local port to listen for UDP packets

// ##############  NTP  ##############
//IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;



LiquidCrystal_I2C lcd(LCD_i2c_ADDR,LCD_Chars,LCD_Lines);
void InitSerial();
void InitLCD();
void I2CScanner();
void InitWifi();
//void MSG(String msg,int LCD_Line = 0, int LCD_row = 0,bool LCD_Clear = false);



void setup() {
  InitSerial();
  InitLCD();
  delay(2000);
  InitWifi();
////  I2CScanner();


}

void loop() {
  // put your main code here, to run repeatedly:
//  MSG("a", 0, 1, true);
}


void I2CScanner()
{
  Serial.println("i2c scanner started");
  int nDevices = 0;
  byte error;

  for (int addr = 0; addr < 127 ; addr++)
  {
    Wire.begin();
    Wire.beginTransmission(addr);
    error = Wire.endTransmission();

    if (error == 0)
    {
      nDevices++;
      Serial.print("Found device on i2c: 0x");
      if (addr < 16)
      {
        Serial.print(addr);
      }
      else
      {
        Serial.print(addr,HEX);  
        Serial.println("");
      }
    }
  }
  String message = "All devices ddfound: " + String(nDevices);
  Serial.print(message);
}

void InitSerial()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Serial.Started");
}


void MSG(String msg,int LCD_Line , int LCD_row ,bool LCD_Clear)
{
  
  String message = "LCD and Serial message print:";
  String fullMsg = message + "'" +  msg + "'";
  Serial.println(fullMsg);
  int len = msg.length();
  if (LCD_Clear)
  {
    Serial.println("LCD clear");
    lcd.clear();
  }
  lcd.setCursor(LCD_Line,LCD_row);
  lcd.print(msg);
  if (len > 16)
    {
      lcd.noAutoscroll();
      delay(1500);
      Serial.println("WiekszE");
      for (int i=0 ; i<= len - 6 ; i++)
      {        
        lcd.scrollDisplayLeft();
        delay(600);
        Serial.println(String(i));
        
      }
      lcd.setCursor(0,0);
      lcd.home();
      lcd.print(msg);
    }
   else
   {
      Serial.println("mniejsze"); 
   }  
  
//  lcd.noAutoscroll();
//  lcd.scrollDisplayLeft();

  
}

void InitLCD()
{
  Serial.println("Lets start LCD");
  lcd.init(); // initialize the lcd
  Serial.println("LCD clear");
  lcd.clear();
  Serial.println("LCD set backlight");
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("-= RainDriver =-");
  lcd.setCursor(4,1);
  lcd.print("Started");
}


void InitWifi(){
  
  String message = "Connecting SSID: " + String(STASSID); // + " Password: " +String(STAPSK);
  MSG(message, 0, 0, true);
  Serial.println("Wifi password : " + String(STAPSK));
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println(WiFi.status());
//  Serial.println("Connecting to wifi: " + String(STASSID));
//  Serial.println("Connecting with password: " + String(STAPSK));
}
