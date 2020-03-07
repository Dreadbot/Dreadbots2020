#pragma once

#include <map>

#include "autonomous/DriveForwardAutonState.h"
#include "autonomous/RotateToAngleAutonState.h"
#include "autonomous/RotateShootAutonState.h"

#include "SparkDrive.h"
#include "Intake.h"
#include "TeleopFunctions.h"

class Autonomous
{
 public:
  Autonomous(TeleopFunctions* teleop_functions_, SparkDrive* spark_drive_, Manipulator* manipulator_);

  void AutonomousInit();
  void AutonomousPeriodic();

 private:
  TeleopFunctions* teleop_functions;
  SparkDrive* spark_drive;
  Manipulator* manipulator;

  std::vector<AutonState>* auton_routine;
  unsigned int current_auton_index = 0;
};