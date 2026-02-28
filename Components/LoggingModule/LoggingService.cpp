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
uint8_t LoggingService::bufferPerSector = 0;

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
		err = LogToMX66();
		break;

	case LoggingDest::FILE_SYSTEM:
		//TODO FS write api
		//SOAR_PRINT("Log to file system\n");
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

	LoggingStatus status = MemAppend(&loggingData);
	if(status == LoggingStatus::LOG_FLASH_READY){
		if(bufferPerSector == 0){
			MX66xxQSPI_EraseSector(sectorAddress);
		}
		uint8_t txBuf[RAM_LOG_SIZE];
		memcpy(txBuf, ramLog, RAM_LOG_SIZE);


		MX66xxQSPI_WriteSector(txBuf, sectorAddress, (bufferPerSector * 500),  RAM_LOG_SIZE);


		uint8_t rxBuf[RAM_LOG_SIZE];
		MX66xxQSPI_ReadSector(rxBuf, sectorAddress,(bufferPerSector * 500),  RAM_LOG_SIZE);

		if(BytesEqual(rxBuf, txBuf, RAM_LOG_SIZE)){

			bufferPerSector++;
			if(bufferPerSector == 8){
//				SOAR_PRINT("-------------------------------Next Sector---------------------------------");
				sectorAddress++;
				bufferPerSector = 0;
			}

			//SOAR_PRINT("FLASHED DATA");
			return LoggingStatus::LOGGING_SUCCESS;
		}

		SOAR_PRINT("Bytes did not equal");
		return status;

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

	for(uint32_t i = 0; i < n; i++){

		if(a[i] != b[i]){
			return false;
		}

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
		//SOAR_PRINT("LOGGING READY");
		return LoggingStatus::LOG_FLASH_READY;
	}


	for(uint32_t i = 0; i < size; i++){

		ramLog[ramHead++] = data->data[i];
	}

	for(uint32_t i = size; i < MAX_LOG_SIZE; i++){
		ramLog[ramHead++] = 0;
	}


	return LoggingStatus::LOG_FLASH_NOT_READY;



}
