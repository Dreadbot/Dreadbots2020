#include "RamseteTimedFollower.h"

RamseteTimedFollower::RamseteTimedFollower(SparkDrive* spark_drive_,
  TrajectoryGenerationUtility* trajectory_generation_utility_)
    : spark_drive( spark_drive_ ), 
      trajectory_generation_utility( trajectory_generation_utility_ )
{}

void RamseteTimedFollower::Initialize()
{
  previous_time = 0_s;
  
}

void RamseteTimedFollower::Calculate()
{

}