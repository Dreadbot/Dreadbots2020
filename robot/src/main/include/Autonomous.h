#pragma once

#include <map>

#include "SparkDrive.h"

enum AutonState
{
  intake, feeder, shooter, 
  rotate_to_angle, trajectory_following
};

class Autonomous
{
 public:
  Autonomous(frc::Timer* timer_, SparkDrive *spark_drive_);

  void AutonomousInit(std::multimap<units::second_t, AutonState>* time_state_map_);
  void AutonomousPeriodic();

  AutonState GetAutonStateFromCurrentTime();

 private:
  frc::Timer* timer;

  SparkDrive* spark_drive;

  std::multimap<units::second_t, AutonState>* time_state_map;

  AutonState current_state;

  units::time::second_t current_time;
  units::time::second_t lower_bound_time;
};