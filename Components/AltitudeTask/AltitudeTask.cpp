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
#include "LoggingTask.hpp"

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

	DataBroker::Subscribe<IMUData>(this);
	DataBroker::Subscribe<GpsData>(this);
	DataBroker::Subscribe<BaroData>(this);
	DataBroker::Subscribe<MagData>(this);
}

// TODO: Only run thread when appropriate GPIO pin pulled HIGH (or by define)
/**
 *    @brief Runcode for the AltitudeTask
 */
void AltitudeTask::Run(void *pvParams) {

	DataBroker::Publish(&filterData);
	currentTime = TICKS_TO_MS(xTaskGetTickCount()) /1.0f;

	// launch will be in 10 mins from startup.
	float launchTime = currentTime + 0.2 * 60.0f;

	// skip the first iteration. The first deltatime will be enormous.
	bool firstIteration = true;

	int lastTriggerTime = -1;


	while (1) {
		// Time in seconds given by ticks since program execution, divided by 1000.0f for seconds.
		currentTime = TICKS_TO_MS(xTaskGetTickCount()) /1.0f;


		Command cm;
		bool res = qEvtQueue->Receive(cm, 0);
		if (res) {
			HandleCommand(cm);
		}

		// main wrapper for all the Prediction filter tasks. We might want to ensure data is within the same time window.

		if (everest.everestInitialized == 0) {
			everest.updateDeltaTime(currentTime);
		    everest.initEverest();
		}

		// keep the time zeroed after tareing until ready to launch and start the filter properly.
		else if (currentTime < launchTime) {
			everest.updateDeltaTime(currentTime);
		} else if (currentTime >= launchTime) {
			if (firstIteration) {
				SOAR_PRINT("FILTER STARTING");
				everest.updateDeltaTime(currentTime);
				firstIteration = false;
			} else {

				std::vector<float> haloData = everest.QueueEverest(currentTime);

				static float lastReportTime = 0;
				static int loopCounter = 0;

			    // predict 100 slices every 5 time slices.
			    int t = (int)floor(currentTime);

			    /*
			    if (t % 5 == 0 && t != lastTriggerTime) {
			    	VectorXf prediction = everest.halo.predictNStates(100);
					lastTriggerTime = t;
					filterData.altPredicted = prediction(0);
					filterData.veloPredicted = prediction(1);
					filterData.accelPredicted = prediction(2);
					filterData.timePredicted = currentTime + 100 * everest.deltaTime;
					SOAR_PRINT("HALO PREDICTION: %f\n", haloData.at(0));
					SOAR_PRINT("Time: %f\n", currentTime + 100 * everest.deltaTime);
					SOAR_PRINT("ALT: %f\n",  prediction(0));
					SOAR_PRINT("VELO: %f\n", prediction(1));
					SOAR_PRINT("ACCEL: %f\n", prediction(2));

			    }*/

				loopCounter++; // Increment every loop iteration

				float elapsed = currentTime - lastReportTime;

				if (elapsed >= 1.0f) { // Every 1 second
					float realHz = (float)loopCounter / elapsed;
					// SOAR_PRINT("ALT_TASK REAL-TIME ODR: %d Hz\n", realHz);

					loopCounter = 0;
					lastReportTime = currentTime;
				}

				if (haloData.size() > 0) {

					SOAR_PRINT("HALO: \n");
					SOAR_PRINT("ALT: %f\n", haloData.at(0));
					SOAR_PRINT("VELO: %f\n", haloData.at(1));
					SOAR_PRINT("ACCEL: %f\n", haloData.at(2));

					filterData.alt = haloData.at(0);
					filterData.velo = haloData.at(1);
					filterData.accel = haloData.at(2);

					DataBroker::Publish(&filterData);

					Command filterCommand(DATA_BROKER_COMMAND, static_cast<uint16_t>(DataBrokerMessageTypes::FILTER_DATA));
					LoggingTask::Inst().GetEventQueue()->Send(filterCommand);
				}

			}
		}

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
		SOAR_PRINT("Altitude Task - Received Unsupported Command {%d}\n",
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


		SOAR_PRINT("\n IMU DATA : \n");
		SOAR_PRINT("IMU id=%d\n", imu_data.id);
		SOAR_PRINT("  gyroX -> %f \n", imu_data.gyro.x /1.0f);
		SOAR_PRINT("  gyroY -> %f \n", imu_data.gyro.y /1.0f);
		SOAR_PRINT("  gyroZ -> %f \n", imu_data.gyro.z /1.0f);
		SOAR_PRINT("  accelX -> %f \n", imu_data.accel.x /1.0f);
		SOAR_PRINT("  accelY -> %f \n", imu_data.accel.y /1.0f);
		SOAR_PRINT("  accelZ -> %f \n", imu_data.accel.z /1.0f);






		// note that imu_data will not contain magnetometer data. Everest still takes mag as part of IMUData_Everest, so we will add it when MAG_DATA is received.
		// will currentTime be subject to async problems? currentTime might not be updated by the time this is called.

		if (imu_data.id == 0) {

			IMUData1 = IMUData_Everest(currentTime, imu_data.gyro.x /1.0f, imu_data.gyro.y /1.0f,
					imu_data.gyro.z /1.0f, imu_data.accel.x /1.0f, imu_data.accel.y /1.0f,
					imu_data.accel.z /1.0f, IMUData1.magX, IMUData1.magY, IMUData1.magZ, 0);
			everest.IMU1_Measurements(IMUData1);

			SOAR_PRINT("  magX -> %f \n", IMUData1.magX);
			SOAR_PRINT("  magY -> %f \n", IMUData1.magY);
			SOAR_PRINT("  magZ -> %f \n", IMUData1.magZ);
		} else {
			IMUData2 = IMUData_Everest(currentTime, imu_data.gyro.x /1.0f, imu_data.gyro.y /1.0f,
								imu_data.gyro.z /1.0f, imu_data.accel.x /1.0f, imu_data.accel.y /1.0f,
								imu_data.accel.z /1.0f, IMUData2.magX, IMUData2.magY, IMUData2.magZ, 0);
			everest.IMU2_Measurements(IMUData2);

			SOAR_PRINT("  magX -> %f \n", IMUData2.magX);
			SOAR_PRINT("  magY -> %f \n", IMUData2.magY);
			SOAR_PRINT("  magZ -> %f \n", IMUData2.magZ);
		}

		SOAR_PRINT("--DATA_END--\n\n");
		break;
	}

	case DataBrokerMessageTypes::GPS_DATA: {
		GpsData gps_data = DataBroker::ExtractData<GpsData>(cm);

		SOAR_PRINT("\nGPS DATA :\n");

		// Raw NMEA buffer
		SOAR_PRINT("  Buffer        : %s\n", gps_data.buffer_);

		// Timestamp
		SOAR_PRINT("  Time          : %lu\n", gps_data.time_);

		// Latitude
		SOAR_PRINT("  Latitude      : %d\n", gps_data.latitude_.degrees_); // or however LatLongType stores it

		// Longitude
		SOAR_PRINT("  Longitude     : %d\n", gps_data.longitude_.minutes_);

		// Antenna Altitude
		SOAR_PRINT("  Antenna Alt   : %d %c\n", gps_data.antennaAltitude_.altitude_, gps_data.antennaAltitude_.unit_);

		// Geoid Altitude
		SOAR_PRINT("  Geoid Alt     : %d %c\n", gps_data.geoidAltitude_.altitude_, gps_data.geoidAltitude_.unit_);

		// Total Altitude
		SOAR_PRINT("  Total Alt     : %d %c\n", gps_data.totalAltitude_.altitude_, gps_data.totalAltitude_.unit_);

		SOAR_PRINT("--DATA_END--\n\n");

		if (gps_data.totalAltitude_.altitude_ > 999999) {
			everest.GPS_Measurements(gps);
			everest.halo.gpsAvailable = 0;
		}

		gps = gps_data.totalAltitude_.altitude_;
		everest.GPS_Measurements(gps);

		break;
	}

	case DataBrokerMessageTypes::MAG_DATA:{
		MagData mag_data = DataBroker::ExtractData<MagData>(cm);


		SOAR_PRINT("\n MAG DATA : \n");
		SOAR_PRINT("  X -> %f \n", mag_data.rawX /1.0f);
		SOAR_PRINT("  Y -> %f \n", mag_data.rawY /1.0f);
		SOAR_PRINT("  Z -> %f \n", mag_data.rawZ /1.0f);
		SOAR_PRINT("--DATA_END--\n\n");


		if (mag_data.rawX == 0  && mag_data.rawY == 0 && mag_data.rawZ) {
			return;
		}

		//update IMU data with new mag measurements
		IMUData1 = IMUData_Everest(currentTime, IMUData1.gyroX, IMUData1.gyroY,
				IMUData1.gyroZ, IMUData1.accelX, IMUData1.accelY,
				IMUData1.accelZ, mag_data.rawX /1.0f, mag_data.rawY /1.0f, mag_data.rawZ /1.0f, 0);
		everest.IMU1_Measurements(IMUData1);

		// imudata2 is reduplicated for now...
		IMUData2 = IMUData_Everest(currentTime, IMUData2.gyroX, IMUData2.gyroY,
				IMUData2.gyroZ, IMUData2.accelX, IMUData2.accelY,
				IMUData2.accelZ, mag_data.rawX /1.0f, mag_data.rawY /1.0f, mag_data.rawZ /1.0f, 0);
		everest.IMU2_Measurements(IMUData2);


		break;
	}

	case DataBrokerMessageTypes::BARO_DATA: {
		BaroData baro_data = DataBroker::ExtractData<BaroData>(cm);



		SOAR_PRINT("\n BARO DATA : \n");
		SOAR_PRINT("  Baro -> %f \n", baro_data.pressure/1.0f);
		SOAR_PRINT("--DATA_END--\n\n");


		if (baro_data.pressure == 0.0f) {
			return;
		}

		// BaroTask07
		if (baro_data.id == 0) {
			baro1 = {currentTime, baro_data.pressure/1.0f, 0, 0};
			everest.Baro1_Measurements(baro1);
		} else {
			// BaroTask11
			baro2 = {currentTime, baro_data.pressure/1.0f, 0, 0};
			everest.Baro2_Measurements(baro2);
		}

		break;
	}

	case DataBrokerMessageTypes::INVALID:
		[[fallthrough]];
	default:
		break;
	}
}

