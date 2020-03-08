#include <Manipulator.h>

Manipulator::Manipulator(Intake *intake, Feeder *feeder, Shooter *shooter){
    m_intake = intake;
    m_feeder = feeder;
    m_shooter = shooter;
    shooterState = kRamping;
}

void Manipulator::PrepareShot(int rpm, double aimPosition){
    m_shooter->Shoot(-rpm);
    m_shooter->SetAdjusterPosition(aimPosition);
}

int Manipulator::GetSelectedRPM(double inches){
    inches /= 12;
    return ((-0.0029 * inches * inches) + (0.188026 * inches) + 1.7676)*1000.;
}

double Manipulator::GetSelectedHoodPosition(double inches){
    inches /= 12;
    return ((-0.0941 * inches * inches) + (4.96271 * inches) + 2.08)/100.;
}

void Manipulator::ContinuousShoot(double aim_position, double geneva_speed, int shooting_rpm){
    //Finite State Machine logic to switch between states
    frc::SmartDashboard::PutNumber("GetShootingSpeed(): ", -m_shooter->GetShootingSpeed());
    frc::SmartDashboard::PutNumber("ShooterState: ", shooterState);
    //frc::SmartDashboard::PutBoolean("Geneva Limit Switch", m_feeder->GetGenevaSwitchState());
    //frc::SmartDashboard::PutBoolean("If Statement", (shooterState == kRamping && abs(abs(m_shooter->GetShootingSpeed()) - shooting_rpm) < 100));

    int speedDifference = abs(m_shooter->GetShootingSpeed()) - shooting_rpm;
    if(shooterState == kRamping && speedDifference < 300 && speedDifference > 0){
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
    else if(shooterState == kAdvance && !m_feeder->GetGenevaSwitchState()){
        shooterState = kAdvancing;
    }
    //Change state back to the start once the geneva drive has made it back to the limit switch
    else if(shooterState == kAdvancing && m_feeder->GetGenevaSwitchState()){
        shooterState = kRamping;
    }
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
    m_shooter->SetVisionLight(true);
}

void Manipulator::ContinuousIntake(){
    
}
void Manipulator::ResetManipulatorElements(){
    //std::cout << "Switch State" << std::boolalpha << m_feeder->GetPunchExtension() << std::endl;
    
    GetShooter()->SetVisionLight(true);

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
    if(m_shooter->GetAimReadiness()){
        m_shooter->SetAdjusterPosition(0);
    }
    m_shooter->SetVisionLight(true);
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
    double genevaSpeed = 0.4;
    if(genevaState == stopped && spin){
        if(forward){
            m_feeder->SetSpin(-genevaSpeed);
            genevaDirection = kForward;
        }
        else{
            m_feeder->SetSpin(genevaSpeed);
            genevaDirection = kBackward;
        }
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
        if(genevaDirection == kForward){
            m_feeder->SetSpin(-genevaSpeed);
        }
        else{
            m_feeder->SetSpin(genevaSpeed);
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