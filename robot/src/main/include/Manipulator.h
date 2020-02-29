#pragma once

#include <Intake.h>
#include <Feeder.h>
#include <Shooter.h>
#include <frc/Smartdashboard/SmartDashboard.h>

class Manipulator
{
    public:
        Manipulator(Intake *intake, Feeder *feeder, Shooter *shooter);
        void PrepareShot(int flywheel_rpm, int aim_position);
        void ContinuousShoot(int aim_position, double geneva_speed, int shooting_rpm);
        void ContinuousIntake();
        void ResetManipulatorElements();
        void GetState();
        void SensorAdvanceGeneva(bool spin);
        void GenevaSetSpin(double power);
        int GetSensorAdvanceGenevaState();

        Intake* GetIntake();
        Feeder* GetFeeder();
        Shooter* GetShooter();
    private:
        //Member objects
        Intake *m_intake;
        Feeder *m_feeder; 
        Shooter *m_shooter;
        
        //Finite State Machine vars
        enum shooterStates{
            kRamping, kPunching, kRetracting, kAdvance, kAdvancing
        };
        int shooterState = kRamping;
        int state_change_counter = 0;
        const int kCountsToExtend = 5;

           enum genevaStates{
            move, moving, stopped
        };
        int genevaState = stopped;
};