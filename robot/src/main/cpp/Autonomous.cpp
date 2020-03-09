#include "Autonomous.h"

Autonomous::Autonomous(Teleoperated* teleoperated_, TeleopFunctions* teleop_functions_, SparkDrive* spark_drive_, Manipulator* manipulator_)
    : teleoperated( teleoperated_ ),
      teleop_functions( teleop_functions_ ),
      spark_drive( spark_drive_ ),
      manipulator( manipulator_ )
{
  current_index = 0;
}

void Autonomous::AutonomousInit(std::vector<std::pair<AutonState, int>>* state_by_index_)
{
  state_by_index = state_by_index_;

  manipulator->GetIntake()->DeployIntake();

  current_value = 0.0;
}

void Autonomous::AutonomousPeriodic()
{
  std::cout << "index check" << std::endl;
  if(current_index >= state_by_index->capacity())
  {
    return;
  }

  std::cout << "state machine check" << std::endl;
  switch(state_by_index->at(current_index).first)
  {
  case autonomous_shoot_by_number_of_punches:
    AutonomousShooting(state_by_index->at(current_index).second);
    break;
  case autonomous_drive_forward_default:
    AutonomousDriving(state_by_index->at(current_index).second);
    break;
  }

  std::cout << "I am done" << std::endl;
}

void Autonomous::AutonomousShooting(int num_shots)
{
  std::cout << "Getting shuffleboard values" << std::endl;
  // Get Input Data from Shuffleboard
  double hood_position = frc::SmartDashboard::GetNumber("Hood Position", 0.5);
  double distance = frc::SmartDashboard::GetNumber("selectedDistance", 120);
  double pValue = frc::SmartDashboard::GetNumber("Turn P Value", 0.002);
  double selectedAngle = (spark_drive->GetGyroscope()->GetYaw() - frc::SmartDashboard::GetNumber("selectedAngle", 0.0));

  std::cout << "calling teleoperated method" << std::endl;
  // Call the Preset Auton Method
  teleoperated->AimingContinuousShoot(distance, pValue, selectedAngle, 0.4);

  std::cout << "checking punches" << std::endl;
  // If the number of punches has been reached, cycle through the next state.
  if(manipulator->GetNumPunches() >= num_shots)
  {
    ++current_index;
  }

  std::cout << "I am done with this shoot method" << std::endl;
}

void Autonomous::AutonomousDriving(int distance)
{
  if(!started_driving)
  {
    started_driving = true;
    zero_value = spark_drive->GetLeftFrontEncoder().GetPosition();
  }

  current_value = spark_drive->GetLeftFrontEncoder().GetPosition();
  target = zero_value + distance;

  if(current_value < target)
  {
    spark_drive->TankDrive(0.3, 0.0, false, false);
  } 
  else
  {
    spark_drive->TankDrive(-0.3, 0.0, false, false);
  }
  
  if(fabs(current_value - target) < 1)
  {
    ++current_index;
    started_driving = false;
  }
}