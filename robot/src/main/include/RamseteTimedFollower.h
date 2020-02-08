/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

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
  RamseteTimedFollower(SparkDrive* spark_drive_,
    TrajectoryGenerationUtility* trajectory_generation_utility_,
    frc::RamseteController ramsete_controller_,
    frc::DifferentialDriveKinematics differential_drive_kinematics_);
  
  void Initialize();
  void Calculate();
 private:
  SparkDrive* spark_drive;

  TrajectoryGenerationUtility* trajectory_generation_utility;

  frc::RamseteController ramsete_controller;
  frc::DifferentialDriveKinematics differential_drive_kinematics;

  units::second_t previous_time;
  frc::DifferentialDriveWheelSpeeds previous_speeds; 
};