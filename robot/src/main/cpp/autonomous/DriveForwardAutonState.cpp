#include "autonomous/DriveForwardAutonState.h"

DriveForwardAutonState::DriveForwardAutonState(
  SparkDrive* spark_drive_, 
  double encoder_determinant_)
    : spark_drive( spark_drive_ ),
      encoder_determinant( encoder_determinant_ )
{}

void DriveForwardAutonState::AutonomousInit()
{
  left_front_encoder_zero_value = spark_drive->GetLeftFrontEncoder().GetPosition();
}

void DriveForwardAutonState::AutonomousPeriodic()
{
  spark_drive->TankDrive(-0.3, 0.0, false, false);
}

void DriveForwardAutonState::CheckState()
{
  double goal = encoder_determinant + left_front_encoder_zero_value;
  double current = encoder_determinant + spark_drive->GetLeftFrontEncoder().GetPosition();

  if(fabs(current - goal) < 1)
  {
    SetDone(true);

    spark_drive->TankDrive(0.0, 0.0, false, false);
  }
}