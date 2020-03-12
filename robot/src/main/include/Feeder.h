#pragma once
#include  <frc/smartdashboard/SmartDashboard.h>
#include "rev/CANSparkMax.h"
#include "frc/Solenoid.h"
#include "frc/DigitalInput.h"
#include "RobotUtilities.h"
#include <frc/Joystick.h>
#include <thread>
#include <chrono>
#include <string.h>

class Feeder
{    
  public:
    Feeder(rev::CANSparkMax *geneva_drive, frc::Solenoid *punch);
    void SetSpin(double rpm);
    void SetPunchExtension(bool extended);
    bool GetPunchExtension();
    bool GetGenevaSwitchState();
    bool GetPunchSwitchState();
    double GetGenevaPosition();
    void ExtendRetract(int milliseconds_between);
    int GetSensorAdvanceGenevaState();
        
  private:
    rev::CANSparkMax *m_geneva_drive;
    rev::CANPIDController *m_geneva_controller;
    rev::CANEncoder *m_geneva_encoder;
    frc::Solenoid *m_punch;
    frc::DigitalInput *geneva_limit_switch;
    frc::DigitalInput *punch_limit_switch;
    const int kGenevaSwitchPort = 9;
    const int kPunchSwitchPort = 3;
    const double kGenevaGearRatio = 100;
};