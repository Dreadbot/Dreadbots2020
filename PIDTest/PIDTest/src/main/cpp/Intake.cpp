#include "Intake.h"

Intake::Intake(int motorId){ 
    intake_motor = new rev::CANSparkMax(motorId, rev::CANSparkMax::MotorType::kBrushless);
    m_pidController = new rev::CANPIDController(intake_motor->GetPIDController());
    m_pidController->SetP(6e-5);
    m_pidController->SetI(1e-6);
    m_pidController->SetD(0.3);
    m_pidController->SetIZone(0);  
    m_pidController->SetFF(.5);
    m_pidController->SetOutputRange(-1.0, 1.0);
    m_pidController = new rev::CANPIDController(intake_motor->GetPIDController());
}

void Intake::Start(){
    intake_motor->Set(.5);
}

void Intake::Stop(){
    intake_motor->Set(0);
    running = false;
}
void Intake::SetSpeed(double speed) {
    if(!running){
        m_pidController->SetReference(speed, rev::ControlType::kVelocity); 
        running = true;
    }
} 
