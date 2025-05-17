#include <Arduino.h>

TaskHandle_t onTask;
TaskHandle_t offTask;
SemaphoreHandle_t ledSemaphore;

#define LED_BUILTIN 8

void setup() {
   pinMode(LED_BUILTIN, OUTPUT);
   digitalWrite(LED_BUILTIN, HIGH);
   
   ledSemaphore = xSemaphoreCreateBinary();
   
   xTaskCreate(
       turnOnLED,      // Function to implement the task
       "OnTask",       // Name of the task
       2048,           // Stack size in words
       NULL,           // Task input parameter
       1,              // Priority of the task
       &onTask         // Task handle
   );

   xTaskCreate(
       turnOffLED,     // Function to implement the task
       "OffTask",      // Name of the task
       2048,           // Stack size in words
       NULL,           // Task input parameter
       1,              // Priority of the task
       &offTask        // Task handle
   );
   
   xSemaphoreGive(ledSemaphore);
}

void turnOnLED(void *parameter) {
   for(;;) {
       xSemaphoreTake(ledSemaphore, portMAX_DELAY);
       digitalWrite(LED_BUILTIN, LOW);
       vTaskDelay(pdMS_TO_TICKS(1000));
       xSemaphoreGive(ledSemaphore);
       vTaskDelay(pdMS_TO_TICKS(10));
   }
}

void turnOffLED(void *parameter) {
   for(;;) {
       xSemaphoreTake(ledSemaphore, portMAX_DELAY);
       digitalWrite(LED_BUILTIN, HIGH);
       vTaskDelay(pdMS_TO_TICKS(1000));
       xSemaphoreGive(ledSemaphore);
       vTaskDelay(pdMS_TO_TICKS(10));
   }
}

void loop() {
   vTaskDelay(pdMS_TO_TICKS(1000));
}