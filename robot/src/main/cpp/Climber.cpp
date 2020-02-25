#include <Climber.h>
Climber::Climber(rev::CANSparkMax *telescopeMotor, rev::CANSparkMax *winchMotor){
    m_telescopeMotor = telescopeMotor;
    m_winchMotor = winchMotor;
    m_telescopePid = new rev::CANPIDController(m_telescopeMotor -> GetPIDController());
    //m_winchPid = new rev::CANPIDController(m_winchMotor -> GetPIDController());

    m_telescopeEncoder = new rev::CANEncoder(m_telescopeMotor->GetEncoder());
}
void Climber::SetTelescope(double telescope_speed){
    m_telescopeMotor->Set(telescope_speed);
    if(telescope_speed != 0){
        std::cout << m_telescopeEncoder->GetPosition() << std::endl;
    }
}
void Climber::SetWinch(double winchspeed){
    m_winchMotor->Set(winchspeed);
}