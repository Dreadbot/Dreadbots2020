#include <Manipulator.h>

Manipulator::Manipulator(Intake *intake, Feeder *feeder, Shooter *shooter){
    m_intake = intake;
    m_feeder = feeder;
    m_shooter = shooter;
    shooterState = kRamping;
}
int Manipulator::Round(){
    distance = frc::SmartDashboard::GetNumber("selectedDistance", 100);
    if(distance <= 60 || (distance > 60 && distance <= 90)){
        return 0;
    }
    else if(distance > 90 && distance <= 150){
        return 1;
    }
    else if(distance > 150 && distance <= 210){
        return 2;
    }
    else if(distance > 210 && distance <= 270){
        return 3;
    }
    else if(distance > 270){ 
      return 4;
    }
    return -1;
}

void Manipulator::PrepareShot(int rpm, double aimPosition){
    m_shooter->Shoot(rpm);
    std::cout << "PREPARE SHOT: " << aimPosition << std::endl;
    m_shooter->SetAdjusterPosition(aimPosition);
}

int Manipulator::GetSelectedRPM(unsigned int index){
    if(index < sizeof(ShootingSpeeds)/sizeof(ShootingSpeeds[0])){
      return ShootingSpeeds[index];
    }
    return -1;
}
double Manipulator::GetSelectedHoodPosition(unsigned int index){
    if(index < sizeof(HoodPositions)/sizeof(HoodPositions[0])){
      return HoodPositions[index];
    }
    return -1;
}

void Manipulator::ContinuousShoot(double aim_position, double geneva_speed, int shooting_rpm){
    //Finite State Machine logic to switch between states
    frc::SmartDashboard::PutNumber("GetShootingSpeed(): ", -m_shooter->GetShootingSpeed());
    frc::SmartDashboard::PutNumber("ShooterState: ", shooterState);
    //frc::SmartDashboard::PutBoolean("Geneva Limit Switch", m_feeder->GetGenevaSwitchState());
    //frc::SmartDashboard::PutBoolean("If Statement", (shooterState == kRamping && abs(abs(m_shooter->GetShootingSpeed()) - shooting_rpm) < 100));

    int speedDifference = abs(m_shooter->GetShootingSpeed()) - shooting_rpm;
    if(shooterState == kRamping && speedDifference < 500 && speedDifference > 0){
        shooterState = kPunching;
    }
    else if(shooterState == kRamping && speedDifference > -100 && speedDifference < 0){
        shooterState = kPunching;
    }
    //Change state based on a counter so that the solenoid has time to extend
    else if(shooterState == kPunching && state_change_counter > kCountsToExtend){
        shooterState = kRetracting;
        state_change_counter = 0;
    }

    //Change state based on a counter so that the solenoid has time to retract
    else if(shooterState == kRetracting && m_feeder->GetPunchSwitchState()){
        shooterState = kAdvance;
        state_change_counter = 0;
    }

    //Change state if the geneva drive has rotated away from the limit switch
    else if(shooterState == kAdvance && !m_feeder->GetGenevaSwitchState())
        shooterState = kAdvancing;

    //Change state back to the start once the geneva drive has made it back to the limit switch
    else if(shooterState == kAdvancing && m_feeder->GetGenevaSwitchState())
        shooterState = kRamping;

    //Choose behavior based on the FSM
    switch(shooterState){
        case(kRamping):
             m_feeder->SetSpin(0);
             break;
        case(kPunching):
            m_feeder->SetPunchExtension(true);//Extend the punch
            state_change_counter++;
            break;
        case(kRetracting):
            m_feeder->SetPunchExtension(false);//Retract the punch
            break;
        case(kAdvance):
        case(kAdvancing):
            m_feeder->SetSpin(geneva_speed);//Turn the motor on
            break;
    }
    
    //Set the position of the aim plate and always drive the flywheel
    //std::cout << "********Calling Adjuster position to: " << aim_position << std::endl;
    m_shooter->SetAdjusterPosition(aim_position);
    m_shooter->Shoot(-shooting_rpm);
}

void Manipulator::ContinuousIntake(){
    
}
void Manipulator::ResetManipulatorElements(){
    //std::cout << "Switch State" << std::boolalpha << m_feeder->GetPunchExtension() << std::endl;
    
    //This function should be called continuously if the system is not shooting power cells or collecting power cells
    //The function will get the system back into a state where the punch is retracted and the geneva drive is lined up

    //If the punch is extended, retract it
    if(m_feeder->GetPunchExtension()){
        m_feeder->SetPunchExtension(false);
    }

    //Once the punch is retracted, if the geneva is not at a limit switch, turn it slowly
    else if(!m_feeder->GetPunchExtension() && !m_feeder->GetGenevaSwitchState()){
        //m_feeder->SetSpin(0.2);
    }

    //Once the geneva drive reaches a limit switch, stop it
    else{
        m_feeder->SetSpin(0);
        //std::cout << "stopping the spin" << std::endl;
    }
    shooterState = kRamping;
    m_shooter->SetShootingPercentOutput(0);
}
// void Manipulator::GetState(){
//     switch(state){
//         case kPunching:
//             std::cout << "kPunching" << std::endl;
//             break;
//         case kRetracting:
//             std::cout << "kRetracting" << std::endl;
//             break;
//         case kAdvance:
//         case kAdvancing:
//             std::cout << "kAdvancing" << std::endl;
//             break;
//     }
// }
void Manipulator::SensorAdvanceGeneva(bool spin, bool forward){
    //std::cout<<"X button is: " << spin <<std::endl;
    //std::cout << "SensorAdvance state: " << genevaState << std::endl;
    if(genevaState == stopped && spin){
        if(forward)
            m_feeder->SetSpin(.4);
        else
            m_feeder->SetSpin(-.4);
        genevaState = move;
        //std::cout<<"Indexing"<<genevaState<<std::endl;
    }
    else if(genevaState == move && !m_feeder->GetGenevaSwitchState()){
        genevaState = moving;
        //std::cout<<"State: "<<genevaState<<std::endl;
    }
    else if(genevaState == moving && m_feeder->GetGenevaSwitchState()){
        m_feeder->SetSpin(0);
        genevaState = stopped;
        //std::cout<<"State: "<<genevaState<<std::endl;
    }

    if(genevaState == move || genevaState == moving){
        //std::cout << "Moving" << std::endl;
        if(forward){
            m_feeder->SetSpin(0.4);
        }
        else{
            m_feeder->SetSpin(-0.4);
        }
    }

}

void Manipulator::GenevaSetSpin(double power){
    m_feeder->SetSpin(power);
}
int Manipulator::GetSensorAdvanceGenevaState(){
    return m_feeder->GetSensorAdvanceGenevaState();
}

Intake* Manipulator::GetIntake()
{
    return m_intake;
}

Feeder* Manipulator::GetFeeder()
{
    return m_feeder;
}

Shooter* Manipulator::GetShooter()
{
    return m_shooter;
}