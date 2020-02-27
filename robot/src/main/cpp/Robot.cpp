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

void Robot::RobotInit() {
  m_chooser.SetDefaultOption(AutoDefault, AutoDefault);
  m_chooser.AddOption(AutoRightRight, AutoRightRight);
  m_chooser.AddOption(AutoRightCenter, AutoRightCenter);
  m_chooser.AddOption(AutoRightLeft, AutoRightLeft);
  frc::SmartDashboard::PutData("Auto Modes", &m_chooser);
  frc::SmartDashboard::PutNumber("Aimpid",0.1);

  timer = new frc::Timer();

  ultra = new Ultra();

  joystick_1 = new frc::Joystick(kPrimaryDriverJoystickID);
  joystick_2 = new frc::Joystick(kSecondaryDriverJoystickID);
  //test = new Diagnostic(joystick_1);

  if(kTrajectoryEnabled){
    trajectory_generation_utility = new TrajectoryGenerationUtility();
    // Trajectory Test (prints to RioLog)
    ramsete_timed_follower = new RamseteTimedFollower(spark_drive,
    trajectory_generation_utility);
  }

  // Initialize SparkDrive Object using the UltraLord Drivetrain Configuration.
  spark_drive = new SparkDrive(new rev::CANSparkMax(kBigSlinkLeftFrontMotorID, rev::CANSparkMax::MotorType::kBrushless),
    new rev::CANSparkMax(kBigSlinkRightFrontMotorID, rev::CANSparkMax::MotorType::kBrushless), 
    new rev::CANSparkMax(kBigSlinkLeftBackMotorID, rev::CANSparkMax::MotorType::kBrushless), 
    new rev::CANSparkMax(kBigSlinkRightBackMotorID, rev::CANSparkMax::MotorType::kBrushless)
  );



  //teleop_functions = new TeleopFunctions(joystick_1, shooter, spark_drive);

  if(kIntakeEnabled){
    intake_motor = new rev::CANSparkMax(kIntakeMotorID, rev::CANSparkMax::MotorType::kBrushless);
    intake_pin = new frc::Solenoid(kIntakePinID);
    intake = new Intake(intake_motor, intake_pin);
  }
  if(kShooterEnabled){
    shooter_motor = new rev::CANSparkMax(kFlyWheelMotorID, rev::CANSparkMax::MotorType::kBrushless);
    aim_motor = new rev::CANSparkMax(kAimMotorID, rev::CANSparkMax::MotorType::kBrushless);
    shooter = new Shooter(shooter_motor, aim_motor);
  }
  if(kFeederEnabled){
    geneva_motor = new rev::CANSparkMax(kGenevaMotorID, rev::CANSparkMax::MotorType::kBrushless);
    punch = new frc::Solenoid(kPunchSolenoidID);
    feeder = new Feeder(geneva_motor, punch, joystick_1);
  }

  manipulator = new Manipulator(intake, feeder, shooter);

  if(kColorWheelEnabled){
    color_motor = new WPI_TalonSRX(kColorWheelMotorID);
    color_sol = new frc::Solenoid(kColorWheelSolenoidID);
    color_wheel = new ColorWheel(color_motor, color_sol);
  }
  if(kClimbEnabled){
    climb_telescope = new rev::CANSparkMax(kClimbTelescopeMotorID, rev::CANSparkMax::MotorType::kBrushless);
    climb_winch = new rev::CANSparkMax(kClimbWinchMotorID, rev::CANSparkMax::MotorType::kBrushless);
    climber = new Climber(climb_telescope, climb_winch);
  }

  autonomous = new Autonomous(timer, spark_drive);
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
  std::cout << "Auto selected: " << m_autoSelected << std::endl;
  // m_autoSelected = frc::SmartDashboard::GetString("Auto Selector",
  //     AutoDefault);

  if(kIntakeEnabled)
  {
    intake->DeployIntake();
  }
  
}

void Robot::AutonomousPeriodic() 
{
  std::cout << "Auto selected: " << m_autoSelected << std::endl;
  std::cout << "AutoRightRight: " << AutoRightRight << std::endl;

  autonomous->AutonomousPeriodic();

  // iterative_clock += kIterationSecondsRatio;

  // // Get the time at the current iteration in the code
  // units::time::second_t time_at_iteration = units::time::second_t(iterative_clock);

  // // Get the Robot's Current Position according to Odometry
  // auto robot_pose2d = spark_drive->GetRobotPose2dPosition();

  // // Get the current trajectory state, or where the robot
  // // should be.
  // auto current_trajectory_state = trajectory_generation_utility->GetTrajectory().Sample(time_at_iteration);

  // // Calculate Chassis Speeds from the Robot Current State &
  // // Trajectory Current State.
  // auto chassis_speeds = trajectory_generation_utility->GetRamseteController()->Calculate(robot_pose2d, current_trajectory_state);

  // // Set the Chassis Speeds in the Utility Class
  // trajectory_generation_utility->SetChassisSpeeds(chassis_speeds);

  // // Calculate the Final Speeds of the Motors
  // double final_speeds = (double) (trajectory_generation_utility->GetChassisSpeeds().vx);

  // // Set the Motor Speeds in the Velocity Format.
  // spark_drive->GetLeftFrontPIDController().SetReference(final_speeds, rev::ControlType::kVelocity);
  // spark_drive->GetRightFrontPIDController().SetReference(final_speeds, rev::ControlType::kVelocity);
  // spark_drive->GetLeftBackPIDController().SetReference(final_speeds, rev::ControlType::kVelocity);
  // spark_drive->GetRightBackPIDController().SetReference(final_speeds, rev::ControlType::kVelocity);
}

void Robot::TeleopInit() {
  spark_drive->GetGyroscope()->ZeroYaw();

  if(kIntakeEnabled)
  {
    intake->DeployIntake();
  }
}

void Robot::TeleopPeriodic() {
  
  // need to create sparkdrive above for this code 
 // spark_drive = new SparkDrive(new rev::CANSparkMax(3, rev::CANSparkMax::MotorType::kBrushless)
   double joystickaxisY = joystick_2->GetRawAxis(y_axis);
   std::cout << "Joystick Axis: " << joystickaxisY << std::endl;
   if(kIntakeEnabled){
    if(fabs(joystickaxisY)  <= 0.025){
       intake->Stop();
    }
    else{
       intake->SetSpeed(joystickaxisY * 5000 );
       std::cout << "bruh" << std::endl;
    }
   }
  //teleopFunctions->ShooterFunction();
    
  if(kDriveEnabled){
    // Call SparkDrive::TankDrive() using the drivetrain motors
    spark_drive->TankDrive(
      joystick_1->GetRawAxis(kForwardBackwardAxis), 
      joystick_1->GetRawAxis(kRotAxis), 
      joystick_1->GetRawButton(kTurboButton), 
      joystick_1->GetRawButton(kTurtleButton),
      0.05
    );
  }

  // if(kFeederEnabled){
  //   feeder->SensorAdvanceGeneva();
  //   if(joystick_2->GetRawButton(x_button)){
  //     feeder->ExtendRetract(20);
  //   }
  // }

  if(kShooterEnabled){
    // Debug Statement Printing Out Current Manipulator State
    //manipulator->GetState();
    if(joystick_2->GetRawButton(a_button)){
      manipulator->ContinuousShoot(0, 0.4);
    }
    else
    {
      //manipulator->ResetManipulatorElements();
      shooter->SetShootingPercentOutput(-0.5);
      if(feeder->GetSenorAdvanceGenevaState() == 2)
      {
        feeder->SetSpin(0);
      }
    }

    feeder->SensorAdvanceGeneva();
  }

  if(kClimbEnabled){
    if(joystick_1->GetRawButton(kExtendClimbButton)){
      climber->SetTelescope(0.5);
    }
    else if(joystick_1->GetRawButton(kRetractClimbButton)){
      climber->SetTelescope(-0.5);
    }
    else{
      climber->SetTelescope(0.0);
    }

    if(joystick_1->GetRawButton(y_button)){
      climber->SetWinch(0.2);
    }
    else if(joystick_1->GetRawButton(b_button)){
      climber->SetWinch(-0.2);
    }
    else{
      climber->SetWinch(0.0);
    }
  }

  frc::SmartDashboard::PutNumber("Current Angle", spark_drive->GetGyroscope()->GetYaw());
  if(kRotateToAngleEnabled){
    //Check if vision is actually seeing anything
    if(frc::SmartDashboard::GetNumber("detectionCount", lastCount) == lastCount){
      staleCount++; //A variable to show how "stale" the detectionCount is
    }
    else{
      //if vision does see a target, then the count is no longer stale
      staleCount = 0;
    }
    //update the latest count, for use on next loop iteration
    lastCount = frc::SmartDashboard::GetNumber("detectionCount", lastCount);

    //if we are done turning (not currently turning), then update angle from vision
    if(teleop_functions->GetTurnStatus()){
      selectedAngle = (spark_drive->GetGyroscope()->GetYaw() + frc::SmartDashboard::GetNumber("selectedAngle", 0.0));
    }
    //Only turn when we hold the button, and we have seen the target recently
    if(joystick_1->GetRawButton(kAutoAimButton) && staleCount < 5){
      teleop_functions->TurnToAngle(selectedAngle, .002);
      staleCount = 0;
      manipulator->PrepareShot(1000, 10);
    }
    else if(!joystick_1->GetRawButtonReleased(kAutoAimButton)){
      //when we release the button, then set motors to zero
      //this eliminates the constant turn after turn is done.
      spark_drive->TankDrive(0,0,false,false);
      teleop_functions->SetTurnStatus(true);
    }
  }
  if(kColorWheelEnabled){
    color_wheel->GetCurrentColor();
    if(joystick_1->GetRawButton(kDeployColorWheelButton)){
      color_wheel->SetExtended(true);
    }
    else if(joystick_1->GetRawButton(kRetractColorWheelButton)){
      color_wheel->SetExtended(false);
    }
    else if(joystick_1->GetRawButton(kColorWheelColorControl)){
      color_wheel->TurnToColor(kRedTarget);
    }
  }
}

void Robot::TestPeriodic() {
  // test->run();
  spark_drive->Test(joystick_1);
}
#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
