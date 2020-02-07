#pragma once
#include <Shooter.h>
#include "SparkDrive.h"
#include <frc/Joystick.h>
#include "RobotUtilities.h"
class TeleopFunctions
{
    public:
        TeleopFunctions(frc::Joystick *joystick_1, Shooter *shooter, SparkDrive *sparkDrive);
        void TurnToAngle(double targetAngle, double proportion);//Autonomously turn to an angle for use with vision targeting. Must be continuously called until the turn is finished
        void ShooterFunction();
        bool GetTurnStatus();//Returns true if the turn if the turn is completed. Used to help call the TelopFunctions::TurnToAngle function.
    private:
        frc::Joystick* js1;
        Shooter* shooter;
        SparkDrive *m_sparkDrive;

    //ShooterFunction vars
        int shooterButton = 1;

     //TurnToAngle vars
        double current_rotation_rate, proportion;
        const double slop = 1.5, min_rotation_speed = .1;
        int TURN_BUTTON_TIMEOUT;
        const int timeToAdjust = 10;
        bool turn_complete = true;
};