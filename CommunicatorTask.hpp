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

		Message recvMsg, sendMsg;
		RecvState recvState = READ_SEPARATOR;
		bool __recvAvailable = false, __sendAvailable = false;
		bool hasTimedOut = false;	// TODO find a more elegant and effective way

		/*
		Reads characters from stream.
		@returns true if message end character received - in this case buffer string contains message
		*/
		bool receiveChars();

		/** @brief Parses message from buffer byte array.
		*/
		void fetchMessage();

		void __initialize() override;
		void __run(void *unused) override;
		void __onTimedOut() override;

		/*
		Sends message as string.
		@returns number of bytes sent
		*/
		int sendMessage();

	public:

		CommunicatorTask();

		bool getReceivedMessage(Message& msg);

		void setMessageToSend(const Message& msg);
	};

}
#endif	// COMMUNICATOR_TASK_HPP
