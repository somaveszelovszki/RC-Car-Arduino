#ifndef RC_CAR__COMMUNICATOR_TASK__HPP
#define RC_CAR__COMMUNICATOR_TASK__HPP

#include "PeriodicTask.hpp"
#include "Message.hpp"
#include "Watchdog.hpp"
#include "ByteArray.hpp"

namespace rc_car {
/** @brief Handles communication with phone - sends and receives messages.
*/
class CommunicatorTask : public PeriodicTask {

private:

    /** @brief States of read state-machine.
    */
    enum RecvState {
        READ_SEPARATOR, READ_CODE, READ_DATA
    };

    /** @brief Buffer for received bytes.
    */
    ByteArray<COMM_MSG_LENGTH> recvBuffer;

    /** @brief Stores buffer index of the next byte to be read.
    */
    int recvByteIdx;

    /** @brief Stores the last received message.
    */
    Message recvMsg;

    /** @brief Stores the messages to send (one for each task).
    */
    Message sendMsgs[TASK_COUNT];

    /** @brief Stores current of the message receiving operation.
    */
    RecvState recvState = READ_SEPARATOR;

    /** @brief Stores received message availabilities (one for each task).
    With this the tasks can check if there is an unread message for them.
    */
    bool __recvAvailable[TASK_COUNT];

    /** @brief Stores send message availabilities (one for each task).
    With this tasks can signal that they have a new message to send.
    */
    bool __sendAvailable[TASK_COUNT];

    /** @brief Reads characters from stream and stores them in the buffer or throws them away - according to the current state and the received byte's value

    @returns Boolean value indicating if a whole message has been received.
    */
    bool receiveChars();

    /** @brief Parses message from buffer byte array and stores it in recvMsg object.
    */
    void fetchMessage();

    /** @brief Sends task's message and updates message availability.

    @param taskId The task id.
    */
    int sendMessage(int taskId);

public:
    /** @brief Constructor - initializes parent.
    */
    CommunicatorTask() : PeriodicTask(TASK_PERIOD_TIME_COMMUNICATOR, TASK_WATCHDOG_TIMEOUT_COMMUNICATOR) {}

    /** @brief Sets pin modes and resets availabilities.
    NOTE: Compulsory TASK function - initializes task!
    */
    void initialize();

    /** @brief Receives characters, fetches message and sets message availabilities.
    NOTE: Compulsory TASK function - called in every cycle!
    */
    void run();

    /** @brief Restarts timeout check.
    NOTE: Compulsory TASK function - called when task watchdog timed out!
    */
    void onTimedOut() {
        restartTimeoutCheck();
    };

    /** @brief Gets received message availability for the given task.

    @param taskId The task id.
    @returns Boolean value indicating if a message is available for the task to read.
    */
    bool isRecvMsgAvailable(int taskId) const {
        return __recvAvailable[taskId];
    }

    /** @brief Gets send message availability for the given task.

    @param taskId The task id.
    @returns Boolean value indicating if a message for the task is set for sending but has not been sent yet.
    */
    bool isSendMsgAvailable(int taskId) const {
        return __sendAvailable[taskId];
    }

    /** @brief Gets received message and updates message availability for the given task.

    @param taskId The task id.
    @returns The received message.
    */
    const Message& getReceivedMessage(int taskId);

    /** @brief Gets message to send for the given task.

    @param taskId The task id.
    @returns The message to send.
    */
    const Message& getMessageToSend(int taskId) const {
        return sendMsgs[taskId];
    }

    /** @brief Sets message to send and updates message availability for the given task.
    NOTE: If a high priority message is in the buffer and has not been sent, will NOT override it!

    @param msg The message to send.
    @param taskId The task id.
    @returns Boolean value indicating if message has been set successfully.
    */
    bool setMessageToSend(const Message& msg, int taskId);
};
}

#endif // RC_CAR__COMMUNICATOR_TASK__HPP
