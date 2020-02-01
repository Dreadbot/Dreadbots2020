#pragma once
#include <rev/CANSparkMax.h>
class Intake
{
    public:
        Intake();
        void Start();
        void Stop();
        void SetSpeed(double speed);
    private:
        rev::CANSparkMax *intake_motor;
        rev::CANPIDController *intakePid;
};