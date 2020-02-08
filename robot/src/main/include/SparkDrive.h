/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <rev/CANSparkMax.h>
#include <rev/ControlType.h>

#include <frc/controller/SimpleMotorFeedforward.h>
#include <frc/kinematics/DifferentialDriveKinematics.h>
#include <frc/kinematics/DifferentialDriveOdometry.h>
#include <frc/kinematics/DifferentialDriveWheelSpeeds.h>
#include <frc/geometry/Pose2d.h>

#include <units/units.h>

#include <AHRS.h>

using Velocity =
    units::compound_unit<units::meters, units::inverse<units::seconds>>;
using Acceleration =
    units::compound_unit<Velocity, units::inverse<units::seconds>>;
using kv_unit = units::compound_unit<units::volts, units::inverse<Velocity>>;
using ka_unit =
    units::compound_unit<units::volts, units::inverse<Acceleration>>;

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
   * @param left_front_ The Left Front motor on the 4-wheel tank drive.
   * @param right_front_ The Right Front motor on the 4-wheel tank drive.
   * @param left_back_ The Left Back motor on the 4-wheel tank drive.
   * @param right_back_ The Right Back motor on the 4-wheel tank drive.
  */
  SparkDrive(rev::CANSparkMax *left_front_, rev::CANSparkMax *right_front_, rev::CANSparkMax *left_back_, rev::CANSparkMax *right_back_);

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
   *Overload of Dreadbot Tank Drive Function
   * 
   * Takes a forward/backward factor input, a rotation factor input,
   * turbo and turtle button to change speeds, and a joystick deadband value,
   * and calculates 4 motor controller speed inputs.
   * Overloaded TankDrive in order to control joystick deadband when needed, 
   * such as for autonomous uses of the function
   * 
   * y_axis Forward/Backward facing axis of type double with a range from -1.0 to 1.0
   * @param rot_axis Rotation speed factor of type double with a range of -1.0 to 1.0
   * @param turbo_button Whether or not to use turbo mode, or twice the speed of normal.
   * @param turtle_button Whether or not to use turbo mode, or half the speed of normal.
   * @param joystick_deadband Define the range in which the values of y_axis and rot_axis are considerd zero, from 0.0 to 1.0
  */
  void TankDrive(double y_axis, double rot_axis, bool turbo_button, bool turtle_button, double joystick_deadband);

  /**
   * Utility Function for Getting the current angle of the gyroscope as a Rotation 2d.
   */
  double GetGyroscopeHeading();

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

  /**
   * Getter for the AHRS Gyroscope on the RoboRIO
   * 
   * @return The AHRS Gyroscope Object
   */
  AHRS* GetGyroscope();

  /**
   * Getter for the Left Front Encoder Object
   * 
   * @return The Left Front Encoder Object
   */
  rev::CANEncoder GetLeftFrontEncoder();

  /**
   * Getter for the Right Front Encoder Object
   * 
   * @return The Right Front Encoder Object
   */
  rev::CANEncoder GetRightFrontEncoder();

  /**
   * Getter for the Left Back Encoder Object
   * 
   * @return The Left Back Encoder Object
   */
  rev::CANEncoder GetLeftBackEncoder();
  
  /**
   * Getter for the Right Back Encoder Object
   * 
   * @return The Right Back Encoder Object
   */
  rev::CANEncoder GetRightBackEncoder();

  /**
   * Getter for the Left Front PID Controller Object
   * 
   * @return The Left Front PID Controller Object
   */
  rev::CANPIDController GetLeftFrontPIDController();
  
  /**
   * Getter for the Right Front PID Controller Object
   * 
   * @return The Right Front PID Controller Object
   */
  rev::CANPIDController GetRightFrontPIDController();
  
  /**
   * Getter for the Left Back PID Controller Object
   * 
   * @return The Left Back PID Controller Object
   */
  rev::CANPIDController GetLeftBackPIDController();
  
  /**
   * Getter for the Right Back PID Controller Object
   * 
   * @return The Right Back PID Controller Object
   */
  rev::CANPIDController GetRightBackPIDController();

  /**
   * Getter for the Left Front Motor Controller Object
   * 
   * @return The Left Front Motor Controller Object
   */
  rev::CANSparkMax* GetLeftFrontMotorController();
  
  /**
   * Getter for the Right Front Motor Controller Object
   * 
   * @return The Right Front Motor Controller Object
   */
  rev::CANSparkMax* GetRightFrontMotorController();
  
  /**
   * Getter for the Left Back Motor Controller Object
   * 
   * @return The Left Back Motor Controller Object
   */
  rev::CANSparkMax* GetLeftBackMotorController();
  
  /**
   * Getter for the Right Back Motor Controller Object
   * 
   * @return The Right Back Motor Controller Object
   */
  rev::CANSparkMax* GetRightBackMotorController();

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
   * Utility Function for Getting the Simple Motor Feedforward Object
   * 
   * @return The Simple Motor Feedforward Object
   */
  frc::SimpleMotorFeedforward<units::meters> GetSimpleMotorFeedforward();

  /**
   * Utility Function for Getting the Current position of the robot, relative
   * to its last reset position
   * 
   * @return The Robot's Current Pose2d
   */
  frc::Pose2d GetRobotPose2dPosition() const;

 private:
  // Gyroscope Objects
  AHRS* gyro;

  // Motor Encoder Objects
  rev::CANEncoder left_front_encoder;
  rev::CANEncoder right_front_encoder;
  rev::CANEncoder left_back_encoder;
  rev::CANEncoder right_back_encoder;

  // Motor PID Controller Objects
  rev::CANPIDController left_front_PID;
  rev::CANPIDController right_front_PID;
  rev::CANPIDController left_back_PID;
  rev::CANPIDController right_back_PID;
  
  // Motor Controller Objects
  rev::CANSparkMax* left_front;
  rev::CANSparkMax* right_front;
  rev::CANSparkMax* left_back;
  rev::CANSparkMax* right_back;

  // Trajectory Utilities
  frc::DifferentialDriveKinematics* differential_drive_kinematics;
  frc::DifferentialDriveOdometry* differential_drive_odometry;

  frc::SimpleMotorFeedforward<units::meters> simple_motor_feedforward;

  frc::Pose2d robot_current_position;

  // Wheel Constants
  const double kGearRatio = 0.77;
  const double kWheelRadiusMeters = 1.00;

  // Feedforward Constants
  const units::volt_t kStaticGain = units::volt_t(0.3);
  const units::unit_t<kv_unit> kVoltsPerSecondPerDistance = units::unit_t<kv_unit>(1.96, 1);
  const units::unit_t<ka_unit> kVoltSecondsSquaredPerDistance = units::unit_t<ka_unit>(0.06, 1);

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