/*
 * Log.hpp
 *
 *  Created on: Jan 21, 2026
 *      Author: jaddina
 */

#ifndef LOGGINGMODULE_INC_LOG_HPP_
#define LOGGINGMODULE_INC_LOG_HPP_

#include <cstdint>
#include <cstddef>



enum class LoggingStatus
{
	LOGGING_SUCCESS,
	LOGGING_ERR,

};

enum class LoggingDest
{
	FLASH_INTERN,
	FLASH_EXTERN,
	FILE_SYSTEM,
	DMA,
};

enum class LoggingData
{
	IMU32G,
	IMU16G,
	MAG,
	BARO07,
	BARO11,
	GPS,
	FILTER

};

struct LoggingPacket{

	LoggingDest dest;
	LoggingData dataType;
	const uint8_t* data;
	uint32_t dataSize;

};


#endif /* LOGGINGMODULE_INC_LOG_HPP_ */
