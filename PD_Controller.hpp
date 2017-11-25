#ifndef RC_CAR__PD_CONTROLLER__HPP
#define RC_CAR__PD_CONTROLLER__HPP

#include "PeriodicTask.hpp"

namespace rc_car {
    /** @brief PD controller implementation - used for controlling speed.
    */
    class PD_Controller : public Periodic {
    private:
        /** @brief Weight of the proportional term.
        */
        float Kp;

        /** @brief Weight of the derivative term.
        */
        float Kd;

        /** @brief The previous error - needed for the derivative term.
        */
        float prevError = 0.0f;

        /** @brief The output - updated in every cycle, holds the output value until the next update.
        */
        float output = 0.0f;

        /** @brief The minimum output value.
        */
        float outMin;

        /** @brief The maximum output value.
        */
        float outMax;

    public:
        /** @brief Constructor - sets period time and term weights.

        @param _periodTime The period time.
        @param _Kp The weight of the proportional term.
        @param _Kd The weight of the derivative term.
        @param _outMin The minimum output value.
        @param _outMax The maximum output value.
        */
        PD_Controller(int _periodTime, float _Kp, float _Kd, float _outMin, float _outMax)
            : Periodic(_periodTime), Kp(_Kp), Kd(_Kd), outMin(_outMin), outMax(_outMax) {}

        /** @brief Updates output according to current and previous errors.

        @param currentError The current error between the desired and the measured outputs.
        */
        void run(float currentError);

        /** @brief Gets output value.

        @returns The output value.
        */
        float getOutput() const {
            return output;
        }
    };
}

#endif // RC_CAR__PD_CONTROLLER__HPP