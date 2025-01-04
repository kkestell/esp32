#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

const char *ap_ssid = "ESP32";
const char *ap_password = "topsecret";

WebServer server(80);

void handleRoot()
{
    String html = "<html><body>";

    html += "<form action='/create' method='post'>";
    html += "New file: <input type='text' name='filename'>";
    html += "<input type='submit' value='Create'>";
    html += "</form><hr>";

    File root = LittleFS.open("/");
    File file = root.openNextFile();
    while (file)
    {
        String fileName = file.name();
        html += "<a href='/edit?file=" + fileName + "'>" + fileName + "</a><br>";
        file = root.openNextFile();
    }
    root.close();

    html += "</body></html>";
    server.send(200, "text/html", html);
}

void handleEdit()
{
    String fileName = server.arg("file");

    File file = LittleFS.open("/" + fileName, "r");
    if (!file)
    {
        server.send(404, "text/plain", "File not found");
        return;
    }

    String content = "";
    while (file.available())
    {
        content += (char)file.read();
    }
    file.close();

    String html = "<html><body>";
    html += "<h3>" + fileName + "</h3>";
    html += "<form action='/save' method='post'>";
    html += "<textarea name='content' rows='20' cols='80'>" + content + "</textarea><br>";
    html += "<input type='hidden' name='filename' value='" + fileName + "'>";
    html += "<input type='submit' value='Save'>";
    html += "</form>";
    html += "<a href='/'>Back to file list</a>";
    html += "</body></html>";

    server.send(200, "text/html", html);
}

void handleCreate()
{
    String fileName = server.arg("filename");
    if (fileName.length() > 0)
    {
        File file = LittleFS.open("/" + fileName, "w");
        file.close();
    }
    server.sendHeader("Location", "/edit?file=" + fileName);
    server.send(303);
}

void handleSave()
{
    String fileName = server.arg("filename");
    String content = server.arg("content");

    File file = LittleFS.open("/" + fileName, "w");
    if (!file)
    {
        server.send(500, "text/plain", "Save failed");
        return;
    }

    file.print(content);
    file.close();

    server.sendHeader("Location", "/");
    server.send(303);
}

void setup()
{
    Serial.begin(115200);

    if (!LittleFS.begin(true))
    {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ap_ssid, ap_password);

    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, handleRoot);
    server.on("/edit", HTTP_GET, handleEdit);
    server.on("/create", HTTP_POST, handleCreate);
    server.on("/save", HTTP_POST, handleSave);

    server.begin();
    Serial.println("HTTP server started");
}

void loop()
{
    server.handleClient();
}