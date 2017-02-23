
#include "Communicator.h"
#include "DriveController.h"

#include <stdlib.h>


Communicator* communicator = new Communicator();
DriveController* driveController = new DriveController();

Command* receivedCommand;

Servo servo;

void setup() {

	Serial.begin(9600);

	//while (!Serial) {}

	Serial.println("setup...");

	communicator->initialize();
	driveController->initialize();

}

void loop() {
	
	// return value is true -> full command received
	if (communicator->receiveChar()) {
		receivedCommand = communicator->fetchCommand();

		Serial.println(receivedCommand->toString());

		switch (receivedCommand->getCode()) {

		case Command::CODE::Speed:
			driveController->executeCommand_Speed(receivedCommand);
			break;

		case Command::CODE::SteeringAngle:
			driveController->executeCommand_SteeringAngle(receivedCommand);
			break;

		case Command::CODE::ServoRecalibrate:
			driveController->executeCommand_ServoRecalibrate(receivedCommand);
			break;
		}

		delete receivedCommand;

		communicator->reset_recv_watchdog();
	}

	char s[3];

	for (int i = 0; i < 100; i++) {
		receivedCommand = new Command(Command::CODE::Speed, itoa(i, s, 10));

		driveController->executeCommand_Speed(receivedCommand);

		delete receivedCommand;

		receivedCommand = new Command(Command::CODE::SteeringAngle, itoa(i, s, 10));

		driveController->executeCommand_SteeringAngle(receivedCommand);

		delete receivedCommand;

		delay(30);
	}


	if (communicator->timedOut()) {
		// TODO
	}
}

