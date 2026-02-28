/**
 ******************************************************************************
 * File Name          : FlightControlTask.cpp
 * Description        : Template task for flight control logic
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "FlightControlTask.hpp"
#include "SystemDefines.hpp"
#include "Command.hpp"
#include "cmsis_os.h"

/**
 * @brief Constructor, sets up task
 */
FlightControlTask::FlightControlTask()
    : Task(TASK_FLIGHT_CONTROL_QUEUE_DEPTH_OBJS),
      start(3)
{
}

/**
 * @brief Initialize the FlightControlTask
 */
void FlightControlTask::InitTask()
{
    // Make sure the task is not already initialized
    SOAR_ASSERT(rtTaskHandle == nullptr, "Cannot initialize FlightControl task twice");

    // Start the task
    BaseType_t rtValue =
        xTaskCreate((TaskFunction_t)FlightControlTask::RunTask,
                    (const char *)"FlightControlTask",
                    (uint16_t)TASK_FLIGHT_CONTROL_STACK_DEPTH_WORDS,
                    (void *)this,
                    (UBaseType_t)TASK_FLIGHT_CONTROL_PRIORITY,
                    (TaskHandle_t *)&rtTaskHandle);

    // Ensure creation succeded
    SOAR_ASSERT(rtValue == pdPASS, "FlightControlTask::InitTask() - xTaskCreate() failed");
}

/**
 * @brief Instance Run loop for the FlightControlTask
 * @param pvParams RTOS passed parameters, should not be used
 */
void FlightControlTask::Run(void *pvParams)
{
    while (1)
    {
        CheckSystemClock();

        Command cm;
        bool res = qEvtQueue->Receive(cm, 10);
        if (res)
        {
            HandleCommand(cm);
        }
    }
}

/**
 * @brief Checks elapsed system clock time and triggers logging function
 */
void FlightControlTask::CheckSystemClock()
{
    const uint32_t systemClock = osKernelSysTick();
    if ((systemClock - start) > 1)
    {
        RunLoggingFunction();
        start = systemClock;
    }
}

/**
 * @brief Placeholder logging function for flight control task
 */
void FlightControlTask::RunLoggingFunction()
{
    // TODO: Define flight control logging behavior
}

/**
 * @brief Handles a command
 * @param cm Command reference to handle
 */
void FlightControlTask::HandleCommand(Command &cm)
{
    if (cm.GetCommand() == TASK_SPECIFIC_COMMAND)
    {
        switch (cm.GetTaskCommand())
        {
        case EVENT_FLIGHT_CONTROL_UPDATE:
            // TODO: Add flight control update logic
            break;

        default:
            SOAR_PRINT("FlightControlTask - Unsupported Task Command {%d}\n", cm.GetTaskCommand());
            break;
        }
    }
    else
    {
        SOAR_PRINT("FlightControlTask - Unsupported Global Command {%d}\n", cm.GetCommand());
    }

    cm.Reset();
}
