#pragma once
#include <string>

#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SendableChooser.h>

#include <frc/Joystick.h>
#include "rev/SparkMax.h"

#include <frc/DigitalInput.h>

#include "rev/CANSparkMax.h"
class Shooter
{
    public:
        Shooter(int shooter_motor_id, int aim_motor_id);
        Shooter(rev::CANSparkMax *shooterMoter, rev::CANSparkMax *aimMotor);
        void Shoot(double shooterSpeed);
        void AimHeight(double positionTicks);
        void SetAimHeightP(double p); 

        void SetShootingPercentOutput(double percent_output);
        void SetAdjusterPercentOutput(double percent_output);
    private:
    rev::CANSparkMax *m_shooterMotor;
    rev::CANPIDController *m_shooterPid;
    double speed;

    rev::CANSparkMax *m_aimMotor; 
    rev::CANPIDController *m_aimPid;
    double aimPosition;

    frc::DigitalInput* upper_limit_switch;
    frc::DigitalInput* lower_limit_switch;
};