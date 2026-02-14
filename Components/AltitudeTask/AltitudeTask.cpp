/**
 ******************************************************************************
 * File Name          : AltitudeTask.cpp
 * Description        : Altitude Prediction Filter
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "AltitudeTask.hpp"
#include "stm32h7xx_hal.h"
#include "SensorDataTypes.hpp"
#include "DataBroker.hpp"
#include "DataBrokerMessageTypes.hpp"
#include "Publisher.hpp"

// External Tasks (to send debug commands to)

/* Macros --------------------------------------------------------------------*/

/* Structs -------------------------------------------------------------------*/

/* Constants -----------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Prototypes ----------------------------------------------------------------*/

AltitudeTask::AltitudeTask() {
	// don't know what to write for a Task constructor.
}

/**
 * @brief Init task for RTOS
 */
void AltitudeTask::InitTask() {
	// Make sure the task is not already initialized
	SOAR_ASSERT(rtTaskHandle == nullptr,
			"Cannot initialize Altitude task twice");

	// Start the task
	BaseType_t rtValue = xTaskCreate((TaskFunction_t) AltitudeTask::RunTask,
			(const char*) "AltitudeTask",
			(uint16_t) TASK_ALTITUDE_STACK_DEPTH_WORDS, (void*) this,
			(UBaseType_t) TASK_ALTITUDE_PRIORITY,
			(TaskHandle_t*) &rtTaskHandle);

	// Ensure creation succeeded
	SOAR_ASSERT(rtValue == pdPASS,
			"AltitudeTask::InitTask - xTaskCreate() failed");

}

// TODO: Only run thread when appropriate GPIO pin pulled HIGH (or by define)
/**
 *    @brief Runcode for the AltitudeTask
 */
void AltitudeTask::Run(void *pvParams) {

	// Task subscribes to Data using DataBroker.
	// Should we subscribe to each sensor individually?


	// Note, if Subscribe is called and the publisher is a nullptr, SOAR_ASSERT will be called, which in debug mode will actually restart the program!
	DataBroker::Subscribe<IMUData>(this);
	DataBroker::Subscribe<GPSData>(this);
	DataBroker::Subscribe<MagData>(this);
	DataBroker::Subscribe<BaroData>(this);


	FilterData haloOutput;
	DataBroker::Publish(&haloOutput);


	while (1) {
		// Time in seconds given by ticks since program execution, divided by 1000.0f for seconds.
		currentTime = TICKS_TO_MS(xTaskGetTickCount()) / 1000.0f;

		DataBroker::Subscribe<IMUData>(this);
		DataBroker::Subscribe<GPSData>(this);
		DataBroker::Subscribe<MagData>(this);
		DataBroker::Subscribe<BaroData>(this);

		/*  Should be Receive. ReceiveWait is blocking! */
		uint32_t timeout = 10; // t0 ms wait

		Command cm;
		bool res = qEvtQueue->Receive(cm, timeout);
		if (res) {
			HandleCommand(cm);
		}

		// main wrapper for all the Prediction filter tasks. We might want to ensure data is within the same time window.
		std::vector<float> haloData = everest.QueueEverest(currentTime);
		if (haloData.size() > 0) {
			SOAR_PRINT("HALO: \n");
			SOAR_PRINT("ALT: %d\n", haloData.at(0));
			SOAR_PRINT("VELO: %d\n", haloData.at(1));
			SOAR_PRINT("ACCEL: %d\n", haloData.at(2));

			haloOutput = { haloData.at(0), haloData.at(1), haloData.at(2) };
		}

		// we dont have a publisher for this type yet!
		// DataBroker::Publish<FilterData>(&haloOutput);
	}
}

/**
 * @brief Handles a command
 * @param cm Command reference to handle
 */
void AltitudeTask::HandleCommand(Command &cm) {
	switch (cm.GetCommand()) {
	case DATA_BROKER_COMMAND:
		HandleDataBrokerCommand(cm);
		break;

	default:
		SOAR_PRINT("PubSubReceive - Received Unsupported Command {%d}\n",
				cm.GetCommand());
		break;
	}

	// No matter what we happens, we must reset allocated data
	cm.Reset();
}

/**
 * @brief Handle all data broker commands
 * @param cm The command object with the data
 *            Use cm.GetTaskCommand() to get the message type
 *              Message types must be cast back into DataBrokerMessageTypes enum
 *            Use cm.GetDataPointer() to get the pointer to the data
 */
void AltitudeTask::HandleDataBrokerCommand(const Command &cm) {
	DataBrokerMessageTypes messageType = DataBroker::getMessageType(cm);
	switch (messageType) {
	case DataBrokerMessageTypes::IMU_DATA: {
		IMUData imu_data = DataBroker::ExtractData<IMUData>(cm);

		// How will we identify each IMU?
		SOAR_PRINT("\n IMU DATA : \n");
		SOAR_PRINT("  X -> %d \n", imu_data.accel.x);
		SOAR_PRINT("  Y -> %d \n", imu_data.accel.y);
		SOAR_PRINT("  Z -> %d \n", imu_data.accel.z);
		SOAR_PRINT("--DATA_END--\n\n");

		// note that imu_data will not contain magnetometer data. Everest still takes mag as part of IMUData_Everest, so we will add it when MAG_DATA is received.
		// will currentTime be subject to async problems? currentTime might not be updated by the time this is called.
		IMUData1 = IMUData_Everest(currentTime, imu_data.gyro.x, imu_data.gyro.y,
				imu_data.gyro.z, imu_data.accel.x, imu_data.accel.y,
				imu_data.accel.z, IMUData1.magX, IMUData1.magY, IMUData1.magZ);
		everest.IMU1_Measurements(IMUData1);

		// imudata2 is reduplicated for now...
		IMUData2 = IMUData_Everest(currentTime, imu_data.gyro.x, imu_data.gyro.y,
				imu_data.gyro.z, imu_data.accel.x, imu_data.accel.y,
				imu_data.accel.z, IMUData1.magX, IMUData1.magY, IMUData1.magZ);
		everest.IMU2_Measurements(IMUData2);

		break;
	}

	case DataBrokerMessageTypes::GPS_DATA: {
		GPSData gps_data = DataBroker::ExtractData<GPSData>(cm);

		gps = gps_data.gps;
		everest.GPS_Measurements(gps);

		break;
	}

	case DataBrokerMessageTypes::MAG_DATA:{
		MagData mag_data = DataBroker::ExtractData<MagData>(cm);

		//update IMU data with new mag measurements
		IMUData1 = IMUData_Everest(currentTime, IMUData1.gyroX, IMUData1.gyroY,
				IMUData1.gyroZ, IMUData1.accelX, IMUData1.accelY,
				IMUData1.accelZ, mag_data.rawX, mag_data.rawY, mag_data.rawZ);
		everest.IMU1_Measurements(IMUData1);

		// imudata2 is reduplicated for now...
		IMUData2 = IMUData_Everest(currentTime, IMUData2.gyroX, IMUData2.gyroY,
				IMUData2.gyroZ, IMUData2.accelX, IMUData2.accelY,
				IMUData2.accelZ, mag_data.rawX, mag_data.rawY, mag_data.rawZ);
		everest.IMU2_Measurements(IMUData2);


		break;
	}

	case DataBrokerMessageTypes::BARO_DATA: {
		BaroData baro_data = DataBroker::ExtractData<BaroData>(cm);

	    baro1 = {currentTime, baro_data.pressure, 0, 0};
	    everest.Baro1_Measurements(baro1);

	    baro2 = {currentTime, baro_data.pressure, 0, 0};
	    everest.Baro2_Measurements(baro2);

		break;
	}

	case DataBrokerMessageTypes::INVALID:
		[[fallthrough]];
	default:
		break;
	}
}

