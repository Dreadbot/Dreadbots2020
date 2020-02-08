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
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/smartdashboard/SmartDashboard.h>

#include "Diagnostic.h"
#include "Intake.h"
#include "RamseteTimedFollower.h"
#include "RobotUtilities.h"
#include "SparkDrive.h"
#include "TeleopFunctions.h"
#include "TrajectoryGenerationUtility.h"

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
  const std::string kAutoNameDefault = "Default";
  const std::string kAutoNameCustom = "My Auto";
  std::string m_autoSelected;

  // Autonomous Functions
  TrajectoryGenerationUtility* trajectory_generation_utility;
  
  RamseteTimedFollower* ramsete_timed_follower;

  // Input Objects
  frc::Joystick *joystick_1;

  // Drive Objects
  SparkDrive* spark_drive;

  // Trajectory Testing Variables
  double const kIterationSecondsRatio = 0.02;

   //BUTTONS
  int const shooterButton = 1;
  
  double iterative_clock = 0.0;

  // Teleop Objects
  TeleopFunctions *teleop_functions;
  Shooter *shooter;
  Intake* intake;
  Diagnostic* test;

};
