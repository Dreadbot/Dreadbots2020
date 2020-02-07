/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/
/*                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
                          .,coddl:;'.                                                  .';lc.       
                     .,:oxOKKK0KKKK0Oxol:,..                                      ..;cdkO00k,       
                .':lxO0KK000KKK0000000KKKK0kxoc:,...                         ..,coxdlc:cdO0O;       
           .':ldO0KK000K0KKKKKKKKKKKKKK00KK00KKK0OOkdlc;'.               .,:oxO00k:.    .:O0:       
        .;cdkO000KKK00KKKKK00KKKKKKKKKKKKKKKKK0000000KKK0Oxol:,..   .,:lxO000000O:    .:..dKl       
        c0OkkxxkkkkOO000KKKKK0KKKKKKKKKK000KKKKK0KKK00KK0000KK00kdodO00000000000O;    ;x;,xKo.      
        lKKKK000OOkkxxxkkkO000KKK0000KKKK000KKKKKKKK00KK0000000000000000000000000d,.   .;x00d.      
        lK0KKKKK0KKK000OkkkxxxkkOO000KKK0KKKKKKK0KKK000000kddxO0000000000000000000Odllox0000x.      
        lK00KKKK0KK00KKKKKK00OOkkxxxxkkOO000KK00K00kk000x:....;x0000000000000000000000000000k'      
        lK0KKKKKK00KKKKK0KKK00000000OkkkxxxkkkOO00Oxx00x'  .',.,k000000000000000000000000000O;      
        lK00OOO0KK0KKKKKK000KK00KKKKKKKK000Okkkxxxxox00l.  .cd',k0000000000000000000000000000:      
       .cxdolllodk0K0KKKK00000KKKKKKKK0K00K000000OOxxO0x'    .'d00000000000000000000000000000c      
      .:lllllllcccdO0KKKKK0000KK0KK00KK00KK00OxkOO0xdO00kl;;:lk000000000000000000000000000000o.     
     .codO00kdllcccoO00kxddoodxk0KK0KKKK000000Okkkkkk0000000000000000000000000000000000000000d.     
     ,ldKNNNNKxllcccoxollllllcccok0K00KKK00000000000O0000000000000000000000000000000000000000x.     
     'ldKNNNNN0olcccclodkOkdolccclx00Okxdooodxk00KKOxO000000000000000000000000000000000000000k'     
     .:oOXNNNN0dllccllxKNNNXOolccclxxolllllccccdOKKOdk00000000000000000000000000000000000Oxl:'      
      .cox0KK0xllcccllkNNNNNXkolccclloxkkdollccclkKOxk000000000000000000000000000000Okoc,.          
       .,coooollc;'.,lxKNNNNNOolcccloOXNNX0dllcccldkxx00000000000000000000000000Oxl;'.              
         ..,,,'..    ,ldOKXX0dllcclldKNNNNN0dlccccldxx000000000000000000000Okoc,.                   
                      'codddollc:,,coONNNNNKxllcccc::d00000000000000000Odl;'.                       
                        .,;;;,'.   .cdOXNNXOollcc:;..o000000000000Okoc,.                            
                                    .:ldxkdollc:,.  .o00000000Odl;'.                                
                                      .;:cc:,..     .l000Oxo:,.                                     
                                                     ;ol;'.                                         
*/

#include "Robot.h"

#include <iostream>

#include <frc/smartdashboard/SmartDashboard.h>

void Robot::RobotInit() {
  m_chooser.SetDefaultOption(kAutoNameDefault, kAutoNameDefault);
  m_chooser.AddOption(kAutoNameCustom, kAutoNameCustom);
  frc::SmartDashboard::PutData("Auto Modes", &m_chooser);
  frc::SmartDashboard::PutNumber("Aimpid",0.1);

  joystick_1 = new frc::Joystick(kPrimaryDriverJoystickID);
  shooter = new Shooter(3,3);//Should have different numbers if your board supports it during testing
  //printf("robotcpp joystick_addr = %d \n",joystick_1);
  //Button assignments

  intake = new Intake(); //Uses SparkMax motor 3 

  // Trajectory Test (prints to RioLog)

  // Initialize SparkDrive Object using the UltraLord Drivetrain Configuration.
  spark_drive = new SparkDrive(new rev::CANSparkMax(kUltraLeftFrontMotorID, rev::CANSparkMax::MotorType::kBrushless),
    new rev::CANSparkMax(kUltraRightFrontMotorID, rev::CANSparkMax::MotorType::kBrushless), 
    new rev::CANSparkMax(kUltraLeftBackMotorID, rev::CANSparkMax::MotorType::kBrushless), 
    new rev::CANSparkMax(kUltraRightBackMotorID, rev::CANSparkMax::MotorType::kBrushless)
  );

  trajectory_generation_utility = new TrajectoryGenerationUtility();
  trajectory_generation_utility->GenerateTestTrajectory(spark_drive);
  teleop_functions = new TeleopFunctions(joystick_1, shooter, spark_drive);
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

  iterative_clock += kIterationSecondsRatio;

  units::time::second_t time_at_iteration = units::time::second_t(iterative_clock);

  trajectory_generation_utility->SetChassisSpeeds(
    trajectory_generation_utility->GetRamseteController()->Calculate(
      spark_drive->GetRobotPose2dPosition(), 
      trajectory_generation_utility->GetTrajectory().Sample(time_at_iteration)
    )
  );

  spark_drive->GetLeftFrontPIDController().SetReference((double) (trajectory_generation_utility->GetChassisSpeeds().vx), rev::ControlType::kVelocity);
  spark_drive->GetRightFrontPIDController().SetReference((double) (trajectory_generation_utility->GetChassisSpeeds().vx), rev::ControlType::kVelocity);
  spark_drive->GetLeftBackPIDController().SetReference((double) (trajectory_generation_utility->GetChassisSpeeds().vx), rev::ControlType::kVelocity);
  spark_drive->GetRightBackPIDController().SetReference((double) (trajectory_generation_utility->GetChassisSpeeds().vx), rev::ControlType::kVelocity);
}

void Robot::TeleopInit() {
  spark_drive->gyro->ZeroYaw();
}

void Robot::TeleopPeriodic() {
  //teleopFunctions->ShooterFunction();
  std::cout << "Teleop Tick" << std::endl;
  
  if (joystick_1->GetRawButtonPressed(b_button)) {
    //intake->Start();
    shooter->AimHeight(10);
  }
  if (joystick_1->GetRawButtonPressed(y_button)) {
    shooter->AimHeight(0);
  }

    
  //Testing Intake Motor Code
  if (joystick_1->GetRawButtonPressed(x_button)) {
    //intake->Start();
    intake->SetSpeed(100);
  }
  if (joystick_1->GetRawButtonPressed(a_button)) {
    intake->Stop();
  }
  
  // Call SparkDrive::TankDrive() using the drivetrain motors
  spark_drive->TankDrive(
    -joystick_1->GetRawAxis(y_axis), 
    joystick_1->GetRawAxis(z_axis), 
    joystick_1->GetRawButton(right_bumper), 
    joystick_1->GetRawButton(left_bumper)
  );

  if(!teleop_functions->GetTurnStatus() || joystick_1->GetRawButton(a_button)){
    teleop_functions->TurnToAngle(30.0, .002);
  }
  frc::SmartDashboard::PutNumber("Current Angle", spark_drive->gyro->GetYaw());
}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
