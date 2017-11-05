#ifndef COMMUNICATOR_TASK_HPP
#define COMMUNICATOR_TASK_HPP

#include "PeriodicTask.hpp"
//#include <SoftwareSerial.h>
//#include <AltSoftSerial.h>
#include "Message.hpp"
#include "Watchdog.hpp"
#include "ByteArray.hpp"

namespace rc_car {
	/** @brief Handles communication with phone - sends and receives messages.
	*/
	class CommunicatorTask : public PeriodicTask {
	private:
		//SoftwareSerial *commSerial;
		//AltSoftSerial *commSerial;

		enum RecvState {
			READ_SEPARATOR, READ_CODE, READ_DATA
		};

		ByteArray<COMM_MSG_LENGTH> recvBuffer;
		int recvByteIdx;

		Message recvMsg, sendMsgs[PT_NUM_TASKS];
		RecvState recvState = READ_SEPARATOR;
		bool __recvAvailable[PT_NUM_TASKS], __sendAvailable[PT_NUM_TASKS];
		bool __hasTimedOut = false;	// TODO find a more elegant and effective way

		/*
		Reads characters from stream.
		@returns true if message end character received - in this case buffer string contains message
		*/
		bool receiveChars();

		/** @brief Parses message from buffer byte array.
		*/
		void fetchMessage();

		int sendMessage(int taskId);

	public:
		CommunicatorTask();

		void initialize();
		void run();
		void onTimedOut();

		bool getReceivedMessage(Message& msg, int taskId);

		void setMessageToSend(const Message& msg, int taskId);
	};
}

#endif	// COMMUNICATOR_TASK_HPP
