#include "MotorHandler.hpp"

using namespace rc_car;

void MotorHandler::ServoMotor::writeAngle(float _angle) {
    int _value = static_cast<int>(_angle * SERVO_STEERING_TR + SERVO_POS_MID);
    write(Common::incarcerate(_value, SERVO_POS_MIN, SERVO_POS_MAX));
}

void MotorHandler::DCMotor::initialize() {
    pinMode(DC_MOTOR_FORWARD_PIN, OUTPUT);
    pinMode(DC_MOTOR_BACKWARD_PIN, OUTPUT);
}

void MotorHandler::DCMotor::writeValue(int _value) {

    _value = Common::incarcerate(_value, DC_MIN_value, DC_MAX_value);
    bool isFwd = _value >= 0;

    analogWrite(DC_MOTOR_FORWARD_PIN, isFwd ? _value : 0);
    analogWrite(DC_MOTOR_BACKWARD_PIN, isFwd ? 0 : -_value);
}

void MotorHandler::initialize() {
    DC_Motor.initialize();
    attachServo();
    positionServoMiddle();
    setDesiredSpeed(0.0f);
}

void MotorHandler::updateSpeed(float actualSpeed) {
    // if controller's period time has been reached, speed has to be updated
    // -> controller's 'run' method is called with the error (difference between desired and actual speed)
    if (speedCtrl.periodTimeReached()) {
        if (desiredSpeed == 0.0f && Common::isBetween(actualSpeed, -DC_ZERO_SPEED_BOUNDARY, DC_ZERO_SPEED_BOUNDARY))
            DC_Motor.writeValue(0);
        else {
            float error = desiredSpeed - actualSpeed;
            speedCtrl.run(error);
            DC_Motor.writeValue(static_cast<float>(speedCtrl.getOutput()));
        }

        speedCtrl.restartPeriodCheck();
    }
}