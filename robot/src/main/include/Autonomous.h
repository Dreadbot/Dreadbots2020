#pragma once

#include <map>

#include "autonomous/DriveForwardAutonState.h"

#include "SparkDrive.h"
#include "Intake.h"

class Autonomous
{
 public:
  Autonomous(SparkDrive* spark_drive_, Intake* intake_);

  void AutonomousInit();
  void AutonomousPeriodic();

 private:
  SparkDrive* spark_drive;
  Intake* intake;

  DriveForwardAutonState* drive_auton;
};