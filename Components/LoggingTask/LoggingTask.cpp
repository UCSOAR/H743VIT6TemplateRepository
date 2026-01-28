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
/*
#include "WriteBufferFixedSize.h"
#include "ReadBufferFixedSize.h"
#include "cobs.h"
*/
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
}

void LoggingTask::Run(void * pvParams){

	DataBroker::Subscribe<IMU32GData>(this);
	DataBroker::Subscribe<GPSData>(this);
	DataBroker::Subscribe<Baro07Data>(this);
	DataBroker::Subscribe<Baro11Data>(this);
	DataBroker::Subscribe<MagData>(this);
	DataBroker::Subscribe<FilterData>(this);


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

	LoggingStatus err;
	DataBrokerMessageTypes messageType = DataBroker::getMessageType(cm);
	switch(messageType){

	case DataBrokerMessageTypes::IMU32G_DATA:
	{
		IMU32GData data = DataBroker::ExtractData<IMU32GData>(cm);
		uint8_t buf[128];
		buf[0] = static_cast<uint8_t>(LoggingData::IMU32G);
		memcpy(buf + 1, &data, sizeof(IMU32GData));
		LoggingService log = LoggingService(LoggingDest::FLASH_EXTERN, LoggingData::IMU32G, buf, sizeof(IMU32GData));
		err = log.LogData();

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
	case DataBrokerMessageTypes:: BARO07_DATA:
	{
		Baro07Data data = DataBroker::ExtractData<Baro07Data>(cm);
		uint8_t buf[128];
		buf[0] = static_cast<uint8_t>(LoggingData::BARO07);
		memcpy(buf, &data, sizeof(Baro07Data));
		LoggingService log = LoggingService(LoggingDest::FLASH_EXTERN, LoggingData::BARO07, buf, sizeof(Baro07Data));
		err = log.LogData();

		break;
	}
	case DataBrokerMessageTypes:: MAG_DATA:
	{
		MagData data = DataBroker::ExtractData<MagData>(cm);
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
	case DataBrokerMessageTypes:: BARO11_DATA:
	{
		Baro11Data data = DataBroker::ExtractData<Baro11Data>(cm);
		uint8_t buf[128];
		buf[0] = static_cast<uint8_t>(LoggingData::BARO11);
		memcpy(buf, &data, sizeof(Baro11Data));
		LoggingService log = LoggingService(LoggingDest::FLASH_EXTERN, LoggingData::BARO11, buf, sizeof(Baro11Data));
		err = log.LogData();

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



