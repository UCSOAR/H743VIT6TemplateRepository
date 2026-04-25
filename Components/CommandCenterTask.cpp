/**
 ********************************************************************************
 * @file    ${file_name}
 * @author  ${user}
 * @date    ${date}
 * @brief   This is a template source file to create a new task in our firmware
 * 
 * Setup Steps
 * 1. Define the Task Queue Depth in SystemDefines.hpp
 * 2. Define the Task Stack Depth in SystemDefines.hpp
 * 3. Define the Task Priority in SystemDefines.hpp
 * 4. Replace all placeholders marked with a $ sign
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "CommandCenterTask.hpp"
#include "SystemDefines.hpp"
#include "DebugTask.hpp"
#include "CanAutoNode.hpp"
#include "CanAutoNodeMotherBoard.hpp"
#include <string>
#include <algorithm>
#include <vector>


/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * VARIABLES
 ************************************/


/************************************
 * FUNCTION DECLARATIONS
 ************************************/

/************************************
 * FUNCTION DEFINITIONS
 ************************************/

/**
 * @brief Constructor for CommandCenterTask
 */
CommandCenterTask::CommandCenterTask()
    : Task(TASK_COMMANDCENTER_QUEUE_DEPTH_OBJS)
{
    activeBoards = CanAutoNode::GetNamesOfAllBoards();
}
/**
 * @brief Initialize the CommandCenterTask
 *        Do not modify this function aside from adding the task name
 */
void CommandCenterTask::InitTask()
{
    // Make sure the task is not already initialized
    SOAR_ASSERT(rtTaskHandle == nullptr, "Cannot initialize watchdog task twice");

    BaseType_t rtValue =
		xTaskCreate(
			(TaskFunction_t)CommandCenterTask::RunTask,
			"CommandCenterTask",
			(uint16_t)TASK_COMMANDCENTER_STACK_DEPTH_WORDS,
			(void*)this,
			(UBaseType_t)TASK_COMMANDCENTER_PRIORITY,
			(TaskHandle_t*)&rtTaskHandle
    );

                SOAR_ASSERT(rtValue == pdPASS, "CommandCenterTask::InitTask() - xTaskCreate() failed");
}

/**
 * @brief Instance Run loop for the Task, runs on scheduler start as long as the task is initialized.
 * @param pvParams RTOS Passed void parameters, contains a pointer to the object instance, should not be used
 */
void CommandCenterTask::Run(void * pvParams)
{

    while (1) {
        // Process commands in blocking mode
        Command cm;
        bool res = qEvtQueue->ReceiveWait(cm);
        if(res) {
            HandleCommand(cm);
        }
    }
}

/**
 * @brief Handles a command
 * @param cm Command reference to handle
 */
//handles the command and decides to process or ignore
void CommandCenterTask::HandleCommand(Command& cm)
{
    switch (cm.GetCommand()) {
       case DATA_COMMAND:
       // Process the command
       if (cm.GetTaskCommand() == EVENT_DEBUG_RX_COMPLETE)
        {
     ExecuteCommand((const char*)commandBuffer);
   }
       break;

    default:
        SOAR_PRINT("CommandCenterTask - Received Unsupported Command {%d}\n", cm.GetCommand());
        break;
    }

    //No matter what we happens, we must reset data
    cm.Reset();
}

   void CommandCenterTask::ExecuteCommand(const char* msg)
   {

       //discover daughterboards while the task is running
      activeBoards = CanAutoNode::GetNamesOfAllBoards();

       //parse string to check if there is a start or an end
      std::string command(msg);

      //find which daughterboards are chosen
      std::vector<CanAutoNode::UniqueBoardID> daughterBoards;

      // start command
      if (command.find("start") == 0) {

          //if all appears all boards get put through commands
          if (command.find("all") != std::string::npos) {
              for (auto& board : activeBoards) {
                  daughterBoards.push_back(board.uniqueID);
              }
          } else {
              //if d2, d3 etc gets typed in the commands get sent for those boards
              for (size_t i = 0; i < activeBoards.size(); i++) {
                  std::string boardName = "d" + std::to_string(i + 1);
                  if (command.find(boardName) != std::string::npos) {
                      daughterBoards.push_back(activeBoards[i].uniqueID);
                  }
              }
          }

          //if not d1, d2, d3 etc it goes to error
          if (daughterBoards.empty()) {
              SOAR_PRINT("Error: enter all boards, d1, d2, etc\n");
              return;
          }

          //Loop through boards
           for (auto& board : activeBoards) {
               if (std::find(daughterBoards.begin(), daughterBoards.end(), board.uniqueID) != daughterBoards.end()) {
                //sends a can message
                   CanAutoNodeMotherboard::SendMessageToNameByLogIndex(board.nodeName, START_LOGGING, msg);
                   if (!CanAutoNodeMotherboard::CheckCANCommands()) {
                       SOAR_PRINT("Board %d did not respond\n", board.uniqueID);
                   }
               }
           }

          //checking state of daughter boards and looping back
           bool ContinueLogging = true;
           while (ContinueLogging) {
               for (auto& board : activeBoards) {
                   if (std::find(daughterBoards.begin(), daughterBoards.end(), board.uniqueID) != daughterBoards.end()) {
                       CanAutoNodeMotherboard::SendMessageToNameByLogIndex(board.nodeName, START_LOGGING, msg);
                       if (!CanAutoNodeMotherboard::CheckCANCommands()) {
                           SOAR_PRINT("Board %d did not respond to state check\n", board.uniqueID);
                       }
                   }
               }

              vTaskDelay(pdMS_TO_TICKS(1000)); //wait 1 second between the next check
          }
      }
   }

//       end command
      else if (command.find("end") == 0) {

          //if all appears all boards get put through commands
          if (command.find("all") != std::string::npos) {
              for (auto& board : activeBoards) {
                  daughterBoards.push_back(board.uniqueID);
              }
          } else {
              //if d2, d3 etc gets typed in the commands get sent for those boards
              for (size_t i = 0; i < activeBoards.size(); i++) {
                  std::string boardName = "d" + std::to_string(i + 1);
                  if (command.find(boardName) != std::string::npos) {
                      daughterBoards.push_back(activeBoards[i].uniqueID);
                  }
              }
          }

          //if not d1, d2, d3 etc it goes to error
          if (daughterBoards.empty()) {
              SOAR_PRINT("Error: enter all boards, d1, d2, etc\n");
              return;
          }

          //stops logging and sends files
           for (auto& board : activeBoards) {
               if (std::find(daughterBoards.begin(), daughterBoards.end(), board.uniqueID) != daughterBoards.end()) {
                   CanAutoNodeMotherboard::SendMessageToNameByLogIndex(board.nodeName, STOP_LOGGING, msg);
                   if (!CanAutoNodeMotherboard::CheckCANCommands()) {
                       SOAR_PRINT("Board %d did not respond to STOP_Logging\n", board.uniqueID);
                   }
               }

          //receive files
          for (auto& board : activeBoards) {
               if (std::find(daughterBoards.begin(), daughterBoards.end(), board.uniqueID) != daughterBoards.end()) {
                   CanAutoNodeMotherboard::SendMessageToNameByLogIndex(board.nodeName, SEND_FILES, msg);
                   if (!CanAutoNodeMotherboard::CheckCANCommands()) {
                       SOAR_PRINT("Board %d failed to send files\n", board.uniqueID);
                   } else {
                       CanAutoNodeMotherboard::ReadMessageFromDaughterByLogIndex(board.uniqueID); //pulls the files from the board into system
                   }
               }
           }
       }
   	}
