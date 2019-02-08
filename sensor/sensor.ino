#define SENSOR A0

#define KANNE_LEER 410
#define KANNE_VOLL 605
#define KANNE_05 590
#define KANNE_1 583
#define KANNE_15 578
#define KANNE_2 554
#define KANNE_25 539
#define KANNE_3 525
#define KANNE_35 510
#define KANNE_4 496
#define KANNE_45 481
#define KANNE_5 468
#define KANNE_55 453
#define KANNE_6 439
#define KANNE_65 424
#define KANNE_WEG 200

#include "wifi_password.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
String page = "";

void setup() {
  //Serial.begin(115200);
  
  page = "<h1>Kaffee</h1><p><a href=\"kaffee\"><button>Fuellstand abfragen</button></a>&nbsp;</p>";
  connect_to_wifi();
 // WiFi.begin(ssid, password);
 // while (WiFi.status() != WL_CONNECTED) {
  //  delay(500);
   // Serial.print(".");
 // }
 // Serial.println("");
  //Serial.print("Connected to ");
  //Serial.println(ssid);
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());

  server.on("/", [](){
    server.send(200, "text/html", page);
  });
  server.on("/kaffee", [](){
    get_kaffe_simple();
    delay(1000);
  });
  server.on("/remaining_cups", [](){
    get_cups();
    delay(1000);
  });
  server.on("/raw", [](){
    get_raw();
    delay(1000);
  });

  server.begin();
  //Serial.println("Web server started!");
}

void loop()
{
  server.handleClient();

  if (WiFi.status() != WL_CONNECTED)
    connect_to_wifi();
}

int get_adc()
{
  int adc_val_sum = 0;
  int adc_val2_sum = 0;
  for (int j=0; j<10;j++)
  {
    adc_val_sum = 0;
    for (int i=0; i<100; i++)
    {
     int adc_val = analogRead(SENSOR);
     adc_val_sum += adc_val;
     //Serial.print("ADC ");
     //Serial.print(i+1);
     //Serial.print(": ");
     //Serial.println(adc_val);
     delay(1);
    }
    int adc_val_mean = adc_val_sum / 100;
    adc_val2_sum += adc_val_mean;
    //Serial.print("Mean of last 100 ADC Values: ");
    //Serial.println(adc_val_mean);
  }
  int adc_val2_mean = adc_val2_sum / 10;
  //Serial.print("Mean of the last 10 mean Values: ");
  //Serial.println(adc_val2_mean);

  if (adc_val2_mean <= 620)
    return adc_val2_mean;
  else
    get_adc();
}

int get_adc_raw()
{
  int adc_val_sum = 0;
  int adc_val2_sum = 0;
  for (int j=0; j<10;j++)
  {
    adc_val_sum = 0;
    for (int i=0; i<100; i++)
    {
     int adc_val = analogRead(SENSOR);
     adc_val_sum += adc_val;
     //Serial.print("ADC ");
     //Serial.print(i+1);
     //Serial.print(": ");
     //Serial.println(adc_val);
     delay(1);
    }
    int adc_val_mean = adc_val_sum / 100;
    adc_val2_sum += adc_val_mean;
    //Serial.print("Mean of last 100 ADC Values: ");
    //Serial.println(adc_val_mean);
  }
  int adc_val2_mean = adc_val2_sum / 10;
  //Serial.print("Mean of the last 10 mean Values: ");
  //Serial.println(adc_val2_mean);

  return adc_val2_mean;
}

void get_kaffe_simple()
{
  int val = get_adc();

   if (val >= KANNE_VOLL)
   {
      server.send(200, "text/html", "<h1>Kaffee ist voll.</h1><p><a href=\"kaffee\"><button>Fuellstand</button></a>&nbsp;</p>");
   }
   else if ((val > KANNE_4) && (val < KANNE_VOLL))
   {
      server.send(200, "text/html", "<h1>Kaffee ist etwa halb voll.</h1><p><a href=\"kaffee\"><button>Fuellstand</button></a>&nbsp;</p>");
   }
   else if ((val > KANNE_LEER) && (val < KANNE_4))
   {
      server.send(200, "text/html", "<h1>Kaffee ist bald leer.</h1><p><a href=\"kaffee\"><button>Fuellstand</button></a>&nbsp;</p>");
   }
   else if ((val <= KANNE_LEER) && (val > KANNE_WEG))
   {
      server.send(200, "text/html", "<h1>Kaffee ist leer.</h1><p><a href=\"kaffee\"><button>Fuellstand</button></a>&nbsp;</p>");
   }
   else if (val <= KANNE_WEG)
   {
      server.send(503, "text/html", "<h1>Coffeepot not found</h1><p><a href=\"kaffee\"><button>Fuellstand</button></a>&nbsp;</p>");
   }
   return;
}

void get_cups()
{
  int val = get_adc();

  if (val <= KANNE_WEG)
  {
    server.send(503, "text/plain", "Coffeepot not found\n");
  }
  else
  {
    if (val >= KANNE_VOLL)
    {
      server.send(200, "text/plain", "7.0\n");
    }
    else if (val >= KANNE_05)
    {
      server.send(200, "text/plain", "6.5\n");
    }
    else if (val >= KANNE_1)
    {
      server.send(200, "text/plain", "6.0\n");
    }
    else if (val >= KANNE_15)
    {
      server.send(200, "text/plain", "5.5\n");
    }
    else if (val >= KANNE_2)
    {
      server.send(200, "text/plain", "5.0\n");
    }
    else if (val >= KANNE_25)
    {
      server.send(200, "text/plain", "4.5\n");
    }
    else if (val >= KANNE_3)
    {
      server.send(200, "text/plain", "4.0\n");
    }
    else if (val >= KANNE_35)
    {
      server.send(200, "text/plain", "3.5\n");
    }
    else if (val >= KANNE_4)
    {
      server.send(200, "text/plain", "3.0\n");
    }
    else if (val >= KANNE_45)
    {
      server.send(200, "text/plain", "2.5\n");
    }
    else if (val >= KANNE_5)
    {
      server.send(200, "text/plain", "2.0\n");
    }
    else if (val >= KANNE_55)
    {
      server.send(200, "text/plain", "1.5\n");
    }
    else if (val >= KANNE_6)
    {
      server.send(200, "text/plain", "1.0\n");
    }
    else if (val >= KANNE_65)
    {
      server.send(200, "text/plain", "0.5\n");
    }
    else
    {
      server.send(200, "text/plain", "0.0\n");
    }
  }
}

void get_raw()
{
  server.send(200, "text/plain", (((String)get_adc_raw()) + "\n"));
}

void connect_to_wifi()
{
  WiFi.disconnect();
  WiFi.begin(WIFI_NAME, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
}
