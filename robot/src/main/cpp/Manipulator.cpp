#include <Manipulator.h>
#include "frc/DigitalInput.h"

Manipulator::Manipulator(Intake *intake, Feeder *feeder, Shooter *shooter){
    m_intake = intake;
    m_feeder = feeder;
    m_shooter = shooter;
    state = 0;
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
    else if(state == kRetracting && state_change_counter > kCountsToRetract){
        state = kAdvance;
        state_change_counter = 0;
    }

    //Change state if the geneva drive has rotated away from the limit switch
    else if(state == kAdvance && !m_feeder->GetLimitSwitchState())
        state = kAdvancing;

    //Change state back to the start once the geneva drive has made it back to the limit switch
    else if(state == kAdvancing && m_feeder->GetLimitSwitchState())
        state = kPunching;

    //Choose behavior based on the FSM
    switch(state){
        case(kPunching):
            m_feeder->SetPunchExtension(true);//Extend the punch
            state_change_counter++;
            break;
        case(kRetracting):
            m_feeder->SetPunchExtension(false);//Retract the punch
            state_change_counter++;
            break;
        case(kAdvance):
            m_feeder->AdvanceGeneva(1);//Use a motor encoder to advance the geneva drive once
            break;
    }
    
    //Set the position of the aim plate and always drive the flywheel
    m_shooter->AimHeight(aim_position);
    m_shooter->Shoot(1);
    std::cout << "state: "<< state << std::endl;
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
    else if(!m_feeder->GetPunchExtension() && !m_feeder->GetLimitSwitchState()){
        m_feeder->SetSpin(60);
        std::cout << "spinning" << std::endl;
    }

    //Once the geneva drive reaches a limit switch, stop it
    else{
        m_feeder->SetSpin(0);
        state = kPunching;
        std::cout << "stopping the spin" << std::endl;
    }
}