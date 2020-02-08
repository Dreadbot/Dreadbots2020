/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "RamseteTimedFollower.h"

RamseteTimedFollower::RamseteTimedFollower(SparkDrive* spark_drive_,
  TrajectoryGenerationUtility* trajectory_generation_utility_)
    : spark_drive( spark_drive_ ), 
      trajectory_generation_utility( trajectory_generation_utility_ )
{
  timer = frc::Timer();
}

void RamseteTimedFollower::Initialize()
{
  // Set time to 0
  previous_time = 0_s;
  
  // Get the initial trajectory state
  auto initial_state = trajectory_generation_utility->GetTrajectory().Sample(0_s);
  
  // Get the previous speeds of wheels
  previous_speeds = spark_drive->GetDifferentialDriveKinematics()->ToWheelSpeeds(
    frc::ChassisSpeeds{initial_state.velocity, 0_mps,
      initial_state.velocity * initial_state.curvature}
  );

  // Reset Timer
  timer.Reset();
  timer.Start();

  // Reset PID Controllers
  spark_drive->GetLeftRamsetePIDController()->Reset();
  spark_drive->GetRightRamsetePIDController()->Reset();
}

void RamseteTimedFollower::Calculate()
{
  // Timing Calculations
  units::time::second_t current_time = units::time::second_t(timer.Get());
  units::time::second_t delta_time = current_time - previous_time;

  // Get the target wheel speeds for
  // this iteration in seconds.
  frc::DifferentialDriveWheelSpeeds target_wheel_speeds = spark_drive->GetDifferentialDriveKinematics()->ToWheelSpeeds(
    trajectory_generation_utility->GetRamseteController()->Calculate(
      spark_drive->GetRobotPose2dPosition(),
      trajectory_generation_utility->GetTrajectory().Sample(current_time)
    )
  );
  
  // Calculate Feedfowards from Target and Previous Speeds.
  units::voltage::volt_t left_feedforward = spark_drive->GetSimpleMotorFeedforward().Calculate(
    target_wheel_speeds.left,
    (target_wheel_speeds.left - previous_speeds.left) / delta_time
  );

  units::voltage::volt_t right_feedforward = spark_drive->GetSimpleMotorFeedforward().Calculate(
    target_wheel_speeds.right,
    (target_wheel_speeds.right - previous_speeds.right) / delta_time
  );

  // Calculate Final Outputs Accounting for Current Speed
  units::voltage::volt_t left_output = units::voltage::volt_t(spark_drive->GetLeftRamsetePIDController()->Calculate(
    spark_drive->GetDifferentialDriveWheelSpeeds()->left.to<double>(),
    target_wheel_speeds.left.to<double>()
  )) + left_feedforward;

  units::voltage::volt_t right_output = units::voltage::volt_t(spark_drive->GetRightRamsetePIDController()->Calculate(
    spark_drive->GetDifferentialDriveWheelSpeeds()->right.to<double>(),
    target_wheel_speeds.right.to<double>()
  )) + right_feedforward;

  // Send voltages to SparkDrive as outputs
  spark_drive->SetOutputVolts(left_output, right_output);
}