#pragma once
#include "rev/CANSparkMax.h"
#include "frc/Solenoid.h"
#include "frc/DigitalInput.h"
#include <frc/Joystick.h>

class Feeder
{
    public:
        Feeder(rev::CANSparkMax *geneva_drive, frc::Solenoid *punch, frc::Joystick *joystick);
        void SetSpin(int rpm);
        void GetSpin();
        void AdvanceGeneva(int rots);
        void SensorAdvanceGeneva();
        void SetPunchExtension(bool extended);
        bool GetPunchExtension();
        bool GetLimitSwitchState();
        double GetGenevaPosition();
        

    private:
        rev::CANSparkMax *m_geneva_drive;
        rev::CANPIDController *m_geneva_controller;
        rev::CANEncoder *m_geneva_encoder;
        frc::Solenoid *m_punch;
        frc::DigitalInput *geneva_limit_switch;
        frc::Joystick *joystick_1;
        const int kLimitSwitchPort = 9;
        const double kGenevaGearRatio = 100;
        enum States{
            move, moving, stopped
        };
        int state = stopped;
};