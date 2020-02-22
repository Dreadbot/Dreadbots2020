#pragma once
#include "rev/SparkMax.h"
#include "rev/CANSparkMax.h"

class Climber
{
    public:
        Climber(rev::CANSparkMax *telescopeMoter, rev::CANSparkMax *winchMotor);
        void SetTelescope(bool raisetelescope);
        void SetWinch(double winchspeed);
    private:
    rev::CANSparkMax *m_telescopeMotor;
    rev::CANPIDController *m_telescopePid;
    
    rev::CANSparkMax *m_winchMotor;
    rev::CANPIDController *m_winchPid;
};