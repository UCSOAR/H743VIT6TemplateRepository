#include <WatchdogTask.hpp>
#include "stm32h7xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include <cstring>



// Internal watchdog handle
static IWDG_HandleTypeDef hiwdg;

// Tracks which task last petted the watchdog
static char lastTaskToPet[configMAX_TASK_NAME_LEN + 1] = "None";

const char* GetLastTaskToPetWatchdog()
{
    return lastTaskToPet;
}

void InitWatchdog()
{
    hiwdg.Instance = IWDG1;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
    // 4 second timeout
    hiwdg.Init.Reload = 3999; 

    if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
    {
        // stop system if watchdog cannot initialize
        while(1); 
    }
}

void PetWatchdog()
{
    // Get the current running task
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();

    if (handle != NULL)
    {
        const char* taskName = pcTaskGetName(handle);

        strncpy(lastTaskToPet, taskName, sizeof(lastTaskToPet));
        lastTaskToPet[sizeof(lastTaskToPet) - 1] = '\0';
    }

    // Refresh the independent watchdog
    HAL_IWDG_Refresh(&hiwdg);
}
