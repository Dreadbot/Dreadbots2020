#pragma once

#include <frc/DigitalInput.h>

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

  void Shoot(double shooter_speed);
  void AimHeight(double position_ticks);
  void SetAimHeightP(double p); 

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

 private:
  rev::CANSparkMax* shooting_motor;
  rev::CANPIDController* shooting_motor_pid_controller;
  double speed;

  rev::CANSparkMax* aiming_motor; 
  rev::CANPIDController *aiming_motor_pid_controller;
  double aimPosition;

  frc::DigitalInput* upper_limit_switch;
  frc::DigitalInput* lower_limit_switch;
};