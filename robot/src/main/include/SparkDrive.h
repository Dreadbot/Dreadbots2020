/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <rev/CANSparkMax.h>

#include <frc/controller/SimpleMotorFeedforward.h>
#include <frc/kinematics/DifferentialDriveKinematics.h>
#include <frc/kinematics/DifferentialDriveOdometry.h>
#include <frc/kinematics/DifferentialDriveWheelSpeeds.h>
#include <frc/geometry/Pose2d.h>

#include <AHRS.h>

class SparkDrive
{
 public:
  /**
   * SparkDrive class constructor
   * 
   * The constructor for the SparkDrive constructor takes in 4 motor inputs,
   * and those motors are used for any drive method within the class. Within
   * the block, the motor PID Settings are set up and are configurable.
   * 
   * @param l_front_ The Left Front motor on the 4-wheel tank drive.
   * @param r_front_ The Right Front motor on the 4-wheel tank drive.
   * @param l_back_ The Left Back motor on the 4-wheel tank drive.
   * @param r_back_ The Right Back motor on the 4-wheel tank drive.
  */
  SparkDrive(rev::CANSparkMax *l_front_, rev::CANSparkMax *r_front_, rev::CANSparkMax *l_back_, rev::CANSparkMax *r_back_);

  /**
   * Dreadbot Tank Drive Function
   * 
   * Takes a forward/backward factor input, a rotation factor input, 
   * turbo and turtle buttons to change speeds and calculates 4 motor
   * controller speed inputs.
   * 
   * @param y_axis Forward/Backward facing axis of type double with a range from -1.0 to 1.0
   * @param rot_axis Rotation speed factor of type double with a range of -1.0 to 1.0
   * @param turbo_button Whether or not to use turbo mode, or twice the speed of normal.
   * @param turtle_button Whether or not to use turbo mode, or half the speed of normal.
  */
  void TankDrive(double y_axis, double rot_axis, bool turbo_button, bool turtle_button);

  /**
   * Utility Function for Getting DifferentialDriveWheelSpeeds object 
   * from encoder velocities and other constants
   * 
   * @return The DifferentialDriveWheelSpeeds pointer object for calculations.
   */
  frc::DifferentialDriveWheelSpeeds* GetDifferentialDriveWheelSpeeds();

  /**
   * Utility Function for Getting the Differential Drive Kinematics Object
   * 
   * @return The DifferentialDriveKinematics Pointer Object.
   */
  frc::DifferentialDriveKinematics* GetDifferentialDriveKinematics();

  /**
   * Utility Function fro Getting the Current position of the robot, relative
   * to its last reset position
   * 
   * @return The Robot's Current Pose2d
   */
  frc::Pose2d* GetRobotPose2dPosition();

  /**
   * Sets the current SparkMAX power level in volts
   * 
   * @param left_volts The Left side of the robot motors' power in volts
   * @param right_volts The Right side of the robot motors' power in volts
   */
  void SetOutputVolts(double left_volts, double right_volts);

  /**
   * Zeroes the current position of the robot to the current position.
   */
  void ResetRobotPosition();

  /**
   * Method to update odometry in order to perform more accurate calculations.
   */
  void SparkDriveAutonomousPeriodic();

 private:
  // Gyroscope Objects
  AHRS* gyro;
  
  // Motor Encoder Objects
  rev::CANEncoder l_front_encoder;
  rev::CANEncoder r_front_encoder;
  rev::CANEncoder l_back_encoder;
  rev::CANEncoder r_back_encoder;

  // Motor PID Controller Objects
  rev::CANPIDController l_front_PID;
  rev::CANPIDController r_front_PID;
  rev::CANPIDController l_back_PID;
  rev::CANPIDController r_back_PID;
  
  // Motor Controller Objects
  rev::CANSparkMax* l_front;
  rev::CANSparkMax* r_front;
  rev::CANSparkMax* l_back;
  rev::CANSparkMax* r_back;

  // Trajectory Utilities
  frc::DifferentialDriveKinematics* differential_drive_kinematics;
  frc::DifferentialDriveOdometry* differential_drive_odometry;

  const double kGearRatio = 0.77;
  const double kWheelRadiusMeters = 1.00;

  // Speed Variables
  double const kTurtleSpeed = 0.2;
  double const kNormalSpeed = 0.4;
  double const kTurboSpeed = 0.8;

  // The Speed Setting during a call of TankDrive
  double current_speed = 0.0;

  // Joystick Deadband Setting
  const double kJoystickDeadband = 0.2;

  // Tank Drive Function Variables
  double y_speed;
  double rot_speed;

  double left_final_speed;
  double right_final_speed;
};