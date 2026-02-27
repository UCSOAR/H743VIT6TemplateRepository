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
//#include "MX66L1G45GMI.hpp"

#define MAX_LOG_SIZE 20 //bytes, minus one byte for priority
#define RAM_LOG_SIZE 500 //bytes

class LoggingService{
	public:
		LoggingService(LoggingDest dest, LoggingData dataType, uint8_t* data, uint32_t dataSize, LoggingPriority priority);
		LoggingStatus LogData();
	private:
		LoggingStatus LogToMX66();
		LoggingStatus LogToInternalMemory();
		bool BytesEqual(const uint8_t* a, const uint8_t* b, uint32_t n);
		LoggingStatus MemAppend(const LoggingPacket *data);

		LoggingPacket loggingData;


		//variables for internal ram buffer

		static uint8_t  ramLog[RAM_LOG_SIZE];
		static uint32_t ramHead;
		static uint16_t sectorAddress;
		static uint8_t bufferPerSector;





};





#endif /* LOGGINGMODULE_INC_LOGGINGSERVICE_HPP_ */
