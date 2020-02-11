#include <Intake.h>

Intake::Intake(int motorId){ 
    intake_motor = new rev::CANSparkMax(motorId, rev::CANSparkMax::MotorType::kBrushless);
    m_pidController = new rev::CANPIDController(intake_motor->GetPIDController());
    m_pidController->SetP(6e-5);
    m_pidController->SetI(1e-6);
    m_pidController->SetD(0.3);
    m_pidController->SetIZone(0);  
    m_pidController->SetFF(0.0);
    m_pidController->SetOutputRange(0.0, 1.0);
    m_pidController = new rev::CANPIDController(intake_motor->GetPIDController());

}

void Intake::Start(){
    intake_motor->Set(-.9);
}

void Intake::Stop(){
    std::cout << "Stop(): OutputCurrent" << intake_motor->GetOutputCurrent() << std::endl;
    std::cout << "Stop(): Velocity: " <<  intake_motor->GetEncoder().GetVelocity() << std::endl;
    intake_motor->Set(0);
    running = false;
}

void Intake::SetSpeed(double speed) {
    std::cout << "SetSpeed(): OutputCurrent" << intake_motor->GetOutputCurrent() << std::endl;
    std::cout << "SetSpeed(): Velocity: " <<  intake_motor->GetEncoder().GetVelocity() << std::endl;
    if (!running) {
        m_pidController->SetReference(speed, rev::ControlType::kVelocity); 
        running = true;
    }
} 
