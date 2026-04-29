#ifndef WATCHDOGTASK_HPP
#define WATCHDOGTASK_HPP

class WatchdogTask
{
public:
    static WatchdogTask& Inst();

    void InitTask();
    void Pet();

    const char* GetLastTaskToPet();

private:
    WatchdogTask() = default;
};

#endif
