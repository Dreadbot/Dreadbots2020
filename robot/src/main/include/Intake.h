#pragma once
#include <rev/CANSparkMax.h>
#include <frc/Solenoid.h>
class Intake
{
    public:
        Intake(rev::CANSparkMax *intake_motor, frc::Solenoid *intake_pin);
        void SetSpeed(double speed);
        void SetPercentOutput(double percent_output);
        void DeployIntake();
        void Start();
        void Stop();
    private:
        rev::CANSparkMax *m_intake_motor;
        rev::CANPIDController m_pidController;
        frc::Solenoid *m_intake_pin;
        bool running = false;
};