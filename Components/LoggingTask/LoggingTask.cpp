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


/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * VARIABLES
 ************************************/

/************************************
 * FUNCTION DECLARATIONS
 ************************************/



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
    }
}

void LoggingTask::HandleCommand(Command& cm){

	SOAR_PRINT("Data In Logging Task\n");
	LoggingStatus err;
	DataBrokerMessageTypes messageType = DataBroker::getMessageType(cm);
	switch(messageType){

	case DataBrokerMessageTypes::IMU_DATA:
	{
		IMUData data = DataBroker::ExtractData<IMUData>(cm);
		SOAR_PRINT("IMU accelX: %d\n", data.accel.x);
		SOAR_PRINT("IMU accelY: %d\n", data.accel.y);
		SOAR_PRINT("IMU accelY: %d\n", data.accel.x);

		SOAR_PRINT("IMU accelY: %d\n", data.gyro.x);
		SOAR_PRINT("IMU accelY: %d\n", data.gyro.y);
		SOAR_PRINT("IMU accelY: %d\n", data.gyro.z);

		SOAR_PRINT("IMU temperature: %d\n", data.temp);

		if(data.id ==0){
			uint8_t buf[128];
			buf[0] = static_cast<uint8_t>(LoggingData::IMU16G);
			memcpy(buf + 1, &data, sizeof(IMUData));
			LoggingService log = LoggingService(LoggingDest::FLASH_EXTERN, LoggingData::IMU16G, buf, sizeof(IMUData));
			err = log.LogData();
		}
		else{
			uint8_t buf[128];
			buf[0] = static_cast<uint8_t>(LoggingData::IMU32G);
			memcpy(buf + 1, &data, sizeof(IMUData));
			LoggingService log = LoggingService(LoggingDest::FLASH_EXTERN, LoggingData::IMU32G, buf, sizeof(IMUData));
			err = log.LogData();
		}

		break;
	}
	case DataBrokerMessageTypes::GPS_DATA:
	{
		GPSData data = DataBroker::ExtractData<GPSData>(cm);
		uint8_t buf[128];
		buf[0] = static_cast<uint8_t>(LoggingData::GPS);
		memcpy(buf, &data, sizeof(GPSData));
		LoggingService log = LoggingService(LoggingDest::FLASH_EXTERN, LoggingData::GPS, buf, sizeof(GPSData));
		err = log.LogData();

		break;

	}
	case DataBrokerMessageTypes:: MAG_DATA:
	{
		MagData data = DataBroker::ExtractData<MagData>(cm);
		SOAR_PRINT("Mag rawX: %d\n", data.rawX);
		SOAR_PRINT("Mag rawY: %d\n", data.rawY);
		SOAR_PRINT("Mag rawZ: %d\n", data.rawZ);

		SOAR_PRINT("Mag scaledX: %f\n", data.scaledX);
		SOAR_PRINT("Mag scaledY: %f\n", data.scaledY);
		SOAR_PRINT("Mag scaledZ: %f\n", data.scaledZ);
		uint8_t buf[128];
		buf[0] = static_cast<uint8_t>(LoggingData::MAG);
		memcpy(buf, &data, sizeof(MagData));
		LoggingService log = LoggingService(LoggingDest::FLASH_EXTERN, LoggingData::MAG, buf, sizeof(MagData));
		err = log.LogData();

		break;
	}
	case DataBrokerMessageTypes:: FILTER_DATA:
	{
		FilterData data = DataBroker::ExtractData<FilterData>(cm);
		uint8_t buf[128];
		buf[0] = static_cast<uint8_t>(LoggingData::FILTER);
		memcpy(buf, &data, sizeof(FilterData));
		LoggingService log = LoggingService(LoggingDest::FLASH_EXTERN, LoggingData::FILTER, buf, sizeof(FilterData));
		err = log.LogData();

		break;
	}
	case DataBrokerMessageTypes:: BARO_DATA:
	{
		BaroData data = DataBroker::ExtractData<BaroData>(cm);
		SOAR_PRINT("Baro temperature: %d\n", data.temp);
		SOAR_PRINT("Baro pressure: %d\n", data.pressure);

		if(data.id == 0){
			uint8_t buf[128];
			buf[0] = static_cast<uint8_t>(LoggingData::BARO07);
			memcpy(buf, &data, sizeof(BaroData));
			LoggingService log = LoggingService(LoggingDest::FLASH_EXTERN, LoggingData::BARO07, buf, sizeof(BaroData));
			err = log.LogData();
		}
		else{
			uint8_t buf[128];
			buf[0] = static_cast<uint8_t>(LoggingData::BARO11);
			memcpy(buf, &data, sizeof(BaroData));
			LoggingService log = LoggingService(LoggingDest::FLASH_EXTERN, LoggingData::BARO11, buf, sizeof(BaroData));
			err = log.LogData();

		}


		break;
	}
	case DataBrokerMessageTypes :: INVALID:
	{
		err = LoggingStatus::LOGGING_ERR;
		SOAR_PRINT("Non-existant DataBrokerMessageType");
		break;
	}

	}

	if(err == LoggingStatus::LOGGING_ERR){
		SOAR_PRINT("Log was unsuccessful");
		return;
	}

	SOAR_PRINT("Log was successful");
	return;



}



