#include <Feeder.h>

Feeder::Feeder(rev::CANSparkMax *geneva_drive, frc::Solenoid *punch, frc::Joystick *joystick_1, frc::Joystick *joystick_2){
    m_geneva_drive = geneva_drive;
    this->joystick_1 = joystick_1;
    this->joystick_2 = joystick_2;
    m_geneva_drive->SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
    m_geneva_controller = new rev::CANPIDController(m_geneva_drive->GetPIDController());
    m_geneva_encoder = new rev::CANEncoder(m_geneva_drive->GetEncoder());
    m_punch = punch;
    m_geneva_encoder->SetPosition(0);
    
    geneva_limit_switch = new frc::DigitalInput(kGenevaSwitchPort);
    punch_limit_switch = new frc::DigitalInput(kPunchSwitchPort);

    m_geneva_controller->SetP(0.002);
    m_geneva_controller->SetI(1e-6);
    m_geneva_controller->SetD(0.02);
    m_geneva_controller->SetFF(.000015);
    m_geneva_controller->SetIZone(0);
    m_geneva_controller->SetOutputRange(-1.0, 1.0);
    m_geneva_encoder->SetPositionConversionFactor(1.0);
}
void Feeder::SetSpin(double power){
    m_geneva_drive->Set(power);
}

//NOW MOVED TO MANIPULATOR.CPP:

// void Feeder::SensorAdvanceGeneva(bool spin){ 
//     std::cout<<"Start State: "<<state<<std::endl;
//     if(state == stopped && spin){
//         m_geneva_drive->Set(.4);
//         state = move;
//         std::cout<<"Indexing"<<state<<std::endl;
//     }
//     else if(state == move && !GetGenevaSwitchState()){
//         state = moving;
//         std::cout<<"State: "<<state<<std::endl;
//     }
//     else if(state == moving && GetGenevaSwitchState()){
//         m_geneva_drive->Set(0);
//         state = stopped;
//         std::cout<<"State: "<<state<<std::endl;
//     }
// }   

void Feeder::SetPunchExtension(bool extended){
    m_punch->Set(extended);
    frc::SmartDashboard::PutBoolean("Punch", extended);
}
bool Feeder::GetPunchExtension(){
    return m_punch->Get();
}

//These two functions are reversed because limit switches Get() method return opposite of intuition
bool Feeder::GetGenevaSwitchState(){
    return !geneva_limit_switch->Get();
}
bool Feeder::GetPunchSwitchState(){
    return !punch_limit_switch->Get();
}
//

double Feeder::GetGenevaPosition(){
    return m_geneva_encoder->GetPosition();
}
void Feeder::ExtendRetract(int milliseconds_between){
    std::chrono::milliseconds timespan(milliseconds_between);
    SetPunchExtension(true);
    frc::SmartDashboard::PutBoolean("Punch", true);
    std::this_thread::sleep_for(timespan);
    SetPunchExtension(false);
    frc::SmartDashboard::PutBoolean("Punch", false);
}
int Feeder::GetSensorAdvanceGenevaState(){
    return state;
}