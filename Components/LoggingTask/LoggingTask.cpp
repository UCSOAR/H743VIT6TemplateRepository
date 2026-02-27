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


uint8_t LoggingTask::buf[30] = {0};
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


	LoggingStatus err;
	DataBrokerMessageTypes messageType = DataBroker::getMessageType(cm);
	switch(messageType){

	case DataBrokerMessageTypes::IMU_DATA:
	{
		IMUData data = DataBroker::ExtractData<IMUData>(cm);

		if(data.id ==0){

			buf[0] = static_cast<uint8_t>(LoggingData::IMU16G);
			memcpy(buf + 1, &data, sizeof(IMUData));
			LoggingService log = LoggingService(LoggingDest::FLASH_EXTERN, LoggingData::IMU16G, buf, sizeof(IMUData) + 1, LoggingPriority::SECOND);
			err = log.LogData();
		}
		else{

			buf[0] = static_cast<uint8_t>(LoggingData::IMU32G);
			memcpy(buf + 1, &data, sizeof(IMUData));
			LoggingService log = LoggingService(LoggingDest::FLASH_EXTERN, LoggingData::IMU32G, buf, sizeof(IMUData) + 1, LoggingPriority::SECOND);
			err = log.LogData();

		}

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

		if(data.id == 0){

			buf[0] = static_cast<uint8_t>(LoggingData::BARO07);
			memcpy(buf + 1, &data, sizeof(BaroData));
			LoggingService log = LoggingService(LoggingDest::FLASH_EXTERN, LoggingData::BARO07, buf, sizeof(BaroData), LoggingPriority::SECOND);
			err = log.LogData();
		}
		else{

			buf[0] = static_cast<uint8_t>(LoggingData::BARO11);
			memcpy(buf + 1, &data, sizeof(BaroData));
			LoggingService log = LoggingService(LoggingDest::FLASH_EXTERN, LoggingData::BARO11, buf, sizeof(BaroData), LoggingPriority::SECOND);
			err = log.LogData();

		}


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
	else if(err == LoggingStatus::LOG_FLASH_NOT_READY || err == LoggingStatus::LOG_FLASH_READY || err == LoggingStatus::FLASH_FULL){
		return;
	}

	SOAR_PRINT("Log was successful\n");
	return;

	cm.Reset();


}



