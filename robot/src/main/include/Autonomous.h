#pragma once

#include <map>

#include "SparkDrive.h"

enum AutonState
{
intake, feeder, shooter, drive
};

class Autonomous
{
 public:
  Autonomous(SparkDrive *Sparkdrive, std::multimap<units::second_t, AutonState>* time_to_state);
  
  void RightRight();
  void RightCenter();
  void RightLeft();

 private:
 SparkDrive *m_SparkDrive;

 std::multimap<units::second_t, AutonState>* m_time_to_state;
};