/*
See: https://github.com/pschatzmann/ESP32-A2DP?tab=readme-ov-file#installation
*/


#include "BluetoothA2DPSource.h"
#include <math.h> 

#define SAMPLE_RATE 44100
#define PI 3.14159265358979323846
#define FREQUENCY 440.0  // A4
#define AMPLITUDE 32767  // Maximum amplitude for 16-bit audio

BluetoothA2DPSource source;

static double phase = 0.0;

int32_t getSoundData(Frame *data, int32_t frameCount) {
    // Since Frame is a raw buffer of PCM data, we'll treat it as 16-bit samples
    int16_t *samples = (int16_t*)data;
    
    // Each frame has 2 channels, so we need to fill 2 samples per frame
    for (int32_t i = 0; i < frameCount * 2; i += 2) {
        // Calculate sine value for current phase
        int16_t sample = (int16_t)(AMPLITUDE * sin(phase));
        
        // Fill both channels with the same value
        samples[i] = sample;     // Left channel
        samples[i + 1] = sample; // Right channel
        
        // Increment phase for next sample
        phase += 2.0 * PI * FREQUENCY / SAMPLE_RATE;
        
        // Prevent floating point errors over time
        if (phase >= 2.0 * PI) {
            phase -= 2.0 * PI;
        }
    }
    
    return frameCount;
}

void setup() {
    source.start("Anker SoundCore", getSoundData);  
}

void loop() {
}
