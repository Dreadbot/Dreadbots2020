#include <Manipulator.h>
#include "frc/DigitalInput.h"

Manipulator::Manipulator(Intake *intake, Feeder *feeder, Shooter *shooter){
    m_intake = intake;
    m_feeder = feeder;
    m_shooter = shooter;
    shooterState = kPunching;
}
void Manipulator::PrepareShot(int rpm, int aim_position){
    m_shooter->Shoot(rpm);
    m_shooter->AimHeight(aim_position);
}
void Manipulator::ContinuousShoot(int aim_position, int geneva_speed){
    //Finite State Machine logic to switch between states

    //Change state based on a counter so that the solenoid has time to extend
    if(shooterState == kPunching && state_change_counter > kCountsToExtend){
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
        shooterState = kPunching;

    //Choose behavior based on the FSM
    switch(shooterState){
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
    m_shooter->AimHeight(aim_position);
    m_shooter->Shoot(-0.5);
    std::cout << "state: "<< shooterState << std::endl;
}
void Manipulator::ContinuousIntake(){
    
}
void Manipulator::ResetManipulatorElements(){
    std::cout << "Switch State" << std::boolalpha << m_feeder->GetPunchExtension() << std::endl;
    
    //This function should be called continuously if the system is not shooting power cells or collecting power cells
    //The function will get the system back into a state where the punch is retracted and the geneva drive is lined up

    //If the punch is extended, retract it
    if(m_feeder->GetPunchExtension()){
        m_feeder->SetPunchExtension(false);
        std::cout << "retracted" << std::endl;
    }

    //Once the punch is retracted, if the geneva is not at a limit switch, turn it slowly
    else if(!m_feeder->GetPunchExtension() && !m_feeder->GetGenevaSwitchState()){
        m_feeder->SetSpin(0.2);
        std::cout << "spinning" << std::endl;
    }

    //Once the geneva drive reaches a limit switch, stop it
    else{
        m_feeder->SetSpin(0);
        shooterState = kPunching;
        std::cout << "stopping the spin" << std::endl;
    }
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
void Manipulator::SensorAdvanceGeneva(bool spin){
    std::cout<<"X button is: " << spin <<std::endl;
    if(genevaState == stopped && spin){
        m_feeder->SetSpin(.4);
        genevaState = move;
        std::cout<<"Indexing"<<genevaState<<std::endl;
    }
    else if(genevaState == move && !m_feeder->GetGenevaSwitchState()){
        genevaState = moving;
        std::cout<<"State: "<<genevaState<<std::endl;
    }
    else if(genevaState == moving && m_feeder->GetGenevaSwitchState()){
        m_feeder->SetSpin(0);
        genevaState = stopped;
        std::cout<<"State: "<<genevaState<<std::endl;
    }
}

void Manipulator::GenevaSetSpin(double power){
    m_feeder->SetSpin(power);
}
int Manipulator::GetSensorAdvanceGenevaState(){
    return m_feeder->GetSensorAdvanceGenevaState();
}