#include <Climber.h>
Climber::Climber(frc::Solenoid *telescope_sol, rev::CANSparkMax *winchMotor){
    m_telescope_sol = telescope_sol;
    m_winchMotor = winchMotor;
}
void Climber::SetTelescope(bool extended){
    m_telescope_sol->Set(extended);
}
void Climber::SetWinch(double winchspeed){
    m_winchMotor->Set(winchspeed);
}