#pragma once

#include <frc/Joystick.h>
#include <rev/CANSparkMax.h>
#include <frc/Solenoid.h>
#include <ctre/Phoenix.h>

#include "RobotUtilities.h"
#include "SparkDrive.h"
#include "Intake.h"
#include "Climber.h"

class Diagnostic
{
    public:
        Diagnostic(frc::Joystick *joystick_1,
        frc::Joystick *joystick_2,
        SparkDrive* sparkdrive,
    Intake* kIntakeMotorID, 
    rev::CANSparkMax* kGenevaMotorID,
    rev::CANSparkMax* kFlyWheelMotorID, 
    rev::CANSparkMax* kAimMotorID, 
    ctre::phoenix::motorcontrol::can::WPI_TalonSRX* kColorWheelMotorID, 
    Climber* kClimbMotorID,
    frc::Solenoid* solup
    //frc::Solenoid* solcolor
    );
        void run();
    double AxisY1 = js1->GetRawAxis(1);
    double AxisX1 = js1->GetRawAxis(0);
    private:
    frc::Joystick * js1;
    frc::Joystick * js2;
    SparkDrive* sparkdrive;
    rev::CANSparkMax * LF;
    rev::CANSparkMax * RF;
    rev::CANSparkMax * LR;
    rev::CANSparkMax * RR;
    Intake * IN;
    rev::CANSparkMax * Gen;
    rev::CANSparkMax * shooty;
    rev::CANSparkMax * aim;
    ctre::phoenix::motorcontrol::can::WPI_TalonSRX * color;
    Climber * climb;
    frc::Solenoid* solup;
    frc::Solenoid* solcolor;
};