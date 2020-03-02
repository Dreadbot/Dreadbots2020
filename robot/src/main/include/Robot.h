/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <iostream>
#include <string>

#include <frc/Joystick.h>
#include <frc/TimedRobot.h>
#include <frc/Timer.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/smartdashboard/SmartDashboard.h>

#include "Diagnostic.h"
#include "Intake.h"
#include "RamseteTimedFollower.h"
#include "RobotUtilities.h"
#include "SparkDrive.h"
#include "Teleoperated.h"
#include "TeleopFunctions.h"
#include "TrajectoryGenerationUtility.h"
#include "Manipulator.h"
#include "Autonomous.h"
#include "ColorWheel.h"
#include "Ultra.h"
#include "Climber.h"

class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;
  void RobotPeriodic() override;
  void AutonomousInit() override;
  void AutonomousPeriodic() override;
  void TeleopInit() override;
  void TeleopPeriodic() override;
  void TestPeriodic() override;

 private:
  frc::SendableChooser<std::string> m_chooser;
  const std::string AutoDefault = "Default";
  const std::string AutoRightRight = "RightRight";
  const std::string AutoRightCenter = "RightCenter";
  const std::string AutoRightLeft = "RightLeft";
  std::string m_autoSelected;

  // Timer Object
  frc::Timer* timer;

  // Autonomous Functions
  TrajectoryGenerationUtility* trajectory_generation_utility;
  
  RamseteTimedFollower* ramsete_timed_follower;

  // Input Objects
  frc::Joystick *joystick_1;
  frc::Joystick *joystick_2;

  // Drive Objects
  SparkDrive* spark_drive;

  //Autonomous Object Container
  Autonomous *autonomous;

  // Teleoperated Object Container
  Teleoperated* teleoperated;

  //Ultrasonic Objecttss
  Ultra *ultra;

  //Shooting Objects
  double position;
  double P, I, D;

  // Trajectory Testing Variables
  double const kIterationSecondsRatio = 0.02;

   //BUTTONS
  int const shooterButton = 1;
  
  double iterative_clock = 0.0;

  //Rotate to angle variables
  double selectedAngle = 0;
  int lastCount = 0;
  int staleCount = 0;

  //ENABLED BOOLEANS
  const bool kDriveEnabled = true;
  const bool kClimbEnabled = false;
  const bool kShooterEnabled = true;
  const bool kRotateToAngleEnabled = true;
  const bool kIntakeEnabled = true;
  const bool kFeederEnabled = true;
  const bool kTrajectoryEnabled = false;
  const bool kColorWheelEnabled = false;

  int enabled_subsystems;

  // Teleop Objects
    //Manipulator Objects
    rev::CANSparkMax *intake_motor;
    rev::CANSparkMax *shooter_motor;
    rev::CANSparkMax *aim_motor;
    rev::CANSparkMax *geneva_motor;
    frc::Solenoid *punch;
    frc::Solenoid *intake_pin;
    Intake *intake;
    Feeder *feeder;
    Shooter *shooter;
    Manipulator *manipulator;
    double ContinuousShooterSpeed = 0.0;
  TeleopFunctions *teleop_functions;
  Climber *climber;
  rev::CANSparkMax *climb_winch;
  rev::CANSparkMax *climb_telescope;
  ColorWheel *color_wheel;
  WPI_TalonSRX *color_motor;
  frc::Solenoid *color_sol;
  Diagnostic* test;

};
