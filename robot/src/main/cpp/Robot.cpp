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

  joystick_1 = new frc::Joystick(kPrimaryDriverJoystickID);
  shooter = new Shooter(3,3);//Should have different numbers if your board supports it during testing
  //printf("robotcpp joystick_addr = %d \n",joystick_1);
  teleopFunctions = new TeleopFunctions(joystick_1, shooter);
  //Button assignments

  // Unused Variable
  // int shooterButton = 1;
  intake = new Intake(); //Uses SparkMax motor 3 

  // Trajectory Test (prints to RioLog)
  trajectory_generation_utility = new TrajectoryGenerationUtility();
  trajectory_generation_utility->GenerateTestTrajectory();

  // Initialize SparkDrive Object using the UltraLord Drivetrain Configuration.
  spark_drive = new SparkDrive(new rev::CANSparkMax(kUltraLeftFrontMotorID, rev::CANSparkMax::MotorType::kBrushless),
    new rev::CANSparkMax(kUltraRightFrontMotorID, rev::CANSparkMax::MotorType::kBrushless), 
    new rev::CANSparkMax(kUltraLeftBackMotorID, rev::CANSparkMax::MotorType::kBrushless), 
    new rev::CANSparkMax(kUltraRightBackMotorID, rev::CANSparkMax::MotorType::kBrushless)
  );
}

/**
 * This function is called every robot packet, no matter the mode. Use
 * this for items like diagnostics that you want ran during disabled,
 * autonomous, teleoperated and test.
 *
 * <p> This runs after the mode specific periodic functions, but before
 * LiveWindow and SmartDashboard integrated updating.
 */
void Robot::RobotPeriodic() {}

/**
 * This autonomous (along with the chooser code above) shows how to select
 * between different autonomous modes using the dashboard. The sendable chooser
 * code works with the Java SmartDashboard. If you prefer the LabVIEW Dashboard,
 * remove all of the chooser code and uncomment the GetString line to get the
 * auto name from the text box below the Gyro.
 *
 * You can add additional auto modes by adding additional comparisons to the
 * if-else structure below with additional strings. If using the SendableChooser
 * make sure to add them to the chooser code above as well.
 */
void Robot::AutonomousInit() {
  m_autoSelected = m_chooser.GetSelected();
  // m_autoSelected = SmartDashboard::GetString("Auto Selector",
  //     kAutoNameDefault);
  std::cout << "Auto selected: " << m_autoSelected << std::endl;

  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
  } else {
    // Default Auto goes here
  }
}

void Robot::AutonomousPeriodic() {
  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
  } else {
    // Default Auto goes here
  }
}

void Robot::TeleopInit() {}

void Robot::TeleopPeriodic() {
 //teleopFunctions->ShooterFunction();

  // need to create sparkdrive above for this code 
 // spark_drive = new SparkDrive(new rev::CANSparkMax(3, rev::CANSparkMax::MotorType::kBrushless)
   
  std::cout << "Teleop Tick" << std::endl;
  
  
    if (joystick_1->GetRawButtonPressed(3)) {
    //intake->Start();
    shooter->AimHeight(10);
  }
    if (joystick_1->GetRawButtonPressed(4)) {
    shooter->AimHeight(0);
    }

    
  //test of intake motor code
  
  if (joystick_1->GetRawButtonPressed(1)) {
    //intake->Start();
    intake->SetSpeed(100);
  }
    if (joystick_1->GetRawButtonPressed(2)) {
    intake->Stop();
  }
  
  // Call SparkDrive::TankDrive() using the drivetrain motors
  spark_drive->TankDrive(
    joystick_1->GetRawAxis(y_axis), 
    joystick_1->GetRawAxis(z_axis), 
    joystick_1->GetRawButton(right_bumper), 
    joystick_1->GetRawButton(left_bumper)
  );
}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
