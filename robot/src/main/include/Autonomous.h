#pragma once

#include <vector>

#include "SparkDrive.h"
#include "Manipulator.h"
#include "TeleopFunctions.h"
#include "Teleoperated.h"

enum AutonState
{
  autonomous_vision_aided_shoot_by_number_of_punches = 0,
  autonomous_blind_shoot_by_number_of_punches = 1,
  autonomous_drive_forward_default = 2,
  autonomous_absolute_rotate = 3,
  autonomous_relative_rotate = 4
};

class Autonomous
{
 public:
  Autonomous(Teleoperated* teleoperated_, TeleopFunctions* teleop_functions_, SparkDrive* spark_drive_, Manipulator* manipulator_);

  void AutonomousInit(std::vector<std::pair<AutonState, double>>* state_by_index_);
  void AutonomousPeriodic();

  void AutonomousVisionAidedShooting(int num_shots);
  void AutonomousBlindShooting(int num_shots);
  void AutonomousDriving(double distance);
  void AutonomousAbsoluteRotate(double angle);
  void AutonomousRelativeRotate(double angle);

 private:
  Teleoperated* teleoperated;
  TeleopFunctions* teleop_functions;
  SparkDrive* spark_drive;
  Manipulator* manipulator;

  std::vector<std::pair<AutonState, double>>* state_by_index;
  unsigned int current_index;

  double current_drive_value;
  double zero_drive_value;
  double drive_target;

  double current_angle_value;
  double zero_angle_value;
  double angle_target;

  bool started_driving;
  bool started_rotating;
};