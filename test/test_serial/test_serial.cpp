/**
 * @file test_serial.cpp
 * @author Julian (51fiftyone51fiftyone@gmail.com)
 * @brief Test File to confirm Serial Working.
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

/*
  ESP32-S3 USB CDC Serial validation
*/

void wait_for_serial()
{
    uint32_t start = millis();
    while (!Serial && (millis() - start < 5000))
    {
        delay(10);
    }
}

void test_serial_enumeration(void)
{
    Serial.begin(115200);
    wait_for_serial();

    TEST_ASSERT_TRUE_MESSAGE(Serial, "USB CDC Serial not enumerated");
}

void test_serial_write(void)
{
    size_t bytes = Serial.println("ESP32-S3 SERIAL UNIT TEST OK");
    TEST_ASSERT_GREATER_THAN(0, bytes);
}

void setup()
{
    delay(1000);   // USB settle
    UNITY_BEGIN();

    RUN_TEST(test_serial_enumeration);
    RUN_TEST(test_serial_write);

    UNITY_END();
}

void loop()
{
}
