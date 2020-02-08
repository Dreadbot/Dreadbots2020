/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <frc/Timer.h>
#include <frc/controller/RamseteController.h>
#include <frc/geometry/Pose2d.h>
#include <frc/kinematics/DifferentialDriveKinematics.h>
#include <frc/trajectory/Trajectory.h>
#include <units/units.h>
#include <wpi/ArrayRef.h>

#include "SparkDrive.h"
#include "TrajectoryGenerationUtility.h"

class RamseteTimedFollower
{
 public:
  /**
   * Default Class Constructor for RamseteTimedFollower
   * 
   * @param spark_drive_ The SparkDrive object used on the robot.
   * @param trajectory_generation_utility_ The utility used to generate the trajectories.
   */
  RamseteTimedFollower(SparkDrive* spark_drive_,
    TrajectoryGenerationUtility* trajectory_generation_utility_);
  
  /**
   * Initializes the Timed Follower, Preparing for Runtime Interpretation.
   */
  void Initialize();

  /**
   * Timed command to calculate current outputs of the motors on the given
   * trajectory.
   */
  void Calculate();
  
 private:
  SparkDrive* spark_drive;

  TrajectoryGenerationUtility* trajectory_generation_utility;

  frc::Timer timer;

  units::second_t previous_time;
  frc::DifferentialDriveWheelSpeeds previous_speeds; 
};