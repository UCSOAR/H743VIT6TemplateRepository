#ifndef WATCHDOGTASK_HPP
#define WATCHDOGTASK_HPP

// Initialize the independent watchdog (IWDG)
void InitWatchdog();

// Refresh/pet the watchdog. Call from tasks regularly
void PetWatchdog();

// Returns the name of the last task that petted the watchdog
const char* GetLastTaskToPetWatchdog();

#endif