/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "SparkDrive.h"

SparkDrive::SparkDrive(rev::CANSparkMax *left_front_, 
  rev::CANSparkMax *right_front_, 
  rev::CANSparkMax *left_back_, 
  rev::CANSparkMax *right_back_)
: left_front_encoder( left_front_->GetEncoder() ), 
  right_front_encoder( right_front_->GetEncoder() ), 
  left_back_encoder( left_back_->GetEncoder() ), 
  right_back_encoder( right_back_->GetEncoder() ), 
  left_front_PID( left_front_->GetPIDController() ), 
  right_front_PID( right_front_->GetPIDController() ), 
  left_back_PID( left_back_->GetPIDController() ), 
  right_back_PID( right_back_->GetPIDController() )
{
  // The reason these variables are initialized here 
  // and not in the initialization list is purely for
  // ordering. These are initialized first, 
  // The objects within the initilization list, since
  // the objects in the list depend on the motor 
  // controllers.
  left_front = left_front_;
  right_front = right_front_;
  left_back = left_back_;
  right_back = right_back_;
  
  // Gyroscope Definition & 
  // Reset the Gyroscope to a 0 position.
  gyro = new AHRS(frc::SPI::Port::kMXP);
  gyro->Reset();

  // PID Settings
  //
  // TODO: These will probably need tuning at a 
  // later time when we finish the bot.
  //
  // Left Front Motor PID Settings
  left_front_PID.SetP(0.2);
  left_front_PID.SetI(1e-4);
  left_front_PID.SetD(1);
  left_front_PID.SetIZone(0.1);
  left_front_PID.SetFF(0);
  left_front_PID.SetOutputRange(-1, 1);

  // Right Front Motor PID Settings
	right_front_PID.SetP(0.2);
  right_front_PID.SetI(1e-4);
  right_front_PID.SetD(1);
  right_front_PID.SetIZone(0.1);
  right_front_PID.SetFF(0);
  right_front_PID.SetOutputRange(-1, 1);

  // Left Back Motor PID Settings
	left_back_PID.SetP(0.2);
  left_back_PID.SetI(1e-4);
  left_back_PID.SetD(1);
  left_back_PID.SetIZone(0.1);
  left_back_PID.SetFF(0);
  left_back_PID.SetOutputRange(-1, 1);

  // Right Back Motor PID Settings
	right_back_PID.SetP(0.2);
  right_back_PID.SetI(1e-4);
  right_back_PID.SetD(1);
  right_back_PID.SetIZone(0.1);
  right_back_PID.SetFF(0);
  right_back_PID.SetOutputRange(-1, 1);

  simple_motor_feedforward = frc::SimpleMotorFeedforward<units::meters>(kStaticGain, kVoltsPerSecondPerDistance, kVoltSecondsSquaredPerDistance);
}

void SparkDrive::TankDrive(double y_axis, 
  double rot_axis, 
  bool turbo_button, 
  bool turtle_button)
{
  // Remove any possibility of Joystick Deadband.
  // Essentially, if the joystick input is within
  // the range of the considered deadband (-0.2 to 0.2),
  // Consider the input as 0.0
  y_axis = (fabs(y_axis) < kJoystickDeadband) ? 0 : y_axis;
  rot_axis = (fabs(rot_axis) < kJoystickDeadband) ? 0 : rot_axis;

  // Set the speed mode from turbo_button and turtle_button.
  // In the case that both turtle mode and turbo mode are
  // both enabled, turbo mode overrides turtle. 
  // (This can absolutely be changed if needed)
  if(turbo_button)
  {
    current_speed = kTurboSpeed;
  } else if(turtle_button)
  {
    current_speed = kTurtleSpeed;
  } else 
  {
    current_speed = kNormalSpeed;
  }

  // Multiply the axes by the current speed setting
  // the -2 for rotation speed is simply because it
  // takes more force to turn due to friction, so we
  // use twice the speed.
  y_speed = y_axis * current_speed;
  rot_speed = -2 * rot_axis * current_speed;

  // Calculating Final Speed by adding both the
  // Forward/Backward speed (y_speed) and Rotation
  // Speeds (rot_speed)
  left_final_speed = -y_speed - rot_speed;
  right_final_speed = y_speed - rot_speed;

  // Setting the final speed to a max of 100%
  // To prevent any errors if we reach the limit.
  // Essentially, if out of bounds, set it to the
  // most extreme bounds to prevent errors.
  left_final_speed = (left_final_speed > 1.0) ? 1.0 : left_final_speed;
  left_final_speed = (left_final_speed < -1.0) ? -1.0 : left_final_speed;
  right_final_speed = (right_final_speed > 1.0) ? 1.0 : right_final_speed;
  right_final_speed = (right_final_speed < -1.0) ? -1.0 : right_final_speed;

  // Set Motor Speeds to the final speeds calculated.
  left_front->Set(left_final_speed);
  left_back->Set(left_final_speed);
  right_front->Set(right_final_speed);
  right_back->Set(right_final_speed);
}

void SparkDrive::TankDrive(double y_axis, 
  double rot_axis, 
  bool turbo_button, 
  bool turtle_button, 
  double joystick_deadband)
{
   // Remove any possibility of Joystick Deadband.
  // Essentially, if the joystick input is within
  // the range of the considered deadband
  // Consider the input as 0.0
  y_axis = (fabs(y_axis) < joystick_deadband) ? 0 : y_axis;
  rot_axis = (fabs(rot_axis) < joystick_deadband) ? 0 : rot_axis;

  // Set the speed mode from turbo_button and turtle_button.
  // In the case that both turtle mode and turbo mode are
  // both enabled, turbo mode overrides turtle. 
  // (This can absolutely be changed if needed)
  if(turbo_button)
  {
    current_speed = kTurboSpeed;
  } else if(turtle_button)
  {
    current_speed = kTurtleSpeed;
  } else 
  {
    current_speed = kNormalSpeed;
  }

  // Multiply the axes by the current speed setting
  // the -2 for rotation speed is simply because it
  // takes more force to turn due to friction, so we
  // use twice the speed.
  y_speed = y_axis * current_speed;
  rot_speed = -2 * rot_axis * current_speed;

  // Calculating Final Speed by adding both the
  // Forward/Backward speed (y_speed) and Rotation
  // Speeds (rot_speed)
  left_final_speed = -y_speed - rot_speed;
  right_final_speed = y_speed - rot_speed;

  // Setting the final speed to a max of 100%
  // To prevent any errors if we reach the limit.
  // Essentially, if out of bounds, set it to the
  // most extreme bounds to prevent errors.
  left_final_speed = (left_final_speed > 1.0) ? 1.0 : left_final_speed;
  left_final_speed = (left_final_speed < -1.0) ? -1.0 : left_final_speed;
  right_final_speed = (right_final_speed > 1.0) ? 1.0 : right_final_speed;
  right_final_speed = (right_final_speed < -1.0) ? -1.0 : right_final_speed;

  // Set Motor Speeds to the final speeds calculated.
  left_front->Set(left_final_speed);
  left_back->Set(left_final_speed);
  right_front->Set(right_final_speed);
  right_back->Set(right_final_speed);
}

double SparkDrive::GetGyroscopeHeading()
{
  return gyro->GetAngle();
}

frc::DifferentialDriveWheelSpeeds* SparkDrive::GetDifferentialDriveWheelSpeeds()
{
  return new frc::DifferentialDriveWheelSpeeds
  {
    (units::meters_per_second_t) (left_front_encoder.GetVelocity() / kGearRatio * 2 * M_PI * kWheelRadiusMeters / 60),
    (units::meters_per_second_t) (right_front_encoder.GetVelocity() / kGearRatio * 2 * M_PI * kWheelRadiusMeters / 60)
  };
}

frc::DifferentialDriveKinematics* SparkDrive::GetDifferentialDriveKinematics()
{
  return differential_drive_kinematics;
}

frc::Pose2d SparkDrive::GetRobotPose2dPosition() const
{
  return robot_current_position;
}

void SparkDrive::SetOutputVolts(units::voltage::volt_t left_volts, units::voltage::volt_t right_volts)
{
  left_front->Set(left_volts.to<double>() / 12);
  left_back->Set(left_volts.to<double>() / 12);
  right_front->Set(left_volts.to<double>() / 12);
  right_back->Set(left_volts.to<double>() / 12);
}

void SparkDrive::ResetRobotPosition()
{
  differential_drive_odometry->ResetPosition(frc::Pose2d(), frc::Rotation2d(units::degree_t(GetGyroscopeHeading())));
}

void SparkDrive::SparkDriveAutonomousPeriodic()
{
  robot_current_position = differential_drive_odometry->Update(
    frc::Rotation2d(units::degree_t(GetGyroscopeHeading())), 
    GetDifferentialDriveWheelSpeeds()->left * 0.02_s, 
    GetDifferentialDriveWheelSpeeds()->right * 0.02_s);
}

AHRS* SparkDrive::GetGyroscope()
{
  return this->gyro;
}

rev::CANEncoder SparkDrive::GetLeftFrontEncoder()
{
  return this->left_front_encoder;
}

rev::CANEncoder SparkDrive::GetRightFrontEncoder()
{
  return this->right_front_encoder;
}

rev::CANEncoder SparkDrive::GetLeftBackEncoder()
{
  return this->left_back_encoder;
}

rev::CANEncoder SparkDrive::GetRightBackEncoder()
{
  return this->right_back_encoder;
}

rev::CANPIDController SparkDrive::GetLeftFrontPIDController()
{
  return this->left_front_PID;
}

rev::CANPIDController SparkDrive::GetRightFrontPIDController()
{
  return this->right_front_PID;
}

rev::CANPIDController SparkDrive::GetLeftBackPIDController()
{
  return this->left_back_PID;
}

rev::CANPIDController SparkDrive::GetRightBackPIDController()
{
  return this->right_back_PID;
}

rev::CANSparkMax* SparkDrive::GetLeftFrontMotorController()
{
  return this->left_front;
}

rev::CANSparkMax* SparkDrive::GetRightFrontMotorController()
{
  return this->right_front;
}

rev::CANSparkMax* SparkDrive::GetLeftBackMotorController()
{
  return this->left_back;
}

rev::CANSparkMax* SparkDrive::GetRightBackMotorController()
{
  return this->right_front;
}

frc::SimpleMotorFeedforward<units::meter> SparkDrive::GetSimpleMotorFeedforward()
{
  return this->simple_motor_feedforward;
}

frc2::PIDController* SparkDrive::GetLeftRamsetePIDController()
{
  return this->left_PID_controller;
}

frc2::PIDController* SparkDrive::GetRightRamsetePIDController()
{
  return this->right_PID_controller;
}