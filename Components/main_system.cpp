/**
 ******************************************************************************
 * File Name          : main_system.cpp
 * Description        : This file acts as an interface supporting CubeIDE Codegen
    while having a clean interface for development.
 ******************************************************************************
*/
/* Includes -----------------------------------------------------------------*/
#include "SystemDefines.hpp"
#include "UARTDriver.hpp"

// Tasks
#include "UARTTask.hpp"
#include "CubeTask.hpp"
#include "DebugTask.hpp"
#include "IMUTask.hpp"
#include "LSM6DSOTask.hpp"
#include "mmc5983Task.hpp"
#include "BaroTask07.hpp"
#include "BaroTask11.hpp"
#include "LoggingTask.hpp"
#include "AltitudeTask.hpp"

/* Drivers ------------------------------------------------------------------*/
namespace Driver {
    UARTDriver usart8(UART8);
    UARTDriver uart7(UART7);
}

/* Interface Functions ------------------------------------------------------------*/
/**
 * @brief Main function interface, called inside main.cpp before os initialization takes place.
*/
void run_main() {
    // Init Tasks
	UARTTask::Inst().InitTask();
	CubeTask::Inst().InitTask();
	DebugTask::Inst().InitTask();
	IMUTask::Inst().InitTask();
	LSM6DSOTask::Inst().InitTask();
//    MMC5983MATask::Inst().InitTask();
    BaroTask07::Inst().InitTask();
    BaroTask11::Inst().InitTask();
    LoggingTask::Inst().InitTask();
    AltitudeTask::Inst().InitTask();

    // Print System Boot Info : Warning, don't queue more than 10 prints before scheduler starts
    SOAR_PRINT("\n-- CUBE SYSTEM --\n");
    SOAR_PRINT("System Reset Reason: [TODO]\n"); //TODO: System reset reason can be implemented via. Flash storage
    SOAR_PRINT("Current System Free Heap: %d Bytes\n", xPortGetFreeHeapSize());
    SOAR_PRINT("Lowest Ever Free Heap: %d Bytes\n\n", xPortGetMinimumEverFreeHeapSize());
    // Start the Scheduler
    // Guidelines:
    // - Be CAREFUL with race conditions after osKernelStart
    // - All uses of new and delete should be closely monitored after this point
//    SOAR_PRINT("Debug Imu 32G read");
//   	Command cmd(DATA_COMMAND, IMUTask::IMU_SAMPLE_AND_LOG);
//   	IMUTask::Inst().GetEventQueue()->Send(cmd);
//
//   	SOAR_PRINT("Debug Imu 16G read");
//	Command cmd1(DATA_COMMAND, LSM6DSOTask::IMU_SAMPLE_AND_LOG);
//	LSM6DSOTask::Inst().GetEventQueue()->Send(cmd1);
//
//	SOAR_PRINT("Debug Baro07 read");
//	Command cmd2(DATA_COMMAND, BARO07_SAMPLE_AND_LOG);
//	BaroTask07::Inst().GetEventQueue()->Send(cmd2);
//
//	 SOAR_PRINT("Debug Baro11 read");
//	Command cmd3(DATA_COMMAND, BARO11_SAMPLE_AND_LOG);
//	BaroTask11::Inst().GetEventQueue()->Send(cmd3);




    osKernelStart();

    // Should never reach here
    SOAR_ASSERT(false, "osKernelStart() failed");

    while (1)
    {
        osDelay(100);
        HAL_NVIC_SystemReset();
    }
}

