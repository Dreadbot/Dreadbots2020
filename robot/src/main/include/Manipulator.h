#pragma once

#include <Intake.h>
#include <Feeder.h>
#include <Shooter.h>
#include <frc/Smartdashboard/SmartDashboard.h>

class Manipulator
{
    public:
        Manipulator(Intake *intake, Feeder *feeder, Shooter *shooter);
        int Round();
        int GetSelectedRPM(int index);
        double GetSelectedHoodPosition(int index);
        void ContinuousShoot(int aim_position, double geneva_speed, int shooting_rpm);
        void ContinuousIntake();
        void ResetManipulatorElements();
        void GetState();
        void SensorAdvanceGeneva(bool spin, bool forward);
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
        int shooterState;
        int state_change_counter = 0;
        const int kCountsToExtend = 5;

        enum genevaStates{
            move, moving, stopped
        };
        int genevaState = stopped;
        

        int ShootingSpeeds [5] = {2500, 3750, 3750, 4000, 4250};
        double HoodPositions [5] = {0.1, .46, .505, .53, .55};
        int distance;
};