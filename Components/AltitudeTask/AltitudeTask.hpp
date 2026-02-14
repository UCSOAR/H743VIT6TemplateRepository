/**
 ******************************************************************************
 * File Name          : AltitudeTask.hpp
 * Description        :
 ******************************************************************************
 */
#ifndef CUBE_SYSTEM_ALTITUDE_TASK_HPP_
#define CUBE_SYSTEM_ALTITUDE_TASK_HPP_
/* Includes ------------------------------------------------------------------*/
#include "Task.hpp"
#include "SystemDefines.hpp"
#include "everestTaskHPP.hpp"

class AltitudeTask: public Task {
public:
	static AltitudeTask& Inst() {
		static AltitudeTask inst;
		return inst;
	}

	void InitTask();

protected:
	static void RunTask(void *pvParams) {
		AltitudeTask::Inst().Run(pvParams);
	}  // Static Task Interface, passes control to the instance Run();

	void Run(void *pvParams);  // Main run code

	void ConfigureUART();

	// Instance of everest, the entry point for our altitude prediction filter.
	// Despite the name EverestTask, everest is not a Task in itself.
	EverestTask everest;

	// structs to hold subscribed data.
	IMUData_Everest IMUData1;
	IMUData_Everest IMUData2;

	BarosData baro1;
	BarosData baro2;

	// GPS merely returns altitude.
	float gps;


	// Time since filter start in seconds. Currently the filter starts as soon as the task is instantiated.
	float currentTime = 0;


	// Implementation of DataBroker API.
	void HandleCommand(Command& cm);
	void HandleDataBrokerCommand(const Command& cm);

private:
	AltitudeTask();                             // Private constructor
	AltitudeTask(const AltitudeTask&);              // Prevent copy-construction
	AltitudeTask& operator=(const AltitudeTask&);  // Prevent assignment
};

#endif  // CUBE_SYSTEM_ALTITUDE_TASK_HPP_
