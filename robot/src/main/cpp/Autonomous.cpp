#include "Autonomous.h"

Autonomous::Autonomous(TeleopFunctions* teleop_functions_, SparkDrive* spark_drive_, Manipulator* manipulator_)
    : teleop_functions( teleop_functions_ ),
      spark_drive( spark_drive_ ),
      manipulator( manipulator_ )
{
  state_by_index = new std::vector<std::pair<AutonState, int>>();
  current_index = 0;
}

void Autonomous::AutonomousInit()
{
  manipulator->GetIntake()->DeployIntake();
}

void Autonomous::AutonomousPeriodic()
{
  switch(state_by_index->at(current_index).first)
  {
  case shooting:
    break;
  case driving:
    break;
  }
}

// void Autonomous::AutonomousShooting(int num_shots)
// {

// }

// void Autonomous::AutonomousDriving(int distance)
// {

// }