#pragma once

#include <vector>

#include "SparkDrive.h"
#include "Manipulator.h"
#include "TeleopFunctions.h"
#include "Teleoperated.h"

enum AutonState
{
  autonomous_shoot_by_number_of_punches = 0,
  autonomous_drive_forward_default = 1
};

class Autonomous
{
 public:
  Autonomous(Teleoperated* teleoperated_, TeleopFunctions* teleop_functions_, SparkDrive* spark_drive_, Manipulator* manipulator_);

  void AutonomousInit(std::vector<std::pair<AutonState, int>>* state_by_index_);
  void AutonomousPeriodic();

  void AutonomousShooting(int num_shots);
  void AutonomousDriving(int distance);

 private:
  Teleoperated* teleoperated;
  TeleopFunctions* teleop_functions;
  SparkDrive* spark_drive;
  Manipulator* manipulator;

  std::vector<std::pair<AutonState, int>>* state_by_index;
  unsigned int current_index;

  double current_value;
  double zero_value;
  double target;

  bool started_driving;
};