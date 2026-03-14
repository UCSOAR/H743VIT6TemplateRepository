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
#include <string>
#include "CanAutoNodeMotherboard.hpp"
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
CommandCenterTask::CommandCenterTask() :
		Task(TASK_COMMANDCENTER_QUEUE_DEPTH_OBJS) {
//    activeBoards = BoardManager::DiscoverActiveBoards();
}

/**
 * @brief Initialize the CommandCenterTask
 *        Do not modify this function aside from adding the task name
 */
void CommandCenterTask::InitTask() {
	// Make sure the task is not already initialized
	SOAR_ASSERT(rtTaskHandle == nullptr,
			"Cannot initialize watchdog task twice");

	BaseType_t rtValue = xTaskCreate(
			(TaskFunction_t) CommandCenterTask::RunTask,
			(const char*) "CommandCenterTask",
			(uint16_t) TASK_COMMANDCENTER_STACK_DEPTH_WORDS, (void*) this,
			(UBaseType_t) TASK_COMMANDCENTER_PRIORITY,
			(TaskHandle_t*) &rtTaskHandle);

	SOAR_ASSERT(rtValue == pdPASS,
			"CommandCenterTask::InitTask() - xTaskCreate() failed");
}

/**
 * @brief Instance Run loop for the Task, runs on scheduler start as long as the task is initialized.
 * @param pvParams RTOS Passed void parameters, contains a pointer to the object instance, should not be used
 */
void CommandCenterTask::Run(void *pvParams) {

	this->motherboard = new CanAutoNodeMotherboard(&hfdcan1);

	while (1) {
		/* Process commands in blocking mode */
		Command cm;
		bool res = qEvtQueue->Receive(cm);
		if (res) {
			HandleCommand(cm);
		}

		uint32_t thisTick = HAL_GetTick();
		if(logging && (thisTick - lastLog) > 1000 ) {
			lastLog = thisTick;
			char newlyJoinedBoardNames[2][MAX_NAME_STR_LEN];
			uint16_t newlyJoinedBoardNum = motherboard->GetNamesOfNewlyJoinedBoards(newlyJoinedBoardNames, sizeof(newlyJoinedBoardNames)/MAX_NAME_STR_LEN);

			for (uint16_t i = 0; i < newlyJoinedBoardNum; i++) {
				const char* thisNewName = newlyJoinedBoardNames[i];

				// Newly joined boards should begin logging immediately during this time
				if (!motherboard->SendMessageToNameByLogIndex(thisNewName, 0, START_LOGGING)) {
					SOAR_PRINT("Board %s did not respond\n",
							thisNewName);
				} else {
					daughterBoards.push_back(motherboard->GetIDOfBoardWithName(thisNewName));
				}

			}
		}
	}
}

/**
 * @brief Handles a command
 * @param cm Command reference to handle
 */
void CommandCenterTask::HandleCommand(Command &cm) {
	switch (cm.GetCommand()) {
        case DATA_COMMAND:
        // Process the command
        if (cm.GetTaskCommand() == RECEIVE_DEBUG_MSG)
         {
        	ExecuteCommand((const char*)cm.GetDataPointer());
         }


	default:
		SOAR_PRINT("CommandCenterTask - Received Unsupported Command {%d}\n",
				cm.GetCommand());
		break;
	}

	//No matter what we happens, we must reset allocated data
	cm.Reset();
}

void CommandCenterTask::ExecuteCommand(const char *msg) {



	//discover daughterboards while the task is running

	char activeBoardNames[10][MAX_NAME_STR_LEN];
	uint16_t activeBoardsNum = motherboard->GetNamesOfAllBoards(
			activeBoardNames, sizeof(activeBoardNames)/MAX_NAME_STR_LEN);

	daughterBoards.clear();

	//parse string to check if there is a start or an end
	std::string command(msg);

	//if all appears all boards get put through commands
	if (command.find("all") != std::string::npos) {
		for (uint16_t i = 0; i < activeBoardsNum; i++) {
			CanAutoNode::UniqueBoardID thisDaughterID =
					motherboard->GetIDOfBoardWithName(activeBoardNames[i]);
			if (thisDaughterID != CanAutoNode::UniqueBoardID { 0, 0, 0 }) {
				daughterBoards.push_back(thisDaughterID);
			}
		}
	} else {
		//if d2, d3 etc gets typed in the commands get sent for those boards
		for (size_t i = 0; i < activeBoardsNum; i++) {
			std::string boardName = "d" + std::to_string(i + 1);
			if (command.find(boardName) != std::string::npos) {
				daughterBoards.push_back(
						motherboard->GetIDOfBoardWithName(
								activeBoardNames[i]));
			}
		}
	}

	if (command.starts_with("start")) {

		//if not d1, d2, d3 etc it goes to error
		if (daughterBoards.empty()) {
			SOAR_PRINT("Error: enter all boards, d1, d2, etc\n");
			return;
		}

		//Loop through boards and get them to start logging
		for (auto &board : daughterBoards) {
			if (!motherboard->SendMessageToDaughterByLogIndex(board, 0, START_LOGGING)) {
				SOAR_PRINT("Board {%llu,%llu,%llu} did not respond\n", board.u0,
						board.u1, board.u2);
			}
		}

		//checking state of daughter boards and looping back
		logging = true;

	}

	else if (strcmp(msg, "end") == 0) {

		// if not d1, d2, d3 etc it goes to error
		if (daughterBoards.empty()) {
			SOAR_PRINT("Error: enter all boards, d1, d2, etc\n");
			return;
		}

		logging = false;

		//   stops logging and sends files
		for (auto &board : daughterBoards) {


			if (!motherboard->SendMessageToDaughterByLogIndex(board, 0, STOP_LOGGING)) {
				SOAR_PRINT("Board {%llu,%llu,%llu} did not respond\n", board.u0,
						board.u1, board.u2);
			}

		}

		//    recives files
		for (auto &board : daughterBoards) {

			uint16_t numberOfLogsInBoard = motherboard->GetNumberOfLogIndicesInBoard(board);

			if (!motherboard->SendMessageToDaughterByLogIndex(board, 0, SEND_FILES)) {
				SOAR_PRINT("Board {%llu,%llu,%llu} did not respond\n", board.u0,
						board.u1, board.u2);
			} else {
				//logs every log of every type on this daughter board and puts them organized into an array
				bool receiving = true;
				while(receiving) {
					uint8_t received[10000][numberOfLogsInBoard]; // not great maybe see if the file system can log continuously?
					uint16_t indices[numberOfLogsInBoard];
					memset(indices,0,sizeof(indices));

					receiving = false;
					for(uint16_t logindex = 0; logindex < numberOfLogsInBoard; logindex++) {
						if(motherboard->ReadMessageFromDaughterByLogIndex(board, logindex, received[logindex] + indices[logindex], sizeof(received[logindex])-indices[logindex])) {
							receiving = true;
						}
					}


				}

				// all your sensor data, one after the other, organized by sensor type, is all in received here!
				// you need to send this to the file system to store somewhere

				//FileSystem::ReceiveFilesFromBoard(board.GetID()); //pulls the files from the board into system
			}
		}

	}
}
