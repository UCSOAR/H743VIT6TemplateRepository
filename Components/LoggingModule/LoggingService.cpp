/*
 * LoggingService.cpp
 *
 *  Created on: Jan 21, 2026
 *      Author: jaddina
 */
#include "LoggingService.hpp"

uint8_t  LoggingService::ramLog[RAM_LOG_SIZE] = {0};
uint32_t LoggingService::ramHead = 0;
uint16_t LoggingService::sectorAddress = 0;

LoggingService::LoggingService(LoggingDest dest, LoggingData dataType, uint8_t* ldata, uint32_t dataSize, LoggingPriority priority)
{
	loggingData.dest = dest;
	loggingData.dataType = dataType;
	loggingData.data = ldata;
	loggingData.dataSize = dataSize;
	loggingData.priority = priority;


}


LoggingStatus LoggingService::LogData(){

	LoggingStatus err;

	switch(loggingData.dest){
	case LoggingDest::RAM:
		//internal flash api
		err = LogToInternalMemory();

		break;

	case LoggingDest::FLASH_EXTERN:

		if(LogToMX66() == LoggingStatus::LOGGING_SUCCESS){

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

	return err;
}


LoggingStatus LoggingService::LogToMX66(){

	if(MemAppend(&loggingData) == LoggingStatus::LOG_FLASH_READY){

		uint8_t txBuf[RAM_LOG_SIZE];
		memcpy(txBuf, ramLog, RAM_LOG_SIZE);
		MX66xxQSPI_WriteSector(txBuf, sectorAddress, 0,  RAM_LOG_SIZE);


		uint8_t rxBuf[RAM_LOG_SIZE];
		MX66xxQSPI_ReadSector(rxBuf, sectorAddress, 0,  RAM_LOG_SIZE);

		if(BytesEqual(rxBuf, txBuf, RAM_LOG_SIZE)){
			sectorAddress++;
			SOAR_PRINT("FLASHED DATA");
			return LoggingStatus::LOGGING_SUCCESS;
		}

		SOAR_PRINT("Bytes did not equal");
		return LoggingStatus::LOGGING_ERR;

	}
	return LoggingStatus::LOG_FLASH_NOT_READY;

}



LoggingStatus LoggingService::LogToInternalMemory(){

	if(loggingData.data == nullptr || loggingData.dataSize == 0){
		return LoggingStatus::LOGGING_ERR;
	}

	return MemAppend(&loggingData);

}

bool LoggingService::BytesEqual(const uint8_t* a, const uint8_t* b, uint32_t n){
	int count = 0;
	for(uint32_t i = 21; i < n; i++){

		if(a[i] != b[i]){
			SOAR_PRINT("COUNT : %d", count);
			return false;
		}
		count++;
	}

	return true;
}

LoggingStatus LoggingService::MemAppend(const LoggingPacket *data){

	if(!data){return LoggingStatus::LOGGING_ERR;}

	uint32_t size = data->dataSize;

	if (size > MAX_LOG_SIZE){
		size = MAX_LOG_SIZE;
	}

	if(ramHead + MAX_LOG_SIZE > RAM_LOG_SIZE){ //check if word will fit in buffer
		ramHead = 0;
		SOAR_PRINT("LOGGING READY");
		return LoggingStatus::LOG_FLASH_READY;
	}


	for(uint32_t i = 0; i < size; i++){

		ramLog[ramHead++] = data->data[i];
	}

	for(uint32_t i = size; i < MAX_LOG_SIZE; i++){
		ramLog[ramHead++] = 0;
	}
	SOAR_PRINT("%d", ramHead);
	return LoggingStatus::LOGGING_SUCCESS;



}
