TaskHandle_t Task1;
TaskHandle_t Task2;

const int led1 = 2;
const int led2 = 4;

void setup()
{
    Serial.begin(115200);

    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);

    xTaskCreatePinnedToCore(
        Task1code, // Function to implement the task
        "Task1",   // Name of the task
        10000,     // Stack size in words
        NULL,      // Task input parameter
        1,         // Priority of the task
        &Task1,    // Task handle
        0);        // Core where the task should run

    // Task2 runs on core 1
    xTaskCreatePinnedToCore(
        Task2code, // Function to implement the task
        "Task2",   // Name of the task
        10000,     // Stack size in words
        NULL,      // Task input parameter
        1,         // Priority of the task
        &Task2,    // Task handle
        1);        // Core where the task should run
}

void Task1code(void *parameter)
{
    Serial.print("Task1 running on core ");
    Serial.println(xPortGetCoreID());

    for (;;)
    {
        digitalWrite(led1, HIGH);
        delay(1000);
        digitalWrite(led1, LOW);
        delay(1000);
        Serial.println("Task 1: Blink LED 1");
    }
}

void Task2code(void *parameter)
{
    Serial.print("Task2 running on core ");
    Serial.println(xPortGetCoreID());

    for (;;)
    {
        digitalWrite(led2, HIGH);
        delay(500);
        digitalWrite(led2, LOW);
        delay(500);
        Serial.println("Task 2: Blink LED 2");
    }
}

void loop()
{
    delay(1000);
}