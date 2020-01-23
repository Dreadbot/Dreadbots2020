/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "SparkDrive.h"

SparkDrive::SparkDrive(rev::CANSparkMax *l_front_, rev::CANSparkMax *r_front_, rev::CANSparkMax *l_back_, rev::CANSparkMax *r_back_)
: l_front_encoder(l_front_->GetEncoder()), r_front_encoder(r_front_->GetEncoder()), l_back_encoder(l_back_->GetEncoder()), r_back_encoder(r_back_->GetEncoder()), l_front_PID(l_front_->GetPIDController()), r_front_PID(r_front_->GetPIDController()), l_back_PID(l_back_->GetPIDController()), r_back_PID(r_back_->GetPIDController())
{
  l_front = l_front_;
  r_front = r_front_;
  l_back = l_back_;
  r_back = r_back_;

  l_front_PID.SetP(0.2);
  l_front_PID.SetI(1e-4);
  l_front_PID.SetD(1);
  l_front_PID.SetIZone(0.1);
  l_front_PID.SetFF(0);
  l_front_PID.SetOutputRange(-1, 1);

	r_front_PID.SetP(0.2);
  r_front_PID.SetI(1e-4);
  r_front_PID.SetD(1);
  r_front_PID.SetIZone(0.1);
  r_front_PID.SetFF(0);
  r_front_PID.SetOutputRange(-1, 1);

	l_back_PID.SetP(0.2);
  l_back_PID.SetI(1e-4);
  l_back_PID.SetD(1);
  l_back_PID.SetIZone(0.1);
  l_back_PID.SetFF(0);
  l_back_PID.SetOutputRange(-1, 1);

	r_back_PID.SetP(0.2);
  r_back_PID.SetI(1e-4);
  r_back_PID.SetD(1);
  r_back_PID.SetIZone(0.1);
  r_back_PID.SetFF(0);
  r_back_PID.SetOutputRange(-1, 1);
}

void SparkDrive::TankDrive(double yAxis, double rotAxis, bool turboButton, bool turtleButton)
{
  // Account for Joystick Deadband
  yAxis = (fabs(yAxis) < kJoystickDeadband) ? 0 : yAxis;
  rotAxis = (fabs(rotAxis) < kJoystickDeadband) ? 0 : rotAxis;

  // Flipping Variables to change behavior for rotation influence.
  rot_speed = (fabs(rotAxis) > 0.0) ? -rot_speed : rot_speed;

  // Configure Speed
  if(turboButton)
  {
    current_speed = kTurboSpeed;
  } else if(turtleButton)
  {
    current_speed = kTurtleSpeed;
  } else 
  {
    current_speed = kNormalSpeed;
  }

  // Multiply Intended Velocity by a cap speed.
  // (See main.include.Robot.h)
  y_speed = yAxis * current_speed;
  rot_speed = rotAxis * current_speed;

  // Calculating Final Speed
  // by Adding the Rotation Factor
  left_final_speed = y_speed + -rot_speed;
  right_final_speed = y_speed + rot_speed;

  if(left_final_speed > 1){
    left_final_speed = 1;
  }
  if(left_final_speed < -1){
    left_final_speed = -1;
  }
  if(right_final_speed > 1){
    right_final_speed = 1;
  }
  if(right_final_speed < -1){
    right_final_speed = -1;
  }

  // Set Motor Values
  std::cout << "left_final_speed: " << left_final_speed << " right_final_speed: " << right_final_speed << std::endl;
  l_front->Set(left_final_speed);
  l_back->Set(left_final_speed);
  r_front->Set(right_final_speed);
  r_back->Set(right_final_speed);
}