#pragma once

#include <Intake.h>
#include <Feeder.h>
#include <Shooter.h>

class Manipulator
{
    public:
        Manipulator(Intake *intake, Feeder *feeder, Shooter *shooter);
        void PrepareShot(int flywheel_rpm, int aim_position);
        void ContinuousShoot(int aim_position);
        void ResetManipulatorElements();
    private:
        //Member objects
        Intake *m_intake; 
        Feeder *m_feeder; 
        Shooter *m_shooter; 
        
        //Finite State Machine vars
        int state;
        enum States{
            kPunching, kRetracting, kAdvance, kAdvancing
        };
        int state_change_counter = 0;
        const int kCountsToExtend = 100, kCountsToRetract = 100, kLoadingRPM = 1000;
};