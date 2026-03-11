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
uint8_t LoggingService::sectorCount = 0;
uint8_t LoggingService::done = 0;

static bool dumpActive = false;
static uint32_t dumpIndex = 0;
static uint8_t sectorBuf[FS_SECTOR_SIZE];
static uint32_t dumpSector = 0;

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

			    SOAR_PRINT("---- Sector Complete ----\n");

			    MX66xxQSPI_ReadSector(sectorBuf, sectorAddress, 0, FS_SECTOR_SIZE);

			    dumpActive = true;
			    dumpIndex = 0;
			    dumpSector = sectorAddress;

			    sectorAddress++;
			    bufferPerSector = 0;
			}


			// SOAR_PRINT("FLASHED DATA");
			return LoggingStatus::LOGGING_SUCCESS;
		}

		SOAR_PRINT("Bytes did not equal");
		return status;

	}
	return LoggingStatus::LOG_FLASH_NOT_READY;

}

const char* SensorTypeName(LoggingData type)
{
    switch(type)
    {
        case LoggingData::IMU32G: return "IMU32G";
        case LoggingData::IMU16G: return "IMU16G";
        case LoggingData::MAG:    return "MAG";
        case LoggingData::BARO07: return "BARO07";
        case LoggingData::BARO11: return "BARO11";
        case LoggingData::GPS:    return "GPS";
        case LoggingData::FILTER: return "FILTER";
        default: return "UNKNOWN";
    }
}

void LoggingService::ProcessFlashDump()
{
    if(!dumpActive) return;

    const uint32_t CHUNK_SIZE = 60; // multiple of 20 bytes for full samples
    uint32_t end = dumpIndex + CHUNK_SIZE;
    if(end > FS_SECTOR_SIZE) end = FS_SECTOR_SIZE;

    for(uint32_t i = dumpIndex; i + 19 < end; i += 20)  // step by full sample
    {
        LoggingData type = static_cast<LoggingData>(sectorBuf[i]);
        uint32_t timestamp;
        int16_t accel[3], gyro[3], temp;
        uint8_t id;

        memcpy(&timestamp, sectorBuf + i + 1, sizeof(timestamp));
        memcpy(accel, sectorBuf + i + 5, sizeof(accel));
        memcpy(gyro, sectorBuf + i + 11, sizeof(gyro));
        memcpy(&temp, sectorBuf + i + 17, sizeof(temp));
        id = sectorBuf[i + 19];

        // Only IMU for now
        if(type == LoggingData::IMU16G || type == LoggingData::IMU32G)
        {
            SOAR_PRINT("%s(ID=%u) Timestamp=%lu Accel=[%d,%d,%d] Gyro=[%d,%d,%d] Temp=%d\n",
                SensorTypeName(type), id, timestamp,
                accel[0], accel[1], accel[2],
                gyro[0], gyro[1], gyro[2],
                temp);
        }
        else if(type == LoggingData::BARO07 || type == LoggingData::BARO11)
        {
            int32_t pressure;
            int16_t temperature;
            memcpy(&pressure, sectorBuf + i + 5, sizeof(pressure));
            memcpy(&temperature, sectorBuf + i + 9, sizeof(temperature));

            int16_t temp_c = temperature / 100;          // integer part
            int16_t temp_frac = temperature % 100;       // fractional part
            if (temp_frac < 0) temp_frac = -temp_frac;   // handle negative temperatures
            SOAR_PRINT("%s(ID=%u) Timestamp=%lu Pressure=%ld Temp=%d.%02d\n",
                SensorTypeName(type), id, timestamp, pressure, temp_c, temp_frac);
        }
    }

    dumpIndex = end;

    if(dumpIndex >= FS_SECTOR_SIZE)
    {
        SOAR_PRINT("\n--- Dump Complete ---\n");
        dumpActive = false;
    }

    vTaskDelay(pdMS_TO_TICKS(5));
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
		// SOAR_PRINT("LOGGING READY");
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
