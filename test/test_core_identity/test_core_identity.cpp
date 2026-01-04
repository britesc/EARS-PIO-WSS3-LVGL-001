/**
 * @file test_core_identity.cpp
 * @author Julian (51fiftyone51fiftyone@gmail.com)
 * @brief Test File for which Core in use.
 * @section tests Tests
 * - Unit Tests for Functionality.
 * @version 0.1
 * @date 20260104
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include <Arduino.h>
#include <unity.h>

static volatile int core0_seen = -1;
static volatile int core1_seen = -1;

/*
  Task that runs on Core 0
*/
void core0_task(void *param)
{
    core0_seen = xPortGetCoreID();
    Serial.printf("Core0 task running on core: %d\n", core0_seen);
    vTaskDelete(NULL);
}

/*
  Task that runs on Core 1
*/
void core1_task(void *param)
{
    core1_seen = xPortGetCoreID();
    Serial.printf("Core1 task running on core: %d\n", core1_seen);
    vTaskDelete(NULL);
}

void test_core_identity(void)
{
    Serial.begin(115200);

    // Wait for USB CDC
    uint32_t start = millis();
    while (!Serial && millis() - start < 5000)
        delay(10);

    // Create one task on each core
    xTaskCreatePinnedToCore(
        core0_task,
        "core0_task",
        2048,
        NULL,
        1,
        NULL,
        0   // Core 0
    );

    xTaskCreatePinnedToCore(
        core1_task,
        "core1_task",
        2048,
        NULL,
        1,
        NULL,
        1   // Core 1
    );

    // Give tasks time to run
    delay(500);

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, core0_seen, "Task pinned to Core0 ran on wrong core");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, core1_seen, "Task pinned to Core1 ran on wrong core");
}

void setup()
{
    delay(1000);
    UNITY_BEGIN();
    RUN_TEST(test_core_identity);
    UNITY_END();
}

void loop()
{
}
