#pragma once
#include <rev/CANSparkMax.h>
class Intake
{
    public:
        Intake(rev::CANSparkMax *intake_motor);
        void SetSpeed(double speed);
        void Start();
        void Stop();
    private:
        rev::CANSparkMax *m_intake_motor;
        rev::CANPIDController *m_pidController;
        bool running = false;
};