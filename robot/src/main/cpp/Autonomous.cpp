#include "Autonomous.h"

Autonomous::Autonomous(Manipulator* manipulator_, 
  SparkDrive* spark_drive_, 
  Teleoperated* teleoperated_, 
  TeleopFunctions* teleop_functions_)
  : manipulator( manipulator_ ),
    spark_drive( spark_drive_ ),
    teleoperated( teleoperated_ ),
    teleop_functions( teleop_functions_ )
{}

void Autonomous::AutonomousInit(
  std::vector<std::pair<AutonState, double>>* state_by_index_)
{
  state_by_index = state_by_index_;
  current_index = 0;

  started_driving = false;
  started_rotating = false;

  manipulator->GetIntake()->DeployIntake();
  manipulator->GetIntake()->SetIntakeArms(false);

  current_drive_value = 0.0;
}

void Autonomous::AutonomousPeriodic()
{
  if(current_index == state_by_index->size())
  {
    return;
  }

  switch(state_by_index->at(current_index).first)
  {
  case autonomous_vision_aided_shoot_by_number_of_punches:
    // Once the Hood is Calibrated, Begin the Shooting Process.
    if(manipulator->GetShooter()->GetAimReadiness())
    {
      // Vision Assisted Shooting Call
      AutonomousVisionAidedShooting((int) (state_by_index->at(current_index).second));
    }
    break;
  case autonomous_blind_shoot_by_number_of_punches:
    // Once the Hood is Calibrated, Begin the Shooting Process.
    if(manipulator->GetShooter()->GetAimReadiness())
    {
      // "Blind" (Without Vision) Shooting Call
      AutonomousBlindShooting((int) (state_by_index->at(current_index).second));
    }
    break;
  case autonomous_drive_forward_default:
    AutonomousDriving(state_by_index->at(current_index).second);
    break;
  case autonomous_absolute_rotate:
    AutonomousAbsoluteRotate(state_by_index->at(current_index).second);
    break;
  case autonomous_relative_rotate:
    AutonomousRelativeRotate(state_by_index->at(current_index).second);
    break;
  }
}

void Autonomous::AutonomousAbsoluteRotate(double angle)
{
  if(!started_rotating)
  {
    started_rotating = true;
  }

  current_angle_value = spark_drive->GetGyroscope()->GetAngle();

  teleop_functions->WPITurnToAngle(angle);

  if(teleop_functions->GetTurnStatus())
  {
    ++current_index;
    started_rotating = false; 
  }
}

void Autonomous::AutonomousRelativeRotate(double angle)
{
  if(!started_rotating)
  {
    started_rotating = true;
    zero_angle_value = spark_drive->GetGyroscope()->GetAngle();
  }

  current_angle_value = spark_drive->GetGyroscope()->GetAngle();
  angle_target = zero_angle_value + angle;

  teleop_functions->WPITurnToAngle(angle_target);

  if(teleop_functions->GetTurnStatus())
  {
    ++current_index;
    started_rotating = false; 
  }
}

void Autonomous::AutonomousBlindShooting(int num_shots)
{
  // Get Input Data from Shuffleboard
  double rpm = frc::SmartDashboard::GetNumber("Target Speed", 3550);
  double hood_position = frc::SmartDashboard::GetNumber("Hood Position", 0.3);
  double distance = frc::SmartDashboard::GetNumber("selectedDistance", 120);
  double pValue = frc::SmartDashboard::GetNumber("Turn P Value", 0.002);
  double selectedAngle = (spark_drive->GetGyroscope()->GetYaw() - frc::SmartDashboard::GetNumber("selectedAngle", 0.0));
  
  // Call the Preset Auton Method
  teleoperated->AimingContinuousShoot(rpm, hood_position, pValue, spark_drive->GetGyroscope()->GetYaw(), 0.4);

  // If the number of punches has been reached, cycle through the next state.
  if(manipulator->GetNumPunches() >= num_shots)
  {
    ++current_index;
    manipulator->ResetManipulatorElements();
  }
}

void Autonomous::AutonomousVisionAidedShooting(int num_shots)
{
  // Get Input Data from Shuffleboard
  double distance = frc::SmartDashboard::GetNumber("selectedDistance", 120);
  double pValue = frc::SmartDashboard::GetNumber("Turn P Value", 0.002);
  double selectedAngle = (spark_drive->GetGyroscope()->GetYaw() - frc::SmartDashboard::GetNumber("selectedAngle", 0.0));

  // Call the Preset Auton Method
  teleoperated->AimingContinuousShoot(distance, pValue, selectedAngle, 0.4);

  // If the number of punches has been reached, cycle through the next state.
  if(manipulator->GetNumPunches() >= num_shots)
  {
    ++current_index;
    manipulator->ResetManipulatorElements();
  }
}

void Autonomous::AutonomousDriving(double distance)
{
  if(!started_driving)
  {
    started_driving = true;
    zero_drive_value = spark_drive->GetLeftFrontEncoder().GetPosition();
  }

  current_drive_value = spark_drive->GetLeftFrontEncoder().GetPosition();
  drive_target = zero_drive_value + distance;

  if(fabs(current_drive_value) < fabs(drive_target))
  {
    spark_drive->TankDrive(-0.3, 0.0, false, false);
  } 
  else
  {
    spark_drive->TankDrive(0.3, 0.0, false, false);
  }
  
  if(fabs(current_drive_value - drive_target) < 1)
  {
    ++current_index;
    started_driving = false;
    spark_drive->TankDrive(0.0, 0.0, false, false);
  }
}