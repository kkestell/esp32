// arduino-cli lib install "Adafruit NeoPixel"

#include <Adafruit_NeoPixel.h>

#define PIN 4
#define NUMPIXELS 60

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint16_t offset = 0;
uint32_t last_update = 0;

void setup()
{
    pixels.begin();
    pixels.setBrightness(50);
    pixels.clear();
    pixels.show();
}

void loop()
{
    if (millis() - last_update > 20)
    {
        for (int i = 0; i < NUMPIXELS; i++)
        {
            uint16_t hue = ((i * 65536 / NUMPIXELS) + offset) % 65536;
            uint32_t color = pixels.gamma32(pixels.ColorHSV(hue, 255, 255));
            pixels.setPixelColor(i, color);
        }

        pixels.show();
        offset += 200;
        last_update = millis();
    }
}