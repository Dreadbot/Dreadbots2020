#pragma once
#include "rev/SparkMax.h"
#include "rev/CANSparkMax.h"
#include "frc/Solenoid.h"

class Climber
{
    public:
        Climber(frc::Solenoid *telescope_sol, rev::CANSparkMax *winchMotor);
        void SetTelescope(bool extended);
        void SetWinch(double winchspeed);
    private:
        frc::Solenoid *m_telescope_sol;
        rev::CANSparkMax *m_winchMotor;
        rev::CANPIDController *m_winchPid;
};