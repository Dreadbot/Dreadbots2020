#pragma once
#include "rev/CANSparkMax.h"
#include "frc/Solenoid.h"
#include "frc/DigitalInput.h"


class Feeder
{
    public:
        Feeder(rev::CANSparkMax *geneva_drive, frc::Solenoid *punch);
        void SetSpin(int rpm);
        void GetSpin();
        void AdvanceGeneva(int rots);
        void SetPunchExtension(bool extended);
        bool GetPunchExtension();
        bool GetLimitSwitchState();
        

    private:
        rev::CANSparkMax *m_geneva_drive;
        rev::CANPIDController *m_geneva_controller;
        rev::CANEncoder *m_geneva_encoder;
        frc::Solenoid *m_punch;
        frc::DigitalInput *geneva_limit_switch;
        const int kLimitSwitchPort = 0;
        const double kGenevaGearRatio = (72/14);
    
};