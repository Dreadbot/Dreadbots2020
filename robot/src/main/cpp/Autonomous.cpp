#include "Autonomous.h"

Autonomous::Autonomous(SparkDrive* spark_drive_, Intake* intake_)
    : spark_drive( spark_drive_ ),
      intake( intake_ )
{
  drive_auton = new DriveForwardAutonState(spark_drive, 64.00);
}

void Autonomous::AutonomousInit()
{
  intake->DeployIntake();

  drive_auton->AutonomousInit();
  drive_auton->SetDone(false);
}

void Autonomous::AutonomousPeriodic()
{
  std::cout << "Is done?" << std::boolalpha << drive_auton->IsDone() << std::endl;
  if(drive_auton->IsDone())
    return;

  drive_auton->AutonomousPeriodic();
  drive_auton->CheckState();
}