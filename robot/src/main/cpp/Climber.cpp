#include <Climber.h>
Climber::Climber(rev::CANSparkMax *telescopeMotor, rev::CANSparkMax *winchMotor){
    m_telescopeMotor = telescopeMotor;
    m_winchMotor = winchMotor;
    m_telescopePid = new rev::CANPIDController(m_telescopeMotor -> GetPIDController());
    m_winchPid = new rev::CANPIDController(m_winchMotor -> GetPIDController());
}
void Climber::SetTelescope(bool rasietelescope){
    if (rasietelescope= true){
        m_telescopePid -> SetReference(1.0, rev::ControlType::kPosition);
    } 
    else
    {
        m_telescopePid -> SetReference(0.0, rev::ControlType::kPosition);
    }
    
};
void Climber::SetWinch(double winchspeed){
    m_winchMotor -> Set(winchspeed);
};
