/**
 ******************************************************************************
 * File Name          : FlightControlTask.hpp
 * Description        : Template task for flight control logic
 ******************************************************************************
 */
#ifndef CUBE_SYSTEM_FLIGHT_CONTROL_TASK_HPP_
#define CUBE_SYSTEM_FLIGHT_CONTROL_TASK_HPP_

/* Includes ------------------------------------------------------------------*/
#include "Task.hpp"
#include "SystemDefines.hpp"

/* Enums ------------------------------------------------------------------*/
enum FLIGHT_CONTROL_TASK_COMMANDS
{
    FLIGHT_CONTROL_TASK_COMMAND_NONE = 0,
    EVENT_FLIGHT_CONTROL_UPDATE
};

/* Class ------------------------------------------------------------------*/
class FlightControlTask : public Task
{
public:
    static FlightControlTask &Inst()
    {
        static FlightControlTask inst;
        return inst;
    }

    void InitTask();

protected:
    static void RunTask(void *pvParams)
    {
        FlightControlTask::Inst().Run(pvParams);
    } // Static Task Interface, passes control to the instance Run();

    void Run(void *pvParams); // Main run code
    void HandleCommand(Command &cm);
    void CheckSystemClock();
    void RunLoggingFunction();

private:
    FlightControlTask();                                     // Private constructor
    FlightControlTask(const FlightControlTask &);            // Prevent copy-construction
    FlightControlTask &operator=(const FlightControlTask &); // Prevent assignment

    uint32_t start;
};

#endif // CUBE_SYSTEM_FLIGHT_CONTROL_TASK_HPP_
