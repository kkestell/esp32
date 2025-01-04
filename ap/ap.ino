#include <WiFi.h>
#include <WebServer.h>

const char *ap_ssid = "ESP32";
const char *ap_password = "topsecret";

WebServer server(80);

void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ap_ssid, ap_password);

    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, []() { server.send(200, "text/plain", "Hello World!"); });

    server.begin();
}

void loop()
{
    server.handleClient();
}