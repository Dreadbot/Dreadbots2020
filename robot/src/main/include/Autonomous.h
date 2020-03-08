#pragma once

#include <vector>

#include "SparkDrive.h"
#include "Manipulator.h"
#include "TeleopFunctions.h"

enum AutonState
{
  shooting = 0,
  driving = 1
};

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

  std::vector<std::pair<AutonState, int>>* state_by_index;
  unsigned int current_index;
};