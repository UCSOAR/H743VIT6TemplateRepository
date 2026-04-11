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
#include "FilterState.hpp"

// External Tasks (to send debug commands to)

/* Macros --------------------------------------------------------------------*/

/* Structs -------------------------------------------------------------------*/

/* Constants -----------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Prototypes ----------------------------------------------------------------*/

AltitudeTask::AltitudeTask() {
	// don't know what to write for a Task constructor.
}

void AltitudeTask::HandleLaunchCommand(uint8_t state) {
	FILTER_STATE s;

	switch (state) {
	case 0:
		s = FILTER_STATE::PRE_START;
		break;
	case 1:
		s = FILTER_STATE::STARTED;
		break;
	case 2:
		s = FILTER_STATE::TAREING;
		break;
	case 3:
		s = FILTER_STATE::TARED;
		break;
	case 4:
		s = FILTER_STATE::LAUNCHED;
		break;
	case 5:
		s = FILTER_STATE::PRE_APOGEE_AIRBRAKES;
		break;
	case 6:
		s = FILTER_STATE::POST_APOGEE_AIRBRAKES;
		break;
	case 7:
		s = FILTER_STATE::POST_APOGEE_NOAIRBRAKES;
		break;
	case 10:
		s = FILTER_STATE::RESTARTING;
		break;
	case 255:
		s = FILTER_STATE::FAILED;
		break;

	default:
		s = FILTER_STATE::FAILED;
		break;
	}

	everest.setFilterState(s);
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

	IMUData_Queue = Queue(1);

	baroData_Queue = Queue(1);

	magData_Queue = Queue(1);

	GPSData_Queue = Queue(1);

	DataBroker::Subscribe<IMUData>(this, &IMUData_Queue);
	DataBroker::Subscribe<GPSData>(this, &GPSData_Queue);
	DataBroker::Subscribe<BaroData>(this, &baroData_Queue);
	DataBroker::Subscribe<MagData>(this, &magData_Queue);
}

// TODO: Only run thread when appropriate GPIO pin pulled HIGH (or by define)
/**
 *    @brief Runcode for the AltitudeTask
 */
void AltitudeTask::Run(void *pvParams) {

	DataBroker::Publish(&filterData);
	currentTime = TICKS_TO_MS(xTaskGetTickCount()) / 1000.0f;

	// skip the first iteration. The first deltatime will be enormous.
	bool firstIteration = true;

	int lastTriggerTime = -1;

	while (1) {
		if (everest.getFilterState() >= FILTER_STATE::STARTED) {
			// Time in seconds given by ticks since program execution, divided by 1000.0f for seconds.
			currentTime = TICKS_TO_MS(xTaskGetTickCount()) / 1.0f;

			// how will we handle both IMUs? Could just make these queues of length 2.

			Command IMUData_cm;
			bool IMUData_res = IMUData_Queue.Receive(IMUData_cm, 0);
			if (IMUData_res) {
				HandleCommand(IMUData_cm);
			}

			Command GPSData_cm;
			bool GPSData_res = GPSData_Queue.Receive(GPSData_cm, 0);
			if (GPSData_res) {
				HandleCommand(GPSData_cm);
			}

			Command BaroData_cm;
			bool BaroData_res = baroData_Queue.Receive(BaroData_cm, 0);
			if (BaroData_res) {
				HandleCommand(BaroData_cm);
			}

			Command MagData_cm;
			bool MagData_res = magData_Queue.Receive(MagData_cm, 0);
			if (MagData_res) {
				HandleCommand(MagData_cm);
			}

			// main wrapper for all the Prediction filter tasks. We might want to ensure data is within the same time window.

			if (everest.getFilterState() < FILTER_STATE::TARED) {
				everest.updateDeltaTime(currentTime);
				everest.initEverest();
			}

			// keep the time zeroed after tareing until ready to launch and start the filter properly.
			else if (everest.getFilterState() < FILTER_STATE::LAUNCHED) {
				everest.updateDeltaTime(currentTime);
			} else if (everest.getFilterState() >= FILTER_STATE::LAUNCHED) {
				if (firstIteration) {
					SOAR_PRINT("FILTER STARTING");
					everest.updateDeltaTime(currentTime);
					firstIteration = false;
				} else {

					std::vector<float> haloData = everest.QueueEverest(
							currentTime);

					static float lastReportTime = 0;
					static int loopCounter = 0;

					// predict 100 slices every 5 time slices.
					int t = (int) floor(currentTime);

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
						float realHz = (float) loopCounter / elapsed;
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

						Command filterCommand(DATA_BROKER_COMMAND,
								static_cast<uint16_t>(DataBrokerMessageTypes::FILTER_DATA));
						LoggingTask::Inst().GetEventQueue()->Send(
								filterCommand);
					}

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
		SOAR_PRINT("  gyroX -> %f \n", imu_data.gyro.x / 1.0f);
		SOAR_PRINT("  gyroY -> %f \n", imu_data.gyro.y / 1.0f);
		SOAR_PRINT("  gyroZ -> %f \n", imu_data.gyro.z / 1.0f);
		SOAR_PRINT("  accelX -> %f \n", imu_data.accel.x / 1.0f);
		SOAR_PRINT("  accelY -> %f \n", imu_data.accel.y / 1.0f);
		SOAR_PRINT("  accelZ -> %f \n", imu_data.accel.z / 1.0f);

		// note that imu_data will not contain magnetometer data. Everest still takes mag as part of IMUData_Everest, so we will add it when MAG_DATA is received.
		// will currentTime be subject to async problems? currentTime might not be updated by the time this is called.

		if (imu_data.id == 0) {

			IMUData1 = IMUData_Everest(currentTime, imu_data.gyro.x / 1.0f,
					imu_data.gyro.y / 1.0f, imu_data.gyro.z / 1.0f,
					imu_data.accel.x / 1.0f, imu_data.accel.y / 1.0f,
					imu_data.accel.z / 1.0f, IMUData1.magX, IMUData1.magY,
					IMUData1.magZ, 0);
			everest.IMU1_Measurements(IMUData1);

			SOAR_PRINT("  magX -> %f \n", IMUData1.magX);
			SOAR_PRINT("  magY -> %f \n", IMUData1.magY);
			SOAR_PRINT("  magZ -> %f \n", IMUData1.magZ);
		} else {
			IMUData2 = IMUData_Everest(currentTime, imu_data.gyro.x / 1.0f,
					imu_data.gyro.y / 1.0f, imu_data.gyro.z / 1.0f,
					imu_data.accel.x / 1.0f, imu_data.accel.y / 1.0f,
					imu_data.accel.z / 1.0f, IMUData2.magX, IMUData2.magY,
					IMUData2.magZ, 0);
			everest.IMU2_Measurements(IMUData2);

			SOAR_PRINT("  magX -> %f \n", IMUData2.magX);
			SOAR_PRINT("  magY -> %f \n", IMUData2.magY);
			SOAR_PRINT("  magZ -> %f \n", IMUData2.magZ);
		}

		SOAR_PRINT("--DATA_END--\n\n");
		break;
	}

	case DataBrokerMessageTypes::GPS_DATA: {
		GPSData GPS_data = DataBroker::ExtractData<GPSData>(cm);

		SOAR_PRINT("\nGPS DATA :\n");

		// Raw NMEA buffer
		SOAR_PRINT("  Buffer        : %s\n", GPS_data.buffer_);

		// Timestamp
		SOAR_PRINT("  Time          : %lu\n", GPS_data.time_);

		// Latitude
		SOAR_PRINT("  Latitude      : %d\n", GPS_data.latitude_.degrees_); // or however LatLongType stores it

		// Longitude
		SOAR_PRINT("  Longitude     : %d\n", GPS_data.longitude_.minutes_);

		// Antenna Altitude
		SOAR_PRINT("  Antenna Alt   : %d %c\n",
				GPS_data.antennaAltitude_.altitude_,
				GPS_data.antennaAltitude_.unit_);

		// Geoid Altitude
		SOAR_PRINT("  Geoid Alt     : %d %c\n",
				GPS_data.geoidAltitude_.altitude_,
				GPS_data.geoidAltitude_.unit_);

		// Total Altitude
		SOAR_PRINT("  Total Alt     : %d %c\n",
				GPS_data.totalAltitude_.altitude_,
				GPS_data.totalAltitude_.unit_);

		SOAR_PRINT("--DATA_END--\n\n");

		if (GPS_data.totalAltitude_.altitude_ > 999999) {
			everest.GPS_Measurements(GPS);
			everest.halo.gpsAvailable = 0;
		}

		GPS = GPS_data.totalAltitude_.altitude_;
		everest.GPS_Measurements(GPS);

		break;
	}

	case DataBrokerMessageTypes::MAG_DATA: {
		MagData mag_data = DataBroker::ExtractData<MagData>(cm);

		SOAR_PRINT("\n MAG DATA : \n");
		SOAR_PRINT("  X -> %f \n", mag_data.magX / 1.0f);
		SOAR_PRINT("  Y -> %f \n", mag_data.magY / 1.0f);
		SOAR_PRINT("  Z -> %f \n", mag_data.magZ / 1.0f);
		SOAR_PRINT("--DATA_END--\n\n");

		if (mag_data.magX == 0 && mag_data.magY == 0 && mag_data.magZ) {
			return;
		}

		//update IMU data with new mag measurements
		IMUData1 = IMUData_Everest(currentTime, IMUData1.gyroX, IMUData1.gyroY,
				IMUData1.gyroZ, IMUData1.accelX, IMUData1.accelY,
				IMUData1.accelZ, mag_data.magX / 1.0f, mag_data.magY / 1.0f,
				mag_data.magZ / 1.0f, 0);
		everest.IMU1_Measurements(IMUData1);

		// imudata2 is reduplicated for now...
		IMUData2 = IMUData_Everest(currentTime, IMUData2.gyroX, IMUData2.gyroY,
				IMUData2.gyroZ, IMUData2.accelX, IMUData2.accelY,
				IMUData2.accelZ, mag_data.magX / 1.0f, mag_data.magY / 1.0f,
				mag_data.magZ / 1.0f, 0);
		everest.IMU2_Measurements(IMUData2);

		break;
	}

	case DataBrokerMessageTypes::BARO_DATA: {
		BaroData baro_data = DataBroker::ExtractData<BaroData>(cm);

		SOAR_PRINT("\n BARO DATA : \n");
		SOAR_PRINT("  Baro ID -> %d \n", baro_data.id);
		SOAR_PRINT("  Baro -> %f \n", baro_data.pressure / 1.0f);
		SOAR_PRINT("--DATA_END--\n\n");

		if (baro_data.pressure == 0.0f) {
			return;
		}

		// BaroTask07
		if (baro_data.id == 0) {
			baro1 = { currentTime, baro_data.pressure / 1.0f, 0, 0 };
			everest.Baro1_Measurements(baro1);
		} else {
			// BaroTask11
			baro2 = { currentTime, baro_data.pressure / 1.0f, 0, 0 };
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

