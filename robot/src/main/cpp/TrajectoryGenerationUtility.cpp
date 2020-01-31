#include "TrajectoryGenerationUtility.h"

TrajectoryGenerationUtility::TrajectoryGenerationUtility()
{}

void TrajectoryGenerationUtility::GenerateTestTrajectory()
{
  // Starting Pose2d (X-Axis Position, Y-Axis Position, Rotation in Degrees)
  const frc::Pose2d starting_position{0_ft, 0_ft, frc::Rotation2d(0_deg)};j

  // Ending Pose2d (X-Axis Position, Y-Axis Position, Rotation in Degrees)
  const frc::Pose2d ending_position{2_ft, 2_ft, frc::Rotation2d(90_deg)};

  // Group of interior waypoints along a path (To avoid obstacles on the field)
  std::vector<frc::Translation2d> interior_waypoints {};

  // Configuration and other settings for the trajectory generator.
  frc::TrajectoryConfig config{12_fps, 12_fps_sq};
  config.SetReversed(false);

  // Generate the trajectory using the starting points, intermediate points,
  // ending point, and configration.
  auto trajectory = frc::TrajectoryGenerator::GenerateTrajectory(
    starting_position, interior_waypoints, ending_position, config
  );

  // For testing purposes, serialize the trajectory and print it to the RIOLog.
  std::cout << frc::TrajectoryUtil::SerializeTrajectory(trajectory) << std::endl;
}