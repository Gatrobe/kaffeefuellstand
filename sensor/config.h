    #ifndef CONFIG_H
    #define CONFIG_H

    #include <ESP8266WiFi.h>
    #include <Adafruit_NeoPixel.h>
    #include <WiFiClient.h>
    #include <ESP8266WebServer.h>
    #include <ArduinoOTA.h>
    #include "wifi.h"

    //Device Name
    #define NAME "CoffeePot"

    //Main-Loop Time-Out
    #define TIMER_S 1 //15

    //LED Strip
    #define LED_COUNT 22
    #define LED_PIN D5
    #define LED_BRIGHTNESS_DIV 8

    //Scale
    #define SCALE_PIN A0

    //WiFi Settings
    IPAddress ip(172, 24, 0, 12);
    IPAddress gateway(172, 24, 0, 4);
    IPAddress subnet(255, 255, 0, 0);
    IPAddress dns(172, 24, 0, 4);

    //MQTT Settings
    const char* MQTT_SERVER = "000.000.000.000";
    const char* MQTT_USER = "MosquittoUser1234";
    const char* MQTT_PASS = "YouWillNeverGuess";

    const char* MQTT_STATE_TOPIC = "coffeepot/state";

    //OTA Settings
    #define OTA_PASSWORD "WIFI_PASSWORD"
    #define OTA_PORT 8266

    //Serial Settings
    #define SERIAL_BAUD 115200

    //Cups
    const float MAX_CUPS = 6.0;
    #define ADC_PUMP 670
    #define ADC_EMPTY 350
    #define ADC_FULL 660
    #define ADC_GONE 250

    // Maximum voltage that is still considered a measured value and not an outlier.
    #define THRESHOLD_VOLTAGE 660//650
    /*
    int voltages[] = {
     616, // 6.00 Cups 586
     614, // 5.66 Cups 565
     605, // 5.33 Cups 550
     600, // 5.00 Cups 544
     588, // 4.66 Cups 525
     580, // 4.33 Cups 506
     569, // 4.00 Cups 489
     559, // 3.66 Cups 474
     550, // 3.33 Cups 433
     543, // 3.00 Cups 406
     530, // 2.66 Cups 393
     521, // 2.33 Cups
     514, // 2.0 Cups
     502, // 1.66 Cups
     493, // 1.33 Cups
     485, // 1.00 Cups
     470, // 0.66 Cups
     451  // 0.33 Cups
    };*/
    /*
    int voltages[] = {
      586,
      575,
      565,
      550,
      544,
      538,
      525,
      513,
      506,
      495,
      489,
      474,
      465,
      456,
      441,
      433,
      417,
      406
    };
    */
    int voltages[] = {
      586,
      575,
      565,
      550,
      544,
      541,
      538,
      533,
      528,
      523,
      517,
      512,
      507,
      502,
      493,
      485,
      470,
      433	//451
    };

    float a_cups[] = {
      6.0,
      5.6,
      5.3,
      5.0,
      4.6,
      4.3,
      4.0,
      3.6,
      3.3,
      3.0,
      2.6,
      2.3,
      2.0,
      1.6,
      1.3,
      1.0,
      0.6,
      0.3
    };

    #endif
