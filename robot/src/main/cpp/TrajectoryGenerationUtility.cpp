/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "TrajectoryGenerationUtility.h"

TrajectoryGenerationUtility::TrajectoryGenerationUtility()
{
  // Example Code for Creating a Trajectory

  // First (Robot Position? Robot Starting Position?)
  // & Last Position (Destination Position)
  //
  // Positions Contain x & y coordinates as well as rotations on a 0-360 degree scale.
  start_position = new frc::Pose2d(1.54_ft, 23.23_ft, frc::Rotation2d(180_deg));
  end_position = new frc::Pose2d(23.7_ft, 6.8_ft, frc::Rotation2d(-160_deg));

  // Optional interior waypoints if we want to include them.
  std::vector<frc::Translation2d> interior_waypoints{
    frc::Translation2d{14.54_ft, 23.23_ft},
    frc::Translation2d{21.04_ft, 18.23_ft}
  };

  // Configurations (Special Rules)
  frc::TrajectoryConfig config{12_fps, 12_fps_sq};
  config.SetReversed(true);

  // Get the trajectory from the static 
  // GenerateTrajectory(start, interior_waypoints, end, configuration) method.
  auto trajectory = frc::TrajectoryGenerator::GenerateTrajectory(
    *start_position, interior_waypoints, *end_position, config
  );

  std::cout << frc::TrajectoryUtil::SerializeTrajectory(trajectory) << std::endl;
}