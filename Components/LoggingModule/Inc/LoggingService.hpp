/*
 * LoggingService.hpp
 *
 *  Created on: Jan 21, 2026
 *      Author: jaddina
 */

#ifndef LOGGINGMODULE_INC_LOGGINGSERVICE_HPP_
#define LOGGINGMODULE_INC_LOGGINGSERVICE_HPP_

#include "Log.hpp"
#include "DataBroker.hpp"
#include "Command.hpp"
#include "mx66xx_qspi.hpp"
// #include "MX66L1G45GMI.hpp"

#define MAX_LOG_SIZE 20	 // bytes, minus one byte for priority
#define RAM_LOG_SIZE 500 // bytes
#define SECTOR_READ 4000
#define NUM_SECTORS 4096
#define TOTAL_LOGBUF_SLOTS (NUM_SECTORS * 8)

class LoggingService
{
public:
	LoggingService(LoggingDest dest, LoggingData dataType, uint8_t *data, uint32_t dataSize, LoggingPriority priority);
	LoggingStatus LogData();
	static void ProcessFlashDump();

	static void StopDump();

	/**
	 * @brief Log variable-length text data to flash with length prefix
	 * @param data Text data to log
	 * @param size Size of text data in bytes (must be < 65535)
	 * @return LoggingStatus indicating success or state
	 */
	static LoggingStatus LogText(const uint8_t *data, uint32_t size);

	// Explicit flush hook for debug commands or controlled checkpoints.
	static LoggingStatus FlushTextBuffer();

private:
	LoggingStatus LogToMX66();
	LoggingStatus LogToInternalMemory();
	static bool BytesEqual(const uint8_t *a, const uint8_t *b, uint32_t n);
	LoggingStatus MemAppend(const LoggingPacket *data);

	LoggingPacket loggingData;

	// variables for internal ram buffer

	static uint8_t ramLog[RAM_LOG_SIZE];
	static uint32_t ramHead;
	static uint16_t sectorAddress;
	static uint8_t bufferPerSector;
	static uint8_t sectorCount;
	static uint8_t done;
	static uint8_t doneDump;

	// Text log buffer (separate from sensor buffer to avoid mixing concerns)
	static uint8_t textLogBuffer[RAM_LOG_SIZE];
	static uint32_t textLogHead;
	static uint16_t textSectorAddress;
	static uint8_t textBufferPerSector;

	// Helper for text log management
	static LoggingStatus MemAppendText(const uint8_t *data, uint32_t size);
};

#endif /* LOGGINGMODULE_INC_LOGGINGSERVICE_HPP_ */
