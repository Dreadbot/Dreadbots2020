#include "RamseteTimedFollower.h"

RamseteTimedFollower::RamseteTimedFollower(SparkDrive* spark_drive_,
  TrajectoryGenerationUtility* trajectory_generation_utility_,
  frc::RamseteController ramsete_controller_,
  frc::DifferentialDriveKinematics differential_drive_kinematics_)
    : spark_drive( spark_drive_ ), 
      trajectory_generation_utility( trajectory_generation_utility_ ),
      ramsete_controller( ramsete_controller_ ),
      differential_drive_kinematics( differential_drive_kinematics_ )
{}

void RamseteTimedFollower::Initialize()
{
  previous_time = 0_s;
  
}

void RamseteTimedFollower::Calculate()
{

}