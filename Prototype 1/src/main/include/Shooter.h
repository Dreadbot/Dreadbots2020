#pragma once
#include <string>

#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SendableChooser.h>

#include <frc/Joystick.h>
#include "rev/SparkMax.h"
#include "rev/CANSparkMax.h"
#include "frc/WPILib.h"
class Shooter
{
    public:
        Shooter();
        void Shoot(double shooterSpeed);
    private:
    rev::CANSparkMax *shooterMotor;
    rev::CANPIDController *shooterPid;
    double speed;
};