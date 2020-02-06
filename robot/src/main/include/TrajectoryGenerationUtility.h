#pragma once

#include <frc/controller/RamseteController.h>
#include <frc/trajectory/TrajectoryConfig.h>
#include <frc/trajectory/TrajectoryGenerator.h>
#include <frc/trajectory/TrajectoryUtil.h>

#include "SparkDrive.h"

class TrajectoryGenerationUtility
{
 public:
  TrajectoryGenerationUtility();

  void GenerateTestTrajectory(SparkDrive* dreadbot_sparkdrive);

  frc::Trajectory GetTrajectory();

  frc::RamseteController* GetRamseteController();

  frc::ChassisSpeeds GetChassisSpeeds();
 private:
  frc::Trajectory trajectory;

  frc::RamseteController* ramsete_controller;

  frc::ChassisSpeeds chassis_speeds;
};