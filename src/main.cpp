#include <ESP8266wifi.h>
#include "oled.h"
#include <Wire.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

String URL = "http://airbuild.azurewebsites.net/Update";

// WiFi settings
const char *ssid = "MSFTGUEST";

void setup()
{
    Serial.begin(9600);
    oled_setup();
    Serial.println("ESP8266 starting in normal mode");
    // Connect to WiFi
    oled_clear();
    oled_println("Wifi Connecting..");

    WiFi.begin(ssid); //, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    oled_println("Wifi connected");
    // Print the IP address
    Serial.println(WiFi.localIP());
}

void checkUpdate()
{
    HTTPClient http;
    http.begin(URL);
    http.addHeader("HTTP_X_ESP8266_VERSION", "001");

    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0)
    {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            Serial.println(payload);
            oled_clear();
            oled_println(payload);
        }
    }
    else
    {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
}

void loop()
{
    HTTPUpdateResult ret = ESPhttpUpdate.update("http://airbuild.azurewebsites.net/Update", "002");
    
    switch(ret) {
        case HTTP_UPDATE_FAILED:
            Serial.println("[update] Update failed.");
            break;
        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("[update] Update no Update.");
            break;
        case HTTP_UPDATE_OK:
            Serial.println("[update] Update ok."); // may not called we reboot the ESP
            break;
    }

    delay(2000);
}