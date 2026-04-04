/*
 * LoggingService.cpp
 *
 *  Created on: Jan 21, 2026
 *      Author: jaddina
 */
#include "LoggingService.hpp"
#include "stm32h7xx_hal.h"

uint8_t LoggingService::ramLog[RAM_LOG_SIZE] = {0};
uint32_t LoggingService::ramHead = 0;
uint16_t LoggingService::sectorAddress = 0;
uint8_t LoggingService::bufferPerSector = 0;
uint8_t LoggingService::sectorCount = 0;
uint8_t LoggingService::done = 0;
uint8_t LoggingService::doneDump = 0;

// Text log buffers (separate from sensor data logging)
uint8_t LoggingService::textLogBuffer[RAM_LOG_SIZE] = {0};
uint32_t LoggingService::textLogHead = 0;
uint16_t LoggingService::textSectorAddress = 256; // Start text logs at sector 256 to separate from sensor data
uint8_t LoggingService::textBufferPerSector = 0;

static uint32_t dumpIndex = 0;
static uint8_t sectorBuf[RAM_LOG_SIZE];
static uint32_t dumpSector = 0;
static uint16_t dumpOffset = 0;
static uint32_t sectorStartTickMs = 0;
static uint8_t txBuf[RAM_LOG_SIZE];
static uint8_t rxBuf[RAM_LOG_SIZE];

LoggingService::LoggingService(LoggingDest dest, LoggingData dataType, uint8_t *ldata, uint32_t dataSize, LoggingPriority priority)
{
	loggingData.dest = dest;
	loggingData.dataType = dataType;
	loggingData.data = ldata;
	loggingData.dataSize = dataSize;
	loggingData.priority = priority;
}

LoggingStatus LoggingService::LogData()
{

	LoggingStatus err;

	switch (loggingData.dest)
	{
	case LoggingDest::RAM:
		// internal flash api
		err = LogToInternalMemory();

		break;

	case LoggingDest::FLASH_EXTERN:
		err = LogToMX66();
		break;

	case LoggingDest::FILE_SYSTEM:
		// TODO FS write api

		err = LoggingStatus::LOGGING_ERR;
		break;

	case LoggingDest::DMA:

		// TODO DMA write api
		err = LoggingStatus::LOGGING_ERR;
		break;
	}

	return err;
}

LoggingStatus LoggingService::LogToMX66()
{

	// appends data to buffer and returns flag determining if buffer is full (full => write data)
	LoggingStatus status = MemAppend(&loggingData);
	// stops logging, triggered by flash dump, or flashchip is full
	if (!done)
	{
		// checks if 500 byte chunck is full (writes when it is full)
		if (status == LoggingStatus::LOG_FLASH_READY)
		{
			if (bufferPerSector == 0)
			{
				// if a new sector is reached erase before writing
				MX66xxQSPI_EraseSector(sectorAddress);
			}

			// Write data to bufferPerSector * 500 this calculates the offset at which the buffer will be written
			memcpy(txBuf, ramLog, RAM_LOG_SIZE);

			MX66xxQSPI_WriteSector(txBuf, sectorAddress, (bufferPerSector * 500), RAM_LOG_SIZE);

			MX66xxQSPI_ReadSector(rxBuf, sectorAddress, (bufferPerSector * 500), RAM_LOG_SIZE);

			// check if the bytes written equal the bytes read
			if (BytesEqual(rxBuf, txBuf, RAM_LOG_SIZE))
			{

				/* Each buffer is 500 bytes, 8 can fit in 4000 bytes
				 * bufferPerSector increases each successful buffer write which
				 * is used to keep trak of when a sector is full, if it is full
				 * (bufferPerSector == 8) then go to the next sector.
				 */
				bufferPerSector++;
				if (bufferPerSector == 8)
				{
					sectorAddress++;
					bufferPerSector = 0;
					// finish logging if the sectorAddress reaches address that does not exist
					if (sectorAddress > NUM_SECTORS)
					{
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

const char *SensorTypeName(LoggingData type)
{
	switch (type)
	{
	case LoggingData::IMU32G:
		return "IMU32G";
	case LoggingData::IMU16G:
		return "IMU16G";
	case LoggingData::MAG:
		return "MAG";
	case LoggingData::BARO07:
		return "BARO07";
	case LoggingData::BARO11:
		return "BARO11";
	case LoggingData::GPS:
		return "GPS";
	case LoggingData::FILTER:
		return "FILTER";
	default:
		return "UNKNOWN";
	}
}

void LoggingService::ProcessFlashDump()
{
	done = true;
	doneDump = false;

	constexpr uint32_t RECORD_SIZE = 20;
	constexpr uint32_t CHUNK_SIZE = 500;

	while (dumpSector < NUM_SECTORS && !doneDump)
	{ // go untill all sectors have been read or doneDump flag is triggered

		memset(sectorBuf, 0, sizeof(sectorBuf));
		MX66xxQSPI_ReadSector(sectorBuf, dumpSector, dumpOffset, CHUNK_SIZE);

		for (uint32_t i = 0; i + RECORD_SIZE <= CHUNK_SIZE; i += RECORD_SIZE) // each record is 20 bytes, Chunck size is 500 to prevent hardfault
		{
			LoggingData type = static_cast<LoggingData>(sectorBuf[i]); // get type byte first
			uint32_t timestamp;
			memcpy(&timestamp, sectorBuf + i + 1, sizeof(timestamp));
			uint8_t id = sectorBuf[i + 19];

			// parse the data back into structs

			if (type == LoggingData::IMU16G || type == LoggingData::IMU32G)
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
			else if (type == LoggingData::BARO07 || type == LoggingData::BARO11)
			{
				int32_t pressure;
				int16_t temperature;
				memcpy(&pressure, sectorBuf + i + 5, sizeof(pressure));
				memcpy(&temperature, sectorBuf + i + 9, sizeof(temperature));

				int16_t temp_c = temperature / 100;	   // integer part
				int16_t temp_frac = temperature % 100; // fractional part
				if (temp_frac < 0)
					temp_frac = -temp_frac; // handle negative temperatures

				SOAR_PRINT("%s(ID=%u) Timestamp=%lu Pressure=%ld Temp=%d.%02d\n",
						   SensorTypeName(type), id, timestamp, pressure, temp_c, temp_frac);
			}
			else if (type == LoggingData::MAG)
			{

				int32_t magX, magY, magZ;

				memcpy(&magX, sectorBuf + i + 5, sizeof(int32_t));
				memcpy(&magY, sectorBuf + i + 9, sizeof(int32_t));
				memcpy(&magZ, sectorBuf + i + 13, sizeof(int32_t));

				SOAR_PRINT("%s Timestamp=%lu Mag=[%ld,%ld,%ld]\n",
						   SensorTypeName(type), timestamp,
						   (long)magX, (long)magY, (long)magZ);
			}
		}
		/*the dump offset at this point should be 3500, this means sector is full
		 * (writtern up to 4000 bytes, if this is true set the offset back to 0 and
		 * move to the next sector, otherwise increment dumpoffset to the next chunk
		 */
		if (dumpOffset == SECTOR_READ - CHUNK_SIZE)
		{
			dumpOffset = 0;
			dumpSector++;
		}
		else
		{
			dumpOffset += 500;
		}
	}

	SOAR_PRINT("------FLASH DUMP COMPLETE------");
}

void LoggingService::StopDump()
{
	doneDump = true;
}

LoggingStatus LoggingService::LogToInternalMemory()
{

	if (loggingData.data == nullptr || loggingData.dataSize == 0)
	{
		return LoggingStatus::LOGGING_ERR;
	}

	return MemAppend(&loggingData);
}

bool LoggingService::BytesEqual(const uint8_t *a, const uint8_t *b, uint32_t n)
{

	for (uint32_t i = 0; i < n; i++)
	{

		if (a[i] != b[i])
		{
			return false;
		}
	}

	return true;
}

LoggingStatus LoggingService::MemAppend(const LoggingPacket *data)
{

	if (!data)
	{
		return LoggingStatus::LOGGING_ERR;
	}

	uint32_t size = data->dataSize;

	if (size > MAX_LOG_SIZE)
	{
		size = MAX_LOG_SIZE;
	}

	if (ramHead + MAX_LOG_SIZE > RAM_LOG_SIZE)
	{ // check if word will fit in buffer
		ramHead = 0;

		return LoggingStatus::LOG_FLASH_READY;
	}

	for (uint32_t i = 0; i < size; i++)
	{

		ramLog[ramHead++] = data->data[i];
	}

	for (uint32_t i = size; i < MAX_LOG_SIZE; i++)
	{
		ramLog[ramHead++] = 0;
	}

	return LoggingStatus::LOG_FLASH_NOT_READY;
}

/**
 * @brief Log variable-length text data with length prefix to flash
 * @param data Text data buffer
 * @param size Size of text data (must be < 65535)
 * @return LoggingStatus indicating success or state
 */
LoggingStatus LoggingService::LogText(const uint8_t *data, uint32_t size)
{
	if (!data || size == 0 || size > 65535)
	{
		return LoggingStatus::LOGGING_ERR;
	}

	// Try to append to text buffer
	return MemAppendText(data, size);
}

/**
 * @brief Append text record with 2-byte length prefix to text buffer
 * Record format: [1 byte length_high][1 byte length_low][variable data]
 * @param data Text data
 * @param size Size of data
 * @return LoggingStatus
 */
LoggingStatus LoggingService::MemAppendText(const uint8_t *data, uint32_t size)
{
	// Need 2 bytes for length + data size
	uint32_t totalSize = 2 + size;

	// If adding this record would exceed buffer, flush first
	if (textLogHead + totalSize > RAM_LOG_SIZE)
	{
		// Flush what we have
		LoggingStatus flushStatus = FlushTextBuffer();
		if (flushStatus != LoggingStatus::LOGGING_SUCCESS)
		{
			return flushStatus;
		}
		textLogHead = 0;
	}

	// Write length as big-endian uint16_t
	uint16_t len = static_cast<uint16_t>(size);
	textLogBuffer[textLogHead++] = (len >> 8) & 0xFF; // High byte
	textLogBuffer[textLogHead++] = len & 0xFF;		  // Low byte

	// Write data
	memcpy(textLogBuffer + textLogHead, data, size);
	textLogHead += size;

	return LoggingStatus::LOG_FLASH_NOT_READY;
}

/**
 * @brief Flush text buffer to external flash
 * @return LoggingStatus
 */
LoggingStatus LoggingService::FlushTextBuffer()
{
	if (textLogHead == 0)
	{
		return LoggingStatus::LOGGING_SUCCESS; // Nothing to flush
	}

	// Check if we've run out of sectors
	if (textSectorAddress >= 4096)
	{
		SOAR_PRINT("TextLog: Out of sectors\n");
		return LoggingStatus::FLASH_FULL;
	}

	// Erase sector if starting fresh
	if (textBufferPerSector == 0)
	{
		SOAR_PRINT("TextLog: Erasing sector %u\n", textSectorAddress);
		MX66xxQSPI_EraseSector(textSectorAddress);
	}

	// Calculate write offset within sector
	uint32_t writeOffset = textBufferPerSector * 500;

	// Check if this write would overflow current sector
	if (writeOffset + textLogHead > SECTOR_READ)
	{
		// Move to next sector
		textSectorAddress++;
		textBufferPerSector = 0;
		writeOffset = 0;

		if (textSectorAddress >= 4096)
		{
			SOAR_PRINT("TextLog: Out of sectors\n");
			return LoggingStatus::FLASH_FULL;
		}

		// Erase new sector
		SOAR_PRINT("TextLog: Erasing sector %u\n", textSectorAddress);
		MX66xxQSPI_EraseSector(textSectorAddress);
	}

	// Write to flash
	SOAR_PRINT("TextLog: Writing %lu bytes to sector %u, offset %lu\n",
			   (uint32_t)textLogHead, textSectorAddress, writeOffset);

	MX66xxQSPI_WriteSector(textLogBuffer, textSectorAddress, writeOffset, textLogHead);

	// Verify written data
	uint8_t verifyBuf[RAM_LOG_SIZE];
	MX66xxQSPI_ReadSector(verifyBuf, textSectorAddress, writeOffset, textLogHead);

	if (!BytesEqual(verifyBuf, textLogBuffer, textLogHead))
	{
		SOAR_PRINT("TextLog: Verification failed\n");
		return LoggingStatus::LOGGING_ERR;
	}

	// Update tracking
	textBufferPerSector++;
	textLogHead = 0;
	memset(textLogBuffer, 0, sizeof(textLogBuffer));

	return LoggingStatus::LOGGING_SUCCESS;
}

void LoggingService::DumpTextLog()
{
	SOAR_PRINT("------TEXT LOG DUMP START------\n");

	constexpr uint32_t CHUNK_SIZE = 500;
	uint8_t readBuf[CHUNK_SIZE] = {};
	uint16_t sector = 256;
	uint32_t entryCount = 0;
	bool foundAnyData = false;

	while (sector < textSectorAddress || (sector == textSectorAddress && textBufferPerSector > 0))
	{
		uint8_t chunksInSector = 8;
		if (sector == textSectorAddress)
		{
			chunksInSector = textBufferPerSector;
		}

		for (uint8_t chunk = 0; chunk < chunksInSector; ++chunk)
		{
			MX66xxQSPI_ReadSector(readBuf, sector, chunk * CHUNK_SIZE, CHUNK_SIZE);

			uint32_t index = 0;
			while (index + 2 <= CHUNK_SIZE)
			{
				uint16_t length = static_cast<uint16_t>((static_cast<uint16_t>(readBuf[index]) << 8) |
														static_cast<uint16_t>(readBuf[index + 1]));
				if (length == 0)
				{
					break;
				}

				if (index + 2 + length > CHUNK_SIZE)
				{
					break;
				}

				char text[DEBUG_PRINT_MAX_SIZE] = {};
				uint32_t copyLength = length;
				if (copyLength >= sizeof(text))
				{
					copyLength = sizeof(text) - 1;
				}

				memcpy(text, &readBuf[index + 2], copyLength);
				text[copyLength] = '\0';

				SOAR_PRINT("[TEXT %lu] %s\n", entryCount++, text);
				foundAnyData = true;
				index += 2 + length;
			}
		}

		++sector;
	}

	if (!foundAnyData)
	{
		SOAR_PRINT("[TEXT] No text log entries found\n");
	}

	SOAR_PRINT("------TEXT LOG DUMP END------\n");
}
