#ifndef SENSORPUBLISHERTASK_HPP_
#define SENSORPUBLISHERTASK_HPP_

#include "Task.hpp"
#include "SensorDataTypes.hpp"

class SensorPublisherTask : public Task {
public:
    // Singleton accessor
    static SensorPublisherTask& Inst() {
        static SensorPublisherTask inst;
        return inst;
    }

    void InitTask();

protected:
    static void RunTask(void *pvParams) {
        SensorPublisherTask::Inst().Run(pvParams);
    }

    void Run(void *pvParams);

private:
    // Private constructor (enforces singleton)
    SensorPublisherTask();

    // Prevent copy
    SensorPublisherTask(const SensorPublisherTask&);
    SensorPublisherTask& operator=(const SensorPublisherTask&);

};

#endif
