#include "SensorPublisherTask.hpp"
#include "DataBroker.hpp"
#include "SensorDataTypes.hpp"
#include "stm32h7xx_hal.h"
#include "task.h"
#include <cmath>
#include "input_data.hpp"
#include "gpsData.hpp"

#define PUBLISH_RATE_HZ 3

SensorPublisherTask::SensorPublisherTask() {}

void SensorPublisherTask::InitTask() {
    SOAR_ASSERT(rtTaskHandle == nullptr,
                "Cannot initialize SensorPublisherTask twice");

    BaseType_t rtValue = xTaskCreate(
        SensorPublisherTask::RunTask,
        "SensorPublisherTask",
        TASK_DEBUG_STACK_DEPTH_WORDS,
        nullptr,
        TASK_DEBUG_PRIORITY,
        &rtTaskHandle
    );

    SOAR_ASSERT(rtValue == pdPASS,
                "SensorPublisherTask::InitTask - xTaskCreate failed");
}

void SensorPublisherTask::Run(void *) {

    IMUData imu1{};
    IMUData imu2{};
    BaroData baro1{};
    BaroData baro2{};

    int taberIndex = 0;
    int baroIndex = 0;

    const size_t stationaryCycles = 2 * PUBLISH_RATE_HZ; // 20s stationary
    size_t stationaryCounter = 0;

    while (1) {

        // --- Stationary phase ---
        if (stationaryCounter < stationaryCycles) {

            imu1.accel.x = static_cast<int16_t>(0); // scale if needed
            imu1.accel.y = static_cast<int16_t>(0);
            imu1.accel.z = static_cast<int16_t>(1) * 1000.0f;
            imu1.gyro.x  = static_cast<int16_t>(0);
            imu1.gyro.y  = static_cast<int16_t>(0);
            imu1.gyro.z  = static_cast<int16_t>(0);
            imu1.id      = 0;

            imu2.accel.x = static_cast<int16_t>(0); // scale if needed
            imu2.accel.y = static_cast<int16_t>(0);
            imu2.accel.z = static_cast<int16_t>(1) * 1000.0f;
            imu2.gyro.x  = static_cast<int16_t>(0);
            imu2.gyro.y  = static_cast<int16_t>(0);
            imu2.gyro.z  = static_cast<int16_t>(0);
            imu2.id      = 1;

            baro1.pressure = static_cast<uint32_t>(baroData[0][1]) * 1000.0f;
            baro1.temp     = static_cast<int16_t>(baroData[0][2]);
            baro1.id       = 0;

            baro2.pressure = static_cast<uint32_t>(baroData[0][1]) * 1000.0f;
			baro2.temp     = static_cast<int16_t>(baroData[0][2]);
			baro2.id       = 1;

            DataBroker::Publish(&imu1);
            DataBroker::Publish(&imu2);
            DataBroker::Publish(&baro1);
            DataBroker::Publish(&baro2);

            stationaryCounter++;

        } else {
            // --- Normal playback ---
            imu1.accel.x = static_cast<int16_t>(taberLaunch[taberIndex][1] * 1000);
            imu1.accel.y = static_cast<int16_t>(taberLaunch[taberIndex][2] * 1000);
            imu1.accel.z = static_cast<int16_t>(taberLaunch[taberIndex][3] * 1000);
            imu1.gyro.x  = static_cast<int16_t>(taberLaunch[taberIndex][4] * 1000);
            imu1.gyro.y  = static_cast<int16_t>(taberLaunch[taberIndex][5] * 1000);
            imu1.gyro.z  = static_cast<int16_t>(taberLaunch[taberIndex][6] * 1000);
            imu1.temp    = 0; // dummy
            imu1.id      = 0;

            imu2.accel.x = static_cast<int16_t>(taberLaunch[taberIndex][1] * 1000);
            imu2.accel.y = static_cast<int16_t>(taberLaunch[taberIndex][2] * 1000);
            imu2.accel.z = static_cast<int16_t>(taberLaunch[taberIndex][3] * 1000);
            imu2.gyro.x  = static_cast<int16_t>(taberLaunch[taberIndex][4] * 1000);
            imu2.gyro.y  = static_cast<int16_t>(taberLaunch[taberIndex][5] * 1000);
            imu2.gyro.z  = static_cast<int16_t>(taberLaunch[taberIndex][6] * 1000);
            imu2.temp    = 0; // dummy
            imu2.id      = 1;

            baro1.pressure = static_cast<uint32_t>(baroData[baroIndex][1] * 1000);
            baro1.temp     = static_cast<int16_t>(baroData[baroIndex][2]);
            baro1.id       = 0;

            baro2.pressure = static_cast<uint32_t>(baroData[baroIndex][1] * 1000);
            baro2.temp     = static_cast<int16_t>(baroData[baroIndex][2]);
            baro2.id       = 1;

            //DataBroker::Publish(&imu1);
            DataBroker::Publish(&baro1);
            //DataBroker::Publish(&imu2);
            DataBroker::Publish(&baro2);

            taberIndex++;
            baroIndex++;

            if (taberIndex >= taberLaunch.size()) taberIndex = 0;
            if (baroIndex >= baroData.size()) baroIndex = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(1000 / PUBLISH_RATE_HZ));
    }
}
