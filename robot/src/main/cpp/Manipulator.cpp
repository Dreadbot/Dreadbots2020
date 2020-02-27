#include <Manipulator.h>
#include "frc/DigitalInput.h"

Manipulator::Manipulator(Intake *intake, Feeder *feeder, Shooter *shooter){
    m_intake = intake;
    m_feeder = feeder;
    m_shooter = shooter;
    state = kPunching;
}
void Manipulator::PrepareShot(int rpm, int aim_position){
    m_shooter->Shoot(rpm);
    m_shooter->AimHeight(aim_position);
}
void Manipulator::ContinuousShoot(int aim_position){
    //Finite State Machine logic to switch between states

    //Change state based on a counter so that the solenoid has time to extend
    if(state == kPunching && state_change_counter > kCountsToExtend){
        state = kRetracting;
        state_change_counter = 0;
    }

    //Change state based on a counter so that the solenoid has time to retract
    else if(state == kRetracting && m_feeder->GetPunchSwitchState()){
        state = kAdvance;
        state_change_counter = 0;
    }

    //Change state if the geneva drive has rotated away from the limit switch
    else if(state == kAdvance && !m_feeder->GetGenevaSwitchState())
        state = kAdvancing;

    //Change state back to the start once the geneva drive has made it back to the limit switch
    else if(state == kAdvancing && m_feeder->GetGenevaSwitchState())
        state = kPunching;

    //Choose behavior based on the FSM
    switch(state){
        case(kPunching):
            m_feeder->SetPunchExtension(true);//Extend the punch
            state_change_counter++;
            break;
        case(kRetracting):
            m_feeder->SetPunchExtension(false);//Retract the punch
            break;
        case(kAdvance):
        case(kAdvancing):
            m_feeder->SetSpin(0.5);//Turn the motor on
            break;
    }
    
    //Set the position of the aim plate and always drive the flywheel
    m_shooter->AimHeight(aim_position);
    m_shooter->Shoot(-0.5);
    std::cout << "state: "<< state << std::endl;
}
void Manipulator::ContinuousIntake(){
    
}
void Manipulator::ResetManipulatorElements(){
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
        state = kPunching;
        std::cout << "stopping the spin" << std::endl;
    }
    m_shooter->SetShootingPercentOutput(0);
}
void Manipulator::GetState(){
    switch(state){
        case kPunching:
            std::cout << "kPunching" << std::endl;
            break;
        case kRetracting:
            std::cout << "kRetracting" << std::endl;
            break;
        case kAdvance:
        case kAdvancing:
            std::cout << "kAdvancing" << std::endl;
            break;
    }
}