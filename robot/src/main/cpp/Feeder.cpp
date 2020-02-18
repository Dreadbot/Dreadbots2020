#include <Feeder.h>

Feeder::Feeder(rev::CANSparkMax *geneva_drive, frc::Solenoid *punch){
    m_geneva_drive = geneva_drive;
    m_geneva_controller = new rev::CANPIDController(m_geneva_drive->GetPIDController());
    m_geneva_encoder = new rev::CANEncoder(m_geneva_drive->GetEncoder());
    m_punch = punch;
    m_geneva_encoder->SetPosition(0);
    
    geneva_limit_switch = new frc::DigitalInput(kLimitSwitchPort);

    m_geneva_controller->SetP(6e-5);
    m_geneva_controller->SetI(1e-6);
    m_geneva_controller->SetD(0);
    m_geneva_controller->SetFF(.000015);
    m_geneva_controller->SetIZone(0);
    m_geneva_controller->SetOutputRange(1.0, -1.0);
}
void Feeder::SetSpin(int rpm){
    m_geneva_controller->SetReference(rpm, rev::ControlType::kVelocity);
}
void Feeder::GetSpin(){
     m_geneva_encoder->GetVelocity();
}
void Feeder::AdvanceGeneva(int rots){
    //Make it so it stops. Could be a encoder problem.
    // m_geneva_controller->SetReference((m_geneva_encoder->GetPosition() + (rots * kGenevaGearRatio)) 
    //                                 ,rev::ControlType::kPosition);
    m_geneva_controller->SetReference(0, rev::ControlType::kVelocity);
    std::cout << "Setting: " << (m_geneva_encoder->GetPosition() + (rots * kGenevaGearRatio)) << std::endl;
}
void Feeder::SetPunchExtension(bool extended){
    m_punch->Set(extended);
}
bool Feeder::GetPunchExtension(){
    return m_punch->Get();
}
bool Feeder::GetLimitSwitchState(){
    return geneva_limit_switch->Get();
}