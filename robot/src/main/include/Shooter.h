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
        Shooter(int shooter_motor_id, int aim_motor_id);
        Shooter(rev::CANSparkMax *shooterMoter, rev::CANSparkMax *aimMotor);
        void Shoot(double shooterSpeed);
        void AimHeight(double positionTicks);
        void SetAimHeightP(double p); 
    private:
    rev::CANSparkMax *m_shooterMotor;
    rev::CANPIDController *m_shooterPid;
    double speed;

    rev::CANSparkMax *m_aimMotor; 
    rev::CANPIDController *m_aimPid;
    double aimPosition;
};