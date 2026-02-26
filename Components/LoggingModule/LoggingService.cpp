/*
 * LoggingService.cpp
 *
 *  Created on: Jan 21, 2026
 *      Author: jaddina
 */
#include "LoggingService.hpp"

uint8_t  LoggingService::ramLog[RAM_LOG_SIZE] = {0};
uint32_t LoggingService::ramHead = 0;

LoggingService::LoggingService(LoggingDest dest, LoggingData dataType, uint8_t* data, uint32_t dataSize)
{
	loggingData.dest = dest;
	loggingData.dataType = dataType;
	loggingData.data = data;
	loggingData.dataSize = dataSize;


}


LoggingStatus LoggingService::LogData(){

	LoggingStatus err;

	switch(loggingData.dest){
	case LoggingDest::FLASH_INTERN:
		//internal flash api
		if(LogToInternalMemory() == LoggingStatus::LOGGING_SUCCESS){
			err = LoggingStatus::LOGGING_SUCCESS;
		}
		else{
			err = LoggingStatus::LOGGING_ERR;
		}
		break;

	case LoggingDest::FLASH_EXTERN:

		if(LogToMX66() == LoggingStatus::LOGGING_SUCCESS){

			err = LoggingStatus::LOGGING_SUCCESS;
		}
		else if(LogToInternalMemory() == LoggingStatus::LOGGING_SUCCESS){

			err = LoggingStatus::LOGGING_SUCCESS;
		}
		else{
			err = LoggingStatus::LOGGING_ERR;
		}
		break;

	case LoggingDest::FILE_SYSTEM:
		//TODO FS write api
		SOAR_PRINT("Log to file system\n");
		err = LoggingStatus::LOGGING_ERR;
		break;

	case LoggingDest::DMA:

		//TODO DMA write api
		SOAR_PRINT("Transfer to DMA\n");
		err = LoggingStatus::LOGGING_ERR;
		break;
	}
	if(err == LoggingStatus::LOGGING_SUCCESS){
		//SOAR_PRINT("Data logged successfully\n");
	}

	return err;
}


LoggingStatus LoggingService::LogToMX66(){
//	static uint32_t sector = 0;
//	static uint16_t offset = 0;
//
//	if(offset + loggingData.dataSize > 512){
//		sector++;
//		offset = 0;
//	}
//
//	//erase sector when starting fresh
//	if(offset == 0){
//
//		//MX66_Erase_Sector((uint16_t)sector);
//	}
//
//	//MX66_Write_Block(sector, offset, (uint32_t)loggingData.dataSize, (const uint8_t*)loggingData.data);
//
//	uint8_t verifyBuf[256];
//
//	//MX66_Read(sector, offset, (uint32_t)loggingData.dataSize, verifyBuf);
//
//	if (!BytesEqual(loggingData.data, verifyBuf, loggingData.dataSize)){
//		return LoggingStatus::LOGGING_ERR;
//	}
//
//    offset = (uint16_t)(offset + (uint16_t)loggingData.dataSize);
	SOAR_PRINT("%d\n", loggingData.data[0]);
	return LoggingStatus::LOGGING_SUCCESS;



}

LoggingStatus LoggingService::LogToInternalMemory(){

	if(loggingData.data == nullptr || loggingData.dataSize == 0){
		return LoggingStatus::LOGGING_ERR;
	}

	MemAppend(loggingData.data, loggingData.dataSize);

	return LoggingStatus::LOGGING_SUCCESS;

}

bool LoggingService::BytesEqual(const uint8_t* a, const uint8_t* b, uint32_t n){
	for(uint32_t i = 0; i < n; i++){

		if(a[i] != b[i]) return false;
	}

	return true;
}

void LoggingService::MemAppend(const uint8_t *data, uint32_t size){

	if(ramHead + size > RAM_LOG_SIZE){
		ramHead = 0;
	}

	for(uint32_t i = 0; i < size; i++){
		ramLog[ramHead] = data[i];
		ramHead++;
	}
}
