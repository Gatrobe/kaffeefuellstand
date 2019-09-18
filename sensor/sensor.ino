    #include "./config.h"

    Adafruit_NeoPixel line = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

    ESP8266WebServer server(80);

    float cups = MAX_CUPS;

    unsigned long now = 0;
    unsigned long last_change = 0;

    void setup()
    {
      Serial.begin(SERIAL_BAUD);

      line.begin();
      line.clear();
      line.show();
      Serial.println("");

      connect_to_wifi();

      //OTA SETUP
      ArduinoOTA.setPort(OTA_PORT);
      ArduinoOTA.setHostname(NAME);
      ArduinoOTA.setPassword((const char *)OTA_PASSWORD);

      ArduinoOTA.onStart([]() {
        Serial.println("Starting");
      });
      ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
      });
      ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      });
      ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
      });
      ArduinoOTA.begin();

      server.on("/remaining_cups", [](){
        cups = get_remaining_cups(get_scale_voltage());
        server.send(200, "text/plain", (((String)cups) + "\n"));
        Serial.println(cups);
        set_led_bar(cups, 2);
        delay(1000);
      });
      server.on("/raw", [](){
        server.send(200, "text/plain", (((String)get_scale_voltage()) + "\n"));
        delay(1000);
      });

      server.begin();

      Serial.println("Setup Done!");
    }

    void loop()
    {
      server.handleClient(); //do webserver stuff

      now = millis();

      if (WiFi.status() != WL_CONNECTED) //reconnect if wifi lost
        connect_to_wifi();

      if (now - last_change > TIMER_S*1000)//update LED bar from time to time
      {
        if (raw_adc() >= ADC_PUMP);
        else
        {
          cups = get_remaining_cups(get_scale_voltage());
          set_led_bar(cups, 2);
          last_change = now;
          delay(TIMER_S*10);
        }
      }

      if (raw_adc() >= ADC_PUMP)
      {
        //show remaining cups on LED
        set_led_bar(cups, 1);
      }
      else if ((get_scale_voltage() <= ADC_EMPTY) && (get_scale_voltage() > ADC_GONE))//red light if coffee is empty
      {
        for (int i = 0; i < LED_COUNT; i++)
          line.setPixelColor(i, line.Color(255/8, 0, 0));
        line.show();
      }
      else
      {
       //line.clear();
       //line.show();
       set_led_bar(cups, 2);
     }

    }


    //#########################################################################
    //Functions

    void connect_to_wifi()//(re)connects to wifi and does fancy LED stuff
    {
      WiFi.disconnect();
      WiFi.config(ip, dns, gateway, subnet);
      WiFi.begin(WIFI_SSID, WIFI_PASS);
      while (WiFi.status() != WL_CONNECTED)
      {
        for(int i=0; i<LED_COUNT; i++)
        {
          line.setPixelColor(i, line.Color(0, 0, 255/LED_BRIGHTNESS_DIV));
          line.show();
          delay(250/LED_COUNT);
        }
        for(int i=0; i<LED_COUNT; i++)
        {
          line.setPixelColor(i, line.Color(0,0,0));
          line.show();
          delay(250/LED_COUNT);
        }
      }
      Serial.println(WiFi.localIP());
      line.clear();
      line.show();
    }

    int raw_adc()//because Im lazy
    {
      return analogRead(SCALE_PIN);
    }

    void set_led_bar(float _cups, int div)//does Rainboweffect on LED strip
    {

      int end_led = (int) ((_cups/MAX_CUPS) * LED_COUNT);

      for (int i = 0; i < end_led; i++)
      {
        int red = (int) (255 - 255 * ((float) i / LED_COUNT));
        int green = (int) (255 * ((float) i / LED_COUNT));

        line.setPixelColor(i, line.Color(red/(LED_BRIGHTNESS_DIV*div), green/(LED_BRIGHTNESS_DIV*div), 0));
      }
      for (int i = end_led; i < LED_COUNT; i++)
      {
        line.setPixelColor(i, line.Color(0, 0, 0));
      }
      line.show();
    }

    //############################################################
    //functions to get correct amount of Cups

    int get_scale_voltage()
    {
      // We take 11 values because the median point is at index 6.
      int measured_values[11];
      for (uint8_t index=0; index < 10; index++) {
        measured_values[index] = raw_adc();
        delay(1);
      }
      // Calculate the array length.
      int array_length = sizeof(measured_values) / sizeof(measured_values[0]);
      // Sort the array.
      qsort(measured_values, array_length, sizeof(measured_values[0]), sort_asc);
      // Get the median value.
      //Serial.println(measured_values[5]);
      return measured_values[5];
    }

    int sort_asc(const void *cmp1, const void *cmp2)
    {
      // Need to cast the void * to int *
      int a = *((int *)cmp1);
      int b = *((int *)cmp2);
      // The comparison
      return a > b ? 1 : (a < b ? -1 : 0);
    }

    float get_remaining_cups(int voltage) {
      // Init remaining cups with the maximum number of cups.
      float remaining_cups = MAX_CUPS;
      // Iterate over the voltage array and compare the values.
      for (int i = 0; i < sizeof(voltages); i++)
      {
        if (voltage >= voltages[i]) {
          remaining_cups = a_cups[i];
          return remaining_cups;
        }
      }
      // The coffeepot is gone...
      return -1.0;
    }
