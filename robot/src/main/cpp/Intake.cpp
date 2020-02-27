#include <Intake.h>
Intake::Intake(rev::CANSparkMax *intake_motor, frc::Solenoid *intake_pin)
    : m_intake_motor( intake_motor ),
      m_intake_pin( intake_pin ),
      m_pidController( m_intake_motor->GetPIDController() )
{ 
    m_pidController.SetP(6e-5);
    m_pidController.SetI(1e-6);
    m_pidController.SetD(0.3);
    m_pidController.SetIZone(0);  
    m_pidController.SetFF(0.000015);
    m_pidController.SetOutputRange(-1.0, 1.0);
}
void Intake::SetSpeed(double speed) {
    m_pidController.SetReference(speed, rev::ControlType::kVelocity);
} 

void Intake::SetPercentOutput(double percent_output)
{
  m_intake_motor->Set(percent_output);
}

void Intake::DeployIntake(){
    m_intake_pin->Set(true); //This is true because true flips the solenoid from what is default, and the pin should be physically connected to be extended by default
}

void Intake::Start(){
    m_intake_motor->Set(.5);
}

void Intake::Stop(){
    m_intake_motor->Set(0);
    running = false;
}

