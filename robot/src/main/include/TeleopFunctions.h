#pragma once
#include <Shooter.h>
#include "SparkDrive.h"
#include <frc/Joystick.h>
#include "RobotUtilities.h"
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/smartdashboard/SmartDashboard.h>

#include <frc/controller/PIDController.h>

class TeleopFunctions
{
    public:
        TeleopFunctions(frc::Joystick *joystick_1, Shooter *shooter, SparkDrive *sparkDrive);
        void TurnToAngle(double targetAngle, double proportion);//Autonomously turn to an angle for use with vision targeting. Must be continuously called until the turn is finished
        void WPITurnToAngle(double target_angle);
        double CalculateTurnToAngle(double target_angle);
        void UpdatePIDController();
        void ShooterFunction();
        bool GetTurnStatus();//Returns true if the turn if the turn is completed. Used to help call the TelopFunctions::TurnToAngle function.
        void SetTurnStatus(bool turnStatus);
    private:
        frc::Joystick* js1;
        Shooter* shooter;
        SparkDrive *m_sparkDrive;

    //ShooterFunction vars
        int shooterButton = 1;

        //PID
        frc2::PIDController* pid_controller;
        double p;
        double i;
        double d;

     //TurnToAngle vars
        double current_rotation_rate, proportion, min_rotation_speed = 0;
        const double slop = 1.5; 
        int TURN_BUTTON_TIMEOUT;
        const int timeToAdjust = 10;
        bool turn_complete = true;
};