/*
 * LoggingTask.cpp
 *
 *  Created on: Jan 23, 2026
 *      Author: jaddina
 */


/**
 ********************************************************************************
 * @file    LoggingTask.cpp
 * @author  jaddina
 * @date    Sep 13, 2025
 * @brief
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "LoggingTask.hpp"
#include "SystemDefines.hpp"
#include "Command.hpp"
#include "LoggingService.hpp"

#include "DataBroker.hpp"
#include "Task.hpp"
#include "DebugTask.hpp"

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * VARIABLES
 ************************************/

/************************************
 * FUNCTION DECLARATIONS
 ************************************/


uint8_t LoggingTask::buf[20] = {0};
bool LoggingTask::highAltitude = false;
bool LoggingTask::lowAltitude = false;

//bool LoggingTask::firstAlt = false;

float firstAlt = 0.0f;
bool firstAltCaptured = false;
bool launched = false;
bool landed = false;



/************************************
 * FUNCTION DEFINITIONS
 ************************************/
LoggingTask::LoggingTask():Task(TASK_LOGGING_QUEUE_DEPTH_OBJS)
{

}



/**
 * @brief Initialize the LoggingTask
 *        Do not modify this function aside from adding the task name
 */
void LoggingTask::InitTask()
{
    // Make sure the task is not already initialized
    SOAR_ASSERT(rtTaskHandle == nullptr, "Cannot initialize watchdog task twice");

    BaseType_t rtValue =
        xTaskCreate((TaskFunction_t)LoggingTask::RunTask,
            (const char*)"LoggingTask",
            (uint16_t)TASK_LOGGING_QUEUE_DEPTH_WORDS,
            (void*)this,
            (UBaseType_t)TASK_LOGGING_PRIORITY,
            (TaskHandle_t*)&rtTaskHandle);

                SOAR_ASSERT(rtValue == pdPASS, "LoggingTask::InitTask() - xTaskCreate() failed");

	DataBroker::Subscribe<IMUData>(this);
	DataBroker::Subscribe<GPSData>(this);
	DataBroker::Subscribe<BaroData>(this);
	DataBroker::Subscribe<MagData>(this);
	DataBroker::Subscribe<FilterData>(this);
}

void LoggingTask::Run(void * pvParams){



    while (1) {
        /* Process commands in blocking mode */
        Command cm;
        bool res = qEvtQueue->ReceiveWait(cm);
        if(res){

        	HandleCommand(cm);
        }
        cm.Reset();


    }

}

void LoggingTask::HandleCommand(Command& cm){

	// SOAR_PRINT("Data In Logging Task\n");
	LoggingStatus err;
	DataBrokerMessageTypes messageType = DataBroker::getMessageType(cm);

	switch(messageType){

	case DataBrokerMessageTypes::IMU_DATA:
	{
		IMUData data = DataBroker::ExtractData<IMUData>(cm);
		uint32_t timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;

		// Only log IDs 0 or 1
		if (data.id != 0 && data.id != 1) {
		    // Skip this sample
		    break;
		}

//		 SOAR_PRINT: timestamp + accel + gyro + temp
//		if(DebugTask::debugEnabled == true){
//			SOAR_PRINT(
//				"IMU ID: %d | Timestamp: %lu ms | Accel: X=%d Y=%d Z=%d | Gyro: X=%d Y=%d Z=%d | Temp=%d C\n",
//				data.id,
//				timestamp,
//				data.accel.x, data.accel.y, data.accel.z,
//				data.gyro.x, data.gyro.y, data.gyro.z,
//				data.temp
//			);
//		}

		// Prepare buffer for flash logging
		buf[0] = static_cast<uint8_t>(data.id == 0 ? LoggingData::IMU16G : LoggingData::IMU32G);

		// Manual serialization to avoid padding issues
		memcpy(buf + 1, &timestamp, sizeof(timestamp));        // 4 bytes timestamp
		memcpy(buf + 5, &data.accel, sizeof(data.accel));     // 6 bytes accel
		memcpy(buf + 11, &data.gyro, sizeof(data.gyro));      // 6 bytes gyro
		memcpy(buf + 17, &data.temp, sizeof(data.temp));      // 2 bytes temp
		buf[19] = data.id;                                     // 1 byte id

		// Log to flash
		LoggingService log(
		    LoggingDest::FLASH_EXTERN,
		    data.id == 0 ? LoggingData::IMU16G : LoggingData::IMU32G,
		    buf,
		    20,  // total bytes: 1(type) + 4(timestamp) + 6+6+2+1 = 20
		    LoggingPriority::SECOND
		);
		err = log.LogData();
		break;
	}
	case DataBrokerMessageTypes::GPS_DATA:
	{
		GPSData data = DataBroker::ExtractData<GPSData>(cm);

		buf[0] = static_cast<uint8_t>(LoggingData::GPS);
		memcpy(buf + 1, &data, sizeof(GPSData));
		LoggingService log = LoggingService(LoggingDest::FLASH_EXTERN, LoggingData::GPS, buf, sizeof(GPSData), LoggingPriority::SECOND);
		err = log.LogData();

		break;

	}
	case DataBrokerMessageTypes:: MAG_DATA:
	{
		MagData data = DataBroker::ExtractData<MagData>(cm);


		buf[0] = static_cast<uint8_t>(LoggingData::MAG);
		memcpy(buf + 1, &data, sizeof(MagData));
		LoggingService log = LoggingService(LoggingDest::FLASH_EXTERN, LoggingData::MAG, buf, sizeof(MagData), LoggingPriority::SECOND);
		err = log.LogData();

		break;
	}
	case DataBrokerMessageTypes:: FILTER_DATA:
	{
		FilterData data = DataBroker::ExtractData<FilterData>(cm);

		buf[0] = static_cast<uint8_t>(LoggingData::FILTER);
		memcpy(buf + 1, &data, sizeof(FilterData));
		LoggingService log = LoggingService(LoggingDest::FLASH_EXTERN, LoggingData::FILTER, buf, sizeof(FilterData), LoggingPriority::FIRST);
		err = log.LogData();

		if(err == LoggingStatus::LOG_HIGHER_PRIORITY){
			SOAR_PRINT("Logged higher priority data");
		}
		else if(err == LoggingStatus::LOG_LOWER_PRIORITY){
			SOAR_PRINT("Lower pirority data was not logged");
		}
		else{
			SOAR_PRINT("Logging Failed");
		}

		break;
	}
	case DataBrokerMessageTypes:: BARO_DATA:
	{
		BaroData data = DataBroker::ExtractData<BaroData>(cm);

		// Get timestamp in ms
		uint32_t timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;

		// Convert pressure to altitude in centimeters (integer)
		// Compute altitude in meters, rounded to integer
		uint32_t altitude_m = static_cast<uint32_t>(
		    44330.0f * (1.0f - powf(static_cast<float>(data.pressure) / 101325.0f, 1.0f / 5.255f))
		);

		// Capture initial altitude once
		if (!firstAltCaptured)
		{
			firstAlt = altitude_m;
			firstAltCaptured = true;
			SOAR_PRINT("firstcapture: %u\n", altitude_m);
		}

		// Launch detection: climbed more than 300m from initial altitude
		if (!launched && (altitude_m - firstAlt > 0.5f))
		{
			launched = true;
//			SOAR_PRINT("000\n");
		}


		// Landing detection:
		if (launched && !landed)
		{
		    if (fabsf(altitude_m - firstAlt) <= 0.9f)
		    {
		        landed = true;
		        SOAR_PRINT("Landed - starting flash dump\n");

		        // Send sysinfo command to DebugTask to trigger the flash dump
		        Command dumpCmd(DATA_COMMAND, EVENT_DEBUG_RX_COMPLETE);
		        DebugTask::Inst().GetEventQueue()->Send(dumpCmd);
		    }
		}

		// Print all raw values + timestamp + altitude as integer meters
//		if(DebugTask::debugEnabled){
//			SOAR_PRINT("Baro ID: %d | Timestamp: %lu ms | Temp: %d C | Pressure: %lu Pa | Altitude: %lu m\n",
//					   data.id,
//					   timestamp,
//					   data.temp / 100,   // if your temp is in milli-degrees, convert to degC
//					   data.pressure,
//					   altitude_m);
//		}
		// Prepare buffer for flash logging
		// Determine BARO type
		buf[0] = static_cast<uint8_t>(data.id == 0 ? LoggingData::BARO07 : LoggingData::BARO11);

		// Copy timestamp (4 bytes)
		memcpy(buf + 1, &timestamp, sizeof(timestamp));

		// Copy pressure (4 bytes) and temperature (2 bytes)
		memcpy(buf + 5, &data.pressure, sizeof(data.pressure));
		memcpy(buf + 9, &data.temp, sizeof(data.temp));

		// Pad remaining bytes with zeros so the total slot is 20 bytes
		memset(buf + 11, 0, 20 - 1 - 10); // 1 byte type + 10 bytes data already, last byte is ID

		// Set ID as the last byte of the 20-byte slot
		buf[19] = data.id;

		// Log the full 20-byte sample
		LoggingService log(
		    LoggingDest::FLASH_EXTERN,
		    data.id == 0 ? LoggingData::BARO07 : LoggingData::BARO11,
		    buf,
		    20,                  // full 20-byte slot
		    LoggingPriority::SECOND
		);

		err = log.LogData();

		break;
	}
	case DataBrokerMessageTypes :: INVALID:
	{
		err = LoggingStatus::LOGGING_ERR;
		SOAR_PRINT("Non-existant DataBrokerMessageType\n");
		break;
	}

	}

	if(err == LoggingStatus::LOGGING_ERR){
		SOAR_PRINT("Log was unsuccessful\n");
		return;
	}
	else if(err == LoggingStatus::LOG_FLASH_NOT_READY || err == LoggingStatus::LOG_FLASH_READY){
		return;
	}

	// SOAR_PRINT("Log was successful\n");
	return;

	cm.Reset();


}



