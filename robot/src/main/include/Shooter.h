#pragma once
#include <string>

#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SendableChooser.h>

#include <frc/Joystick.h>
#include "rev/SparkMax.h"
#include "rev/CANSparkMax.h"
class Shooter
{
    public:
        Shooter(int shootermotorid,int aimmotorid);
        void Shoot(double shooterSpeed);
        void AimHeight(double positionTicks);
        void SetAimHeightPid(double pid); 
    private:
    rev::CANSparkMax *shooterMotor;
    rev::CANPIDController *shooterPid;
    double speed;

    rev::CANSparkMax *aimMotor; 
    rev::CANPIDController *aimPid;
    double aimPosition;
};