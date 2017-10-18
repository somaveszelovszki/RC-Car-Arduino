#ifndef COMMUNICATOR_TASK_HPP
#define COMMUNICATOR_TASK_HPP

#include "PeriodicTask.hpp"
#include <SoftwareSerial.h>
//#include <AltSoftSerial.h>
#include "Message.hpp"
#include "Watchdog.hpp"
#include "ByteArray.hpp"

namespace rc_car {
	/** @brief Handles communication with phone - sends and receives messages.
	*/
	class CommunicatorTask : public PeriodicTask {
	private:
		SoftwareSerial *commSerial;
		//AltSoftSerial *commSerial;

		ByteArray<COMM_MESSAGE_SIZE> recvBuffer;
		int curByteIdx;

		Message recvMessage;
		bool __available = false;
		bool hasTimedOut = false;	// TODO find a more elegant and effective way

		/*
		Reads characters from stream.
		@returns true if message end character received - in this case buffer string contains message
		*/
		bool receiveChars();

		/** @brief Parses message from buffer string and clears buffer string content.

		@param dest Will contain parsed message.
		*/
		void fetchMessage(Message& dest);

		void __initialize() override;
		void __run() override;
		void __onTimedOut() override;

	public:

		CommunicatorTask();

		Message getMessage();

		/*
			Sends message as string.
			@returns number of bytes sent
		*/
		int sendMessage(const Message& message);

		~CommunicatorTask();
	};

}
#endif	// COMMUNICATOR_TASK_HPP
