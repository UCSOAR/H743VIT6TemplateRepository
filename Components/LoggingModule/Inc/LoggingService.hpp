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
//#include "MX66L1G45GMI.hpp"

#define MAX_LOG_SIZE 128
#define RAM_LOG_SIZE 4096

class LoggingService{
	public:
		LoggingService(LoggingDest dest, LoggingData dataType, uint8_t* data, uint32_t dataSize);
		LoggingStatus LogData();
	private:
		LoggingStatus LogToMX66();
		LoggingStatus LogToInternalMemory();
		bool BytesEqual(const uint8_t* a, const uint8_t* b, uint32_t n);
		void MemAppend(const uint8_t *data, uint32_t size);


		uint8_t logBuffer[MAX_LOG_SIZE];
		LoggingPacket loggingData;
		Command* cm;

		//variables for internal ram buffer

		static uint8_t  ramLog[RAM_LOG_SIZE];
		static uint32_t ramHead;





};





#endif /* LOGGINGMODULE_INC_LOGGINGSERVICE_HPP_ */
