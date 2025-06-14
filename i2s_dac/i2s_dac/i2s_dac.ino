#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

const char* ssid = "SSID";
const char* password = "PASSWORD";
const char* url = "https://github.com/rafaelreis-hotmart/Audio-Sample-files/raw/refs/heads/master/sample.mp3";

URLStream urlStream(ssid, password);
I2SStream i2s;
EncodedAudioStream decoder(&i2s, new MP3DecoderHelix());
StreamCopy copier(decoder, urlStream);

void setup() {
    Serial.begin(115200);
    AudioLogger::instance().begin(Serial, AudioLogger::Info);
    
    // Configure I2S for external DAC with larger buffers
    auto cfg = i2s.defaultConfig(TX_MODE);
    cfg.sample_rate = 44100;
    cfg.channels = 2;
    cfg.bits_per_sample = 16;
    cfg.buffer_size = 2048;  // Increase buffer size
    cfg.buffer_count = 8;    // More buffers
    
    // Set pins for your DAC connection
    cfg.pin_bck = 26;    // BCK
    cfg.pin_ws = 25;     // LCK (WS)
    cfg.pin_data = 22;   // DIN
    
    i2s.begin(cfg);
    
    // Configure URL stream with larger buffer
    urlStream.setReadBufferSize(8192);
    
    // Setup decoder based on sampling rate from MP3
    decoder.begin();
    
    // Start streaming from URL
    urlStream.begin(url, "audio/mp3");
}

void loop() {
    copier.copy();
}
