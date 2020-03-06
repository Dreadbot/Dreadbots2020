#pragma once

#include <frc/DigitalInput.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Solenoid.h>
#include <RobotUtilities.h>
#include "rev/CANSparkMax.h"

class Shooter
{
 public:
  /**
   * Shooter class Constructor using Motor IDs
   * 
   * @param shooting_motor_id The ID of the Shooting Motor
   * @param aiming_motor_id The ID of the Adjusting Motor
   */
  Shooter(int shooting_motor_id, int aiming_motor_id);

  /**
   * Shooter class Constructor using CANSparkMax Object Pointers.
   * 
   * @param shooting_motor The Shooting Motor Controller SparkMax Object Pointer
   * @param aiming_motor The Aiming Motor Controller SparkMax Object Pointer
   */
  Shooter(rev::CANSparkMax* shooting_motor, rev::CANSparkMax* aim_motor);

  void Shoot(int rpm);
  void AimHeight(double position_ticks);
  void SetAimHeightP(double p);

  /**
   * Return the actual speed in rpms of the flywheel
   * 
   * 
   */
  int GetShootingSpeed(); 

  /**
   * Utility Method to Set the Percent Output of the Shooting Motor
   * 
   * @param percent_output The Percent Output to Set the Shooting NEO to.
   */
  void SetShootingPercentOutput(double percent_output);

  /**
   * Utility Method to Set the Percent Output of the Aiming Motor
   * 
   * @param percent_output The Percent Output to Set the Aiming "Baby" NEO to.
   */
  void SetAdjusterPercentOutput(double percent_output);
  void SetAdjusterPosition(double position);
  void SetLowerLimit(int position);
  void SetUpperLimit(int position);
  void SetAimReadiness(bool ready);
  bool GetAimReadiness();
  int GetHoodPosition();
  bool GetUpperLimitSwitch();
  bool GetLowerLimitSwitch();
  bool GetUpperLimitBool();
  bool GetLowerLimitBool();
  void SetUpperBool(bool value);
  void SetLowerBool(bool value);
  void SetPID(double P, double I, double D);
  void SetLight(bool value);

 private:
  rev::CANSparkMax* shooting_motor;
  rev::CANPIDController* shooting_motor_pid_controller;
  rev::CANEncoder *shooter_encoder;
  double speed;

  rev::CANSparkMax* aiming_motor; 
  rev::CANPIDController *aiming_motor_pid_controller;
  rev::CANEncoder *aiming_motor_encoder;
  double aimPosition;

  frc::DigitalInput* upper_limit_switch;
  frc::DigitalInput* lower_limit_switch;
  frc::Solenoid *light;
  int minHoodPosition;
  int maxHoodPosition;
  bool readyToAim = false;
  bool lowerLimitHit = false;
  bool upperLimitHit = false;
  double range;
};