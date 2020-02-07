#include "TrajectoryGenerationUtility.h"

TrajectoryGenerationUtility::TrajectoryGenerationUtility()
{}

void TrajectoryGenerationUtility::GenerateTestTrajectory(SparkDrive* dreadbot_sparkdrive)
{
  // Starting Pose2d (X-Axis Position, Y-Axis Position, Rotation in Degrees)
  const frc::Pose2d starting_position{0_ft, 0_ft, frc::Rotation2d(0_deg)};

  // Ending Pose2d (X-Axis Position, Y-Axis Position, Rotation in Degrees)
  const frc::Pose2d ending_position{2_ft, 2_ft, frc::Rotation2d(90_deg)};

  // Group of interior waypoints along a path (To avoid obstacles on the field)
  std::vector<frc::Translation2d> interior_waypoints {};

  // Configuration and other settings for the trajectory generator.
  frc::TrajectoryConfig config{12_fps, 12_fps_sq};
  config.SetReversed(false);

  // Generate the trajectory using the starting points, intermediate points,
  // ending point, and configration.
  trajectory = frc::TrajectoryGenerator::GenerateTrajectory(
    starting_position, interior_waypoints, ending_position, config
  );

  // For testing purposes, serialize the trajectory and print it to the RIOLog.
  //std::cout << frc::TrajectoryUtil::SerializeTrajectory(trajectory) << std::endl;

  // Define the Trajectory Tracker
  ramsete_controller = new frc::RamseteController(0.2, 7.0);

  for(double delta_time = 0.00; delta_time < 1.00; delta_time += 0.02)
  {
    chassis_speeds = ramsete_controller->Calculate(dreadbot_sparkdrive->GetRobotPose2dPosition(), trajectory.Sample(units::time::second_t(delta_time)));

    std::cout << chassis_speeds.vx << std::endl;
    std::cout << chassis_speeds.omega << std::endl;
  }
}

frc::Trajectory TrajectoryGenerationUtility::GetTrajectory()
{
  return this->trajectory;
}

frc::RamseteController* TrajectoryGenerationUtility::GetRamseteController()
{
  return this->ramsete_controller;
}

frc::ChassisSpeeds TrajectoryGenerationUtility::GetChassisSpeeds()
{
  return this->chassis_speeds;
}

void TrajectoryGenerationUtility::SetChassisSpeeds(frc::ChassisSpeeds chassis_speeds_)
{
  chassis_speeds = chassis_speeds_;
}