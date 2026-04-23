/*
 * LoggingService.cpp
 *
 *  Created on: Jan 21, 2026
 *      Author: jaddina
 */
#include "LoggingService.hpp"
#include "stm32h7xx_hal.h"
#include "cmsis_os.h"

uint8_t  LoggingService::ramLog[RAM_LOG_SIZE] = {0};
uint32_t LoggingService::ramHead = 0;
uint16_t LoggingService::sectorAddress = 0;
uint8_t LoggingService::bufferPerSector = 0;
uint8_t LoggingService::sectorCount = 0;
volatile uint8_t LoggingService::done = true;
volatile uint8_t LoggingService::doneDump =0;


static uint32_t dumpIndex = 0;
static uint8_t sectorBuf[RAM_LOG_SIZE];
static uint32_t dumpSector = 0;
static uint16_t dumpOffset = 0;
static uint32_t sectorStartTickMs = 0;
static uint8_t txBuf[RAM_LOG_SIZE];
static uint8_t rxBuf[RAM_LOG_SIZE];

namespace
{
	constexpr uint16_t FLASH_STATE_SECTOR = NUM_SECTORS - 1;
	constexpr uint16_t FLASH_DATA_LIMIT_SECTOR = NUM_SECTORS - 1;
	constexpr uint32_t FLASH_STATE_MAGIC = 0x4C4F4731;

	struct FlashStateRecord
	{
		uint32_t magic;
		uint32_t sector;
		uint32_t bufferPerSector;
		uint32_t checksum;
	};

	static uint32_t FlashStateChecksum(const FlashStateRecord &record)
	{
		return record.magic ^ record.sector ^ record.bufferPerSector ^ 0xA5A55A5A;
	}

	static bool ReadFlashState(FlashStateRecord &record)
	{
		uint8_t raw[sizeof(FlashStateRecord)] = {0};
		MX66xxQSPI_ReadSector(raw, FLASH_STATE_SECTOR, 0, sizeof(raw));
		memcpy(&record, raw, sizeof(record));
		return (record.magic == FLASH_STATE_MAGIC) && (record.checksum == FlashStateChecksum(record));
	}
}

bool LoggingService::flashStateLoaded = false;

void LoggingService::SaveFlashState()
{
	FlashStateRecord record{};
	record.magic = FLASH_STATE_MAGIC;
	record.sector = sectorAddress;
	record.bufferPerSector = bufferPerSector;
	record.checksum = FlashStateChecksum(record);

	uint8_t raw[sizeof(record)] = {0};
	memcpy(raw, &record, sizeof(record));

	MX66xxQSPI_EraseSector(FLASH_STATE_SECTOR);
	MX66xxQSPI_WriteSector(raw, FLASH_STATE_SECTOR, 0, sizeof(raw));
}

void LoggingService::LoadFlashStateFromStorage()
{
	if (flashStateLoaded)
	{
		return;
	}

	FlashStateRecord record{};
	if (ReadFlashState(record) && record.sector <= FLASH_DATA_LIMIT_SECTOR && record.bufferPerSector <= 8U)
	{
		sectorAddress = static_cast<uint16_t>(record.sector);
		bufferPerSector = static_cast<uint8_t>(record.bufferPerSector);
		done = (sectorAddress == FLASH_STATE_SECTOR) ? 1U : 0U;
		flashStateLoaded = true;
		return;
	}

	for (uint16_t sector = 0; sector < FLASH_DATA_LIMIT_SECTOR; ++sector)
	{
		for (uint8_t chunk = 0; chunk < 8U; ++chunk)
		{
			memset(sectorBuf, 0xFF, sizeof(sectorBuf));
			MX66xxQSPI_ReadSector(sectorBuf, sector, static_cast<uint32_t>(chunk) * RAM_LOG_SIZE, RAM_LOG_SIZE);

			bool empty = true;
			for (uint32_t i = 0; i < RAM_LOG_SIZE; ++i)
			{
				if (sectorBuf[i] != 0xFF)
				{
					empty = false;
					break;
				}
			}

			if (empty)
			{
				sectorAddress = sector;
				bufferPerSector = chunk;
				done = 0;
				flashStateLoaded = true;
				SaveFlashState();
				return;
			}
		}
	}

	sectorAddress = FLASH_STATE_SECTOR;
	bufferPerSector = 0;
	done = 1;
	flashStateLoaded = true;
	SaveFlashState();
}


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

		err = LoggingStatus::LOGGING_ERR;
		break;

	case LoggingDest::DMA:

		//TODO DMA write api
		err = LoggingStatus::LOGGING_ERR;
		break;
	}

	return err;
}


LoggingStatus LoggingService::LogToMX66(){

	LoadFlashStateFromStorage();

	//appends data to buffer and returns flag determining if buffer is full (full => write data)
	LoggingStatus status = MemAppend(&loggingData);
	//stops logging, triggered by flash dump, or flashchip is full
	if(!done){
		//checks if 500 byte chunck is full (writes when it is full)
		if(status == LoggingStatus::LOG_FLASH_READY){
			if(sectorAddress >= FLASH_DATA_LIMIT_SECTOR){
				done = true;
				return LoggingStatus::FLASH_FULL;
			}

			if(bufferPerSector == 0){
				//if a new sector is reached erase before writing
				MX66xxQSPI_EraseSector(sectorAddress);
			}

			//Write data to bufferPerSector * 500 this calculates the offset at which the buffer will be written
			memcpy(txBuf, ramLog, RAM_LOG_SIZE);

			MX66xxQSPI_WriteSector(txBuf, sectorAddress, (bufferPerSector * 500),  RAM_LOG_SIZE);

			MX66xxQSPI_ReadSector(rxBuf, sectorAddress,(bufferPerSector * 500),  RAM_LOG_SIZE);

			//check if the bytes written equal the bytes read
			if(BytesEqual(rxBuf, txBuf, RAM_LOG_SIZE)){

				/* Each buffer is 500 bytes, 8 can fit in 4000 bytes
				 * bufferPerSector increases each successful buffer write which
				 * is used to keep trak of when a sector is full, if it is full
				 * (bufferPerSector == 8) then go to the next sector.
				 */
				bufferPerSector++;
				if(bufferPerSector == 8){
					sectorAddress++;
					bufferPerSector = 0;
					//finish logging if the sectorAddress reaches address that does not exist
					if(sectorAddress >= FLASH_STATE_SECTOR){
						sectorAddress = FLASH_STATE_SECTOR;
						done = true;
					}
				}


				return LoggingStatus::LOGGING_SUCCESS;
			}


			return status;

		}
		return LoggingStatus::LOG_FLASH_NOT_READY;
	}
	
	return LoggingStatus::FLASH_FULL;


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
	LoadFlashStateFromStorage();

	const uint8_t wasDone = done;
	done = true;
	doneDump = false;
	dumpSector = 0;
	dumpOffset = 0;

	constexpr uint32_t RECORD_SIZE = 20;
	constexpr uint32_t CHUNK_SIZE  = 500;

	//variables for gps read back
	char gpsSentence[256] = {0};
	uint16_t gpsSentenceLen = 0;
	uint8_t gpsExpectedChunks = 0;
	uint32_t gpsTimestamp = 0;

	// Snapshot the logged extent so we don't walk the entire flash device.
	uint32_t maxSector = sectorAddress;
	uint16_t maxOffset = static_cast<uint16_t>(bufferPerSector * RAM_LOG_SIZE);
	if (sectorAddress == FLASH_STATE_SECTOR)
	{
		maxSector = FLASH_DATA_LIMIT_SECTOR - 1;
		maxOffset = SECTOR_READ;
	}

	if (maxOffset == 0)
	{
		if (maxSector == 0)
		{
			SOAR_PRINT("------FLASH DUMP EMPTY------\n");
			done = wasDone;
			return;
		}
		maxSector -= 1;
		maxOffset = SECTOR_READ;
	}

	while ((dumpSector < maxSector || (dumpSector == maxSector && dumpOffset < maxOffset)) && !doneDump) {

		memset(sectorBuf, 0, sizeof(sectorBuf));
		MX66xxQSPI_ReadSector(sectorBuf, dumpSector, dumpOffset, CHUNK_SIZE);

		for (uint32_t i = 0; i + RECORD_SIZE <= CHUNK_SIZE; i += RECORD_SIZE) //each record is 20 bytes, Chunck size is 500 to prevent hardfault
		{
			LoggingData type = static_cast<LoggingData>(sectorBuf[i]); //get type byte first
			uint32_t timestamp;
			memcpy(&timestamp, sectorBuf + i + 1, sizeof(timestamp));
			uint8_t id = sectorBuf[i + 19];

			//parse the data back into structs

			if(type == LoggingData::IMU16G || type == LoggingData::IMU32G)
			{
				int16_t accel[3], gyro[3], temp;

				memcpy(accel, sectorBuf + i + 5, sizeof(accel));
				memcpy(gyro, sectorBuf + i + 11, sizeof(gyro));
				memcpy(&temp, sectorBuf + i + 17, sizeof(temp));

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
			else if (type == LoggingData::MAG){

				int32_t magX, magY, magZ;

				memcpy(&magX, sectorBuf + i + 5,  sizeof(int32_t));
				memcpy(&magY, sectorBuf + i + 9,  sizeof(int32_t));
				memcpy(&magZ, sectorBuf + i + 13, sizeof(int32_t));

				SOAR_PRINT("%s Timestamp=%lu Mag=[%ld,%ld,%ld]\n",
					SensorTypeName(type), timestamp,
					(long)magX, (long)magY, (long)magZ);

			}
			else if (type == LoggingData::GPS)
			{
				//get the chunk index, count, and payload length
				uint8_t chunkIdx = sectorBuf[i + 5];
				uint8_t chunkCount = sectorBuf[i + 6];
				uint8_t payloadLen = sectorBuf[i + 7];

				//clamp payload length at 12
				if (payloadLen > 12)
				{
					payloadLen = 12;
				}

				//if the chunk index is 0 set the sentence buffer to 0,
				//set the length to 0, set the chunks and time stamp tp read back values
				if (chunkIdx == 0)
				{
					memset(gpsSentence, 0, sizeof(gpsSentence));
					gpsSentenceLen = 0;
					gpsExpectedChunks = chunkCount;
					gpsTimestamp = timestamp;
				}

				//if payload length is greater than 0, and the sentence length plus the payload length is less than the
				//size of the sentence then the appropriate bytes from the sector buffer get added into the sentence
				if ((payloadLen > 0U) && (gpsSentenceLen + payloadLen < sizeof(gpsSentence)))
				{
					memcpy(gpsSentence + gpsSentenceLen, sectorBuf + i + 8, payloadLen);
					gpsSentenceLen = static_cast<uint16_t>(gpsSentenceLen + payloadLen);
					gpsSentence[gpsSentenceLen] = '\0';
				}

				//If the last chunk is detected the sentence can br printed to the serial terminal
				if ((chunkCount > 0U) && (chunkIdx + 1U >= chunkCount))
				{
					SOAR_PRINT("GPS Timestamp=%lu Chunks=%u Sentence=%s\n",
							   (unsigned long)gpsTimestamp,
							   (unsigned int)gpsExpectedChunks,
							   gpsSentence);
				}
			}

		}
		/*the dump offset at this point should be 3500, this means sector is full
		 * (writtern up to 4000 bytes, if this is true set the offset back to 0 and
		 * move to the next sector, otherwise increment dumpoffset to the next chunk
		 */
		if (dumpOffset == SECTOR_READ - CHUNK_SIZE) {
			dumpOffset = 0;
			dumpSector++;
		} else {
			dumpOffset += 500;
		}

		// Let lower-priority UART/debug tasks run while the dump is in progress.
		osDelay(1);
	}

	if (doneDump)
	{
		SOAR_PRINT("------FLASH DUMP STOPPED------\n");
	}
	else
	{
		SOAR_PRINT("------FLASH DUMP COMPLETE------\n");
	}

	done = wasDone;
}


void LoggingService::StopDump(){
	doneDump = true;
}
void LoggingService::StartLogging(){
	done = false;
}
void LoggingService::StopLogging(){
	done = true;
	SaveFlashState();
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

void LoggingService::FlashClear(){
	LoadFlashStateFromStorage();

	// Keep clear bounded: full-chip erase can block long enough to trip watchdogs.
	MX66xxQSPI_EraseSector(0);
	MX66xxQSPI_EraseSector(FLASH_STATE_SECTOR);

	ramHead = 0;
	sectorAddress = 0;
	bufferPerSector = 0;
	sectorCount = 0;
	flashStateLoaded = true;
	done = 0;
	doneDump = 0;

	SaveFlashState();
	SOAR_PRINT("Flash clear: logging cursor reset\n");

}


