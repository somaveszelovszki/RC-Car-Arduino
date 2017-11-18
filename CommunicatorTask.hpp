#ifndef RC_CAR__COMMUNICATOR_TASK__HPP
#define RC_CAR__COMMUNICATOR_TASK__HPP

#include "PeriodicTask.hpp"
//#include <SoftwareSerial.h>
//#include <AltSoftSerial.h>
#include "Message.hpp"
#include "Watchdog.hpp"
#include "ByteArray.hpp"
#include "BoolArray.hpp"

namespace rc_car {
	/** @brief Handles communication with phone - sends and receives messages.
	*/
	class CommunicatorTask : public PeriodicTask {
	public:
		enum MsgAvailability {
			NOT_AVAILABLE = 0,
			AVAILABLE_LOW_PRIO,
			AVAILABLE_HIGH_PRIO
		};

	private:

		enum RecvState {
			READ_SEP, READ_CODE, READ_DATA
		};

		ByteArray<COMM_MSG_LENGTH> recvBuffer;
		int recvByteIdx;

		Message recvMsg, sendMsgs[TASK_COUNT];
		RecvState recvState = READ_SEP;
		MsgAvailability __recvAvailable[TASK_COUNT], __sendAvailable[TASK_COUNT];

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

		MsgAvailability getRecvAvailability(int taskId) const {
			return __recvAvailable[taskId];
		}

		MsgAvailability getSendAvailability(int taskId) const {
			return __sendAvailable[taskId];
		}

		void getReceivedMessage(Message& msg, int taskId);
		void setMessageToSend(const Message& msg, int taskId, MsgAvailability availability = AVAILABLE_LOW_PRIO);
	};
}

#endif // RC_CAR__COMMUNICATOR_TASK__HPP
