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
  m_chooser.SetDefaultOption(kAutoNameDefault, kAutoNameDefault);
  m_chooser.AddOption(kAutoNameCustom, kAutoNameCustom);
  frc::SmartDashboard::PutData("Auto Modes", &m_chooser);
  js1 = new frc::Joystick(0);
  lFront = new rev::CANSparkMax(10, rev::CANSparkMax::MotorType::kBrushless);
  rFront = new rev::CANSparkMax(1, rev::CANSparkMax::MotorType::kBrushless);
  lBack = new rev::CANSparkMax(2, rev::CANSparkMax::MotorType::kBrushless);
  rBack = new rev::CANSparkMax(3, rev::CANSparkMax::MotorType::kBrushless);
  sparkDrive = new SparkDrive(lFront, lBack, rFront, rBack);
  intake = new WPI_TalonSRX(4);
  wrist = new WPI_TalonSRX(5);
  lift = new WPI_TalonSRX(8);
  }

void Robot::RobotPeriodic() {}

void Robot::AutonomousInit() {
  count = 0;
}

void Robot::AutonomousPeriodic() {
  if(count < 50){
    sparkDrive->MecDrive(0, 1, 0, false, false);
  }
  else{
    sparkDrive->MecDrive(0, 0, 0, false, false);
  }
  count++;
}

void Robot::TeleopInit() {}

void Robot::TeleopPeriodic() {
  sparkDrive->MecDrive(js1->GetRawAxis(0), -(js1->GetRawAxis(1)), js1->GetRawAxis(2), js1->GetRawButton(3), false);
  
  if(js1->GetRawButton(1)) //1 - X Button
  {
    //Get Ball
    intake->Set(ControlMode::PercentOutput, 1);
  } 
  else if(js1->GetRawButton(2)) //2 - A button
  {
    //Shoot Ball
    intake->Set(ControlMode::PercentOutput, -1);
  } 
  else 
  {
    //Constant Intake to Keep Ball in Robot's Control.
    intake->Set(ControlMode::PercentOutput, .2);
  }

  if(js1->GetRawButton(6)) //7 - Right Bumper
  {
    //Set Wrist Toward Upright Position
    wrist->Set(ControlMode::PercentOutput, .5);
  } 
  else if(js1->GetRawButton(8)) //5 - Right Trigger
  {
    //Set Wrist Toward Pickup/Shooting Position
    wrist->Set(ControlMode::PercentOutput, -.5);
  } 
  else
  {
    //Set Wrist to 0 by default.
    wrist->Set(ControlMode::PercentOutput, 0);
  }

  if(js1->GetRawButton(5)) //5 - Left Bumper
  {
    //Lifter Up
    lift->Set(ControlMode::PercentOutput, 1);
  } 
  else if(js1->GetRawButton(7)) //7 - Left Trigger
  {
    //Lifter Down
    lift->Set(ControlMode::PercentOutput, -1);
  } 
  else
  {
    //Lifter Set to 0 by Default.
    lift->Set(ControlMode::PercentOutput, 0);
  }
}
void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
