#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <HardwareSerial.h>

#include <ArduinoJson.h>

#include "configure_gun.h"


ESP8266WebServer server(80);
IPAddress apIP(192, 168, 4, 1);
const char* assid = "no inet()";

const char* ssid = "TylerDurden";
const char* password =  "dapizdato5";
uint8_t tries = 30;  // Попыткок подключения к точке доступа



void handleRootPath() {
    server.send(200, "text/plain", "HI");
}

void add_config(){
    String post_body = server.arg("plain");
    MT2_Configure gun_settings_blob;
    if (JsonToConfig(post_body, gun_settings_blob)) {
        server.send(400, F("text/plain"), "Error");
    }else{
        server.send(200, F("text/plain"), "OK");
    }
}


void get_hits(){
    server.send(200, "text/plain", "NO_HITS");
}


void wifi_init(){
    WiFi.mode(WIFI_AP_STA);
    // create acces point    
    WiFi.softAPConfig(apIP, apIP, IPAddress(255,255,255,0));
    WiFi.softAP(assid);
    Serial.print("Adres: "); Serial.println(WiFi.softAPIP());

    delay(1500);
    // connect to wifi
    WiFi.begin(ssid, password);
    while (--tries && WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println(F("\nNon Connecting to WiFi"));
    } else {
        Serial.println(F("\nWiFi connected"));
        Serial.print(F("IP address: ")); Serial.println(WiFi.localIP());
    }
}


void url_path_connection(){
    server.onNotFound([]() { server.send(404, "test/plain", "No Found"); });
    server.on("/", handleRootPath);
    server.on("/load_conf", HTTP_POST, add_config);
    server.on("/get_hits", HTTP_DELETE, get_hits);
}



void setup() {
    Serial.begin(115200);
    delay(500);

    wifi_init();
    url_path_connection();

    Serial.flush();

    server.begin();
}


void loop() {
    server.handleClient();
}

