#include <Intake.h>
Intake::Intake(rev::CANSparkMax *intake_motor){ 
    m_intake_motor = intake_motor;
    m_pidController = new rev::CANPIDController(m_intake_motor->GetPIDController());

    m_pidController->SetP(6e-5);
    m_pidController->SetI(1e-6);
    m_pidController->SetD(0.3);
    m_pidController->SetIZone(0);  
    m_pidController->SetFF(0.000015);
    m_pidController->SetOutputRange(-1.0, 1.0);
}
void Intake::SetSpeed(double speed) {
    m_pidController->SetReference(speed, rev::ControlType::kVelocity); 
} 

void Intake::Start(){
    m_intake_motor->Set(.5);
}

void Intake::Stop(){
    m_intake_motor->Set(0);
    running = false;
}

