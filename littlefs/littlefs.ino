#include <LittleFS.h>

void setup()
{
    Serial.begin(115200);

    if (!LittleFS.begin(true))
    {
        Serial.println("Mount failed");
        return;
    }

    Serial.println("Deleting all files...");
    File root = LittleFS.open("/");
    File file = root.openNextFile();
    while (file)
    {
        String fileName = file.name();
        file.close();
        Serial.print("Deleting: ");
        Serial.println(fileName);
        LittleFS.remove("/" + fileName);
        file = root.openNextFile();
    }
    root.close();

    Serial.println("Creating foo.txt");
    file = LittleFS.open("/foo.txt", "w");
    if (!file)
    {
        Serial.println("Failed to create foo.txt");
        return;
    }
    file.println("Hello World");
    file.close();

    Serial.println("\nReading foo.txt:");
    file = LittleFS.open("/foo.txt", "r");
    if (!file)
    {
        Serial.println("Failed to open foo.txt for reading");
        return;
    }

    while (file.available())
    {
        Serial.write(file.read());
    }
    file.close();
}

void loop()
{
    delay(1000);
}