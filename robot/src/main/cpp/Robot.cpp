/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"

#include <iostream>

#include <frc/smartdashboard/SmartDashboard.h>


void Robot::RobotInit() {
  motor = new rev::CANSparkMax(1, rev::CANSparkMax::MotorType::kBrushless);
  pidController = new rev::CANPIDController(motor->GetPIDController());

  pidController->SetP(6e-5);
  pidController->SetI(1e-6);
  pidController->SetD(0);
  pidController->SetIZone(0);
  pidController->SetFF(0.000015);
  pidController->SetOutputRange(-1.0, 1.0); 

  frc::SmartDashboard::PutNumber("motorSpeed", motorSpeed);
}


void Robot::RobotPeriodic() {}

void Robot::AutonomousInit() {}

void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}

void Robot::TeleopPeriodic() {
  pidController->SetReference(frc::SmartDashboard::GetNumber("motorSpeed", 0), rev::ControlType::kVelocity);
}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
