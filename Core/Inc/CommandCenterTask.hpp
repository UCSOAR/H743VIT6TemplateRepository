/**
 ********************************************************************************
 * @file    ${file_name}
 * @author  ${user}
 * @date    ${date}
 * @brief   This is a template header file to create a new task in our firmware
 * 
 * Setup Steps
 * 1. Define the Task Queue Depth in SystemDefines.hpp
 * 2. Define the Task Stack Depth in SystemDefines.hpp
 * 3. Define the Task Priority in SystemDefines.hpp
 * 4. Replace all placeholders marked with a $ sign
 ********************************************************************************
 */

 #ifndef CCENTER_TASK_HPP_
 #define CCENTER_TASK_HPP_
 
 /************************************
  * INCLUDES
  ************************************/
 #include "Task.hpp"
 #include "SystemDefines.hpp"
 #include "CanAutoNodeMotherboard.hpp"
 #include <vector>
 
 /************************************
  * MACROS AND DEFINES
  ************************************/
 
 /************************************
  * TYPEDEFS
  ************************************/
 extern FDCAN_HandleTypeDef hfdcan1;
 
 enum CommandCenterTaskCommandType {
	 RECEIVE_DEBUG_MSG
 };

	const uint8_t START_LOGGING[] = {0};
	const uint8_t STOP_LOGGING[] = {1};
	const uint8_t SEND_FILES[] = {2};

 /************************************
  * CLASS DEFINITIONS
  ************************************/
 class CommandCenterTask : public Task
 {
 public:
     static CommandCenterTask& Inst() {
         static CommandCenterTask inst;
         return inst;
     }
 
     void InitTask();
 
 protected:
     static void RunTask(void* pvParams) { CommandCenterTask::Inst().Run(pvParams); } // Static Task Interface, passes control to the instance Run();
     void Run(void * pvParams); // Main run code
     void HandleCommand(Command& cm);
     void ExecuteCommand(const char* msg);

     CanAutoNodeMotherboard* motherboard = nullptr;

     std::vector<CanAutoNode::UniqueBoardID> daughterBoards;

     bool logging = false;

     uint32_t lastLog = 0;
 
 private:
     // Private Functions
     CommandCenterTask();        // Private constructor
     CommandCenterTask(const CommandCenterTask&);                        // Prevent copy-construction
     CommandCenterTask& operator=(const CommandCenterTask&);            // Prevent assignment
 };
 
 /************************************
  * FUNCTION DECLARATIONS
  ************************************/
 
 #endif /* ${include_guard_symbol} */