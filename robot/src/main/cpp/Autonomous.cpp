#include "Autonomous.h"

Autonomous::Autonomous(TeleopFunctions* teleop_functions_, SparkDrive* spark_drive_, Manipulator* manipulator_)
    : teleop_functions( teleop_functions_ ),
      spark_drive( spark_drive_ ),
      manipulator( manipulator_ )
{
  auton_routine = new std::vector<AutonState>();
  
  //auton_routine->push_back(DriveForwardAutonState(spark_drive, 32.00));
  auton_routine->push_back(RotateToAngleAutonState(teleop_functions, spark_drive->GetGyroscope()->GetAngle() - 30.0));
  auton_routine->push_back(RotateShootAutonState(manipulator));
}

void Autonomous::AutonomousInit()
{
  manipulator->GetIntake()->DeployIntake();
}

void Autonomous::AutonomousPeriodic()
{
  std::cout << "Auton Periodic w/i Auton Class" << std::endl;
  if(current_auton_index >= auton_routine->capacity())
  {
    std::cout << "Skip!!" << std::endl;
    return;
  }

  if(auton_routine->at(current_auton_index).IsDone())
  {
    ++current_auton_index;
    auton_routine->at(current_auton_index).AutonomousInit();
    auton_routine->at(current_auton_index).SetDone(false);
  }

  auton_routine->at(current_auton_index).AutonomousPeriodic();
  auton_routine->at(current_auton_index).CheckState();
}