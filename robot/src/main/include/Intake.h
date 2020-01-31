#pragma once
#include <rev/CANSparkMax.h>
class Intake
{
    public:
        Intake();
        void Start();
        void Stop();
    private:
        rev::CANSparkMax *intake_motor;
};