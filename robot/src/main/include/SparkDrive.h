/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <rev/CANSparkMax.h>

class SparkDrive
{
 public:
  SparkDrive(rev::CANSparkMax *l_front_, rev::CANSparkMax *r_front_, rev::CANSparkMax *l_back_, rev::CANSparkMax *r_back_);

  void TankDrive(double y_axis, double rot_axis, bool turbo_button, bool turtle_button);

 private:
  rev::CANEncoder l_front_encoder;
  rev::CANEncoder r_front_encoder;
  rev::CANEncoder l_back_encoder;
  rev::CANEncoder r_back_encoder;

  rev::CANPIDController l_front_PID;
  rev::CANPIDController r_front_PID;
  rev::CANPIDController l_back_PID;
  rev::CANPIDController r_back_PID;
  
  rev::CANSparkMax* l_front;
  rev::CANSparkMax* r_front;
  rev::CANSparkMax* l_back;
  rev::CANSparkMax* r_back;

  // SPPED VARIABLES
  double const kTurtleSpeed = 0.2;
  double const kNormalSpeed = 0.4;
  double const kTurboSpeed = 0.8;

  double current_speed = 0.0;

  // TNAK DRIVE CONSTANTS
  const double kJoystickDeadband = 0.2;

  // TANK DRIVE VARIABLES
  double y_speed;
  double rot_speed;

  double left_final_speed;
  double right_final_speed;
};