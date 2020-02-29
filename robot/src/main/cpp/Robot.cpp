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

void Robot::RobotInit() 
{
  std::cout << "Robot Intializing..." << std::endl;
  enabled_subsystems = 0;

  std::cout << "SmartDashboard Setup..." << std::endl;
  m_chooser.SetDefaultOption(AutoDefault, AutoDefault);
  m_chooser.AddOption(AutoRightRight, AutoRightRight);
  m_chooser.AddOption(AutoRightCenter, AutoRightCenter);
  m_chooser.AddOption(AutoRightLeft, AutoRightLeft);
  frc::SmartDashboard::PutData("Auto Modes", &m_chooser);
  frc::SmartDashboard::PutNumber("Aimpid",0.1);

  // Initialize Timer Object
  timer = new frc::Timer();

  // Initialize Ultra Object for Ultrasonics
  ultra = new Ultra();

  // Initialize Joystick Objects
  joystick_1 = new frc::Joystick(kPrimaryDriverJoystickID);
  joystick_2 = new frc::Joystick(kSecondaryDriverJoystickID);
  
  // Set up Trajectory Tracking System
  std::cout << "Trajectory Tracking Subsystem Setup..." << std::endl;
  std::cout << " ---> ENABLED: " << std::boolalpha << kTrajectoryEnabled << std::endl;
  if(kTrajectoryEnabled)
  {
    ++enabled_subsystems;

    trajectory_generation_utility = new TrajectoryGenerationUtility();
    // Trajectory Test (prints to RioLog)
    ramsete_timed_follower = new RamseteTimedFollower(spark_drive,
    trajectory_generation_utility);
  }

  // Initialize SparkDrive Object using the BigSlink Drivetrain Configuration.
  std::cout << "Drive Subsystem Setup..." << std::endl;
  std::cout << " ---> ENABLED: " << std::boolalpha << kDriveEnabled << std::endl;
  if(kDriveEnabled)
  {
    ++enabled_subsystems;

    spark_drive = new SparkDrive(new rev::CANSparkMax(kBigSlinkLeftFrontMotorID, rev::CANSparkMax::MotorType::kBrushless),
      new rev::CANSparkMax(kBigSlinkRightFrontMotorID, rev::CANSparkMax::MotorType::kBrushless), 
      new rev::CANSparkMax(kBigSlinkLeftBackMotorID, rev::CANSparkMax::MotorType::kBrushless), 
      new rev::CANSparkMax(kBigSlinkRightBackMotorID, rev::CANSparkMax::MotorType::kBrushless));
  }

  // Set up Intake System
  std::cout << "Intake Subsystem Setup..." << std::endl;
  std::cout << " ---> ENABLED: " << std::boolalpha << kIntakeEnabled << std::endl;
  if(kIntakeEnabled)
  {
    ++enabled_subsystems;

    // Intialize Internal Subsystems to Pass into Intake Container Class
    intake_motor = new rev::CANSparkMax(kIntakeMotorID, rev::CANSparkMax::MotorType::kBrushless);
    intake_pin = new frc::Solenoid(kIntakeMotorID);
    
    // Initialize Intake System Container Object using Intake Motor & Intake Holding Solenoid
    intake = new Intake(intake_motor, intake_pin);
  }

  // Set up Shooter System
  std::cout << "Shooter Subsystem Setup..." << std::endl;
  std::cout << " ---> ENABLED: " << std::boolalpha << kShooterEnabled << std::endl;
  if(kShooterEnabled)
  {
    ++enabled_subsystems;

    // Define Internal Subsystems to Pass into Shooter Container Class
    shooter_motor = new rev::CANSparkMax(kFlyWheelMotorID, rev::CANSparkMax::MotorType::kBrushless);
    aim_motor = new rev::CANSparkMax(kAimMotorID, rev::CANSparkMax::MotorType::kBrushless);

    // Initialize Shooter System Container Object using Shooter Motor & Aim (Hood) Motor
    shooter = new Shooter(shooter_motor, aim_motor);
  }

  // Set up Feeder (Geneva Drive) System
  std::cout << "Feeder Subsystem Setup..." << std::endl;
  std::cout << " ---> ENABLED: " << std::boolalpha << kFeederEnabled << std::endl;
  if(kFeederEnabled)
  {
    ++enabled_subsystems;

    // Define Internal Subsystems to Pass into Feeder Container Class
    geneva_motor = new rev::CANSparkMax(kGenevaMotorID, rev::CANSparkMax::MotorType::kBrushless);
    punch = new frc::Solenoid(kPunchSolenoidID);

    // Intialize Feeder System Container Object using Geneva Drive Motor & Punch Solenoid
    feeder = new Feeder(geneva_motor, punch, joystick_1, joystick_2);
  }

  // Initialize Manipulator Container Object using Intake, Feeder, and Shooter Objects
  manipulator = new Manipulator(intake, feeder, shooter);
  // Setup ColorWheel System
  // std::cout << "Color Wheel Subsystem Setup..." << std::endl;
  // std::cout << " ---> ENABLED: " << std::boolalpha << kColorWheelEnabled << std::endl;
  // if(kColorWheelEnabled)
  // {
  //   ++enabled_subsystems;

  //   // Define Internal Subsystems to Pass into ColorWheel Container Class
  //   color_motor = new WPI_TalonSRX(kColorWheelMotorID);
  //   color_sol = new frc::Solenoid(kColorWheelSolenoidID);

  //   // Define ColorWheel Object
  //   color_wheel = new ColorWheel();
  // }

  // Setup Climber System
  std::cout << "Climb Subsystem Setup..." << std::endl;
  std::cout << " ---> ENABLED: " << std::boolalpha << kClimbEnabled << std::endl;
  if(kClimbEnabled)
  {
    ++enabled_subsystems;

    // Define Internal Subsystems to Pass into Climber Container Class
    climb_telescope = new rev::CANSparkMax(kClimbTelescopeMotorID, rev::CANSparkMax::MotorType::kBrushless);
    climb_winch = new rev::CANSparkMax(kClimbWinchMotorID, rev::CANSparkMax::MotorType::kBrushless);

    // Define Climber Object using Telescope & Winch Objects
    climber = new Climber(climb_telescope, climb_winch);
  }

  // Define the Autonomous Container Class using SparkDrive and Robot's Timer Object.
  autonomous = new Autonomous(timer, spark_drive);

  std::cout << "Robot Intialized with " << enabled_subsystems << " Subsystems." << std::endl;
  test = new Diagnostic(joystick_1,
    joystick_2,
    spark_drive,
    intake,
    geneva_motor,
    shooter_motor,
    aim_motor,
    color_motor,
    climber,
    punch
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
  std::cout << "Robot Entering Autonomous Mode..." << std::endl;
  
  m_autoSelected = m_chooser.GetSelected();
  std::cout << "Auto selected: " << m_autoSelected << std::endl;
  // m_autoSelected = frc::SmartDashboard::GetString("Auto Selector",
  //     AutoDefault);

  // Deploy the Intake Mechanism from its "Locked" State.
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
}

void Robot::TeleopInit() 
{
  std::cout << "Robot Entering Teleoperated Mode..." << std::endl;
  spark_drive->GetGyroscope()->ZeroYaw();

  // Deploy the Intake Mechanism from its "Locked" State.
  if(kIntakeEnabled)
  {
    intake->DeployIntake();
  }
}

void Robot::TeleopPeriodic() 
{
  std::cout << "Intake Subsystem Teleoperated Periodic Call" << std::endl;
  if(kIntakeEnabled)
  {
    // X Button for Intake
    if(joystick_2->GetRawButton(kIntakeButton))
    {
      // Set Speed to -3750 RPM (Negative -> Intake)
      intake->SetSpeed(-3750);
    }
    // A Button for Outtake
    else if(joystick_2->GetRawButton(kOuttakeButton))
    {
      // Set Speed to 3750 RPM (Positive -> Outtake)
      intake->SetSpeed(3750);
    }
    else
    {
      // When no Intake/Outtake Buttons Are Pressed, Set Intake Motor to 0 RPM.
      intake->SetSpeed(0);
    }
  }

  std::cout << "Shooter Subsystem Teleoperated Periodic Call" << std::endl;
  if(kShooterEnabled)
  {
    std::cout << "*********************Geneva switch: " << std::boolalpha << feeder->GetGenevaSwitchState() << std::endl;
    // Utility for Adjusting Hood or Aim Motor.
    shooter->SetAdjusterPercentOutput(joystick_2->GetRawAxis(w_axis));

    // B Button for Shoot
    if(joystick_2->GetRawButton(kShootButton))
    {
      // Continually Shoot
      manipulator->ContinuousShoot(0, 0.4);
    }
    else if(joystick_2->GetRawButton(kAdvanceGenevaButton)){
      manipulator->SensorAdvanceGeneva(true);
    }
    else{
      // manipulator->ResetManipulatorElements();
      manipulator->SensorAdvanceGeneva(false);
    }

    // Internal Check for Advancing Geneva without Shooting
    manipulator->SensorAdvanceGeneva(joystick_2->GetRawButton(kAdvanceGenevaButton));
  }

  std::cout << "Drive Subsystem Teleoperated Periodic Call" << std::endl;
  if(kDriveEnabled)
  {
    // Call SparkDrive::TankDrive() using the drivetrain motors
    spark_drive->TankDrive
    (
      joystick_1->GetRawAxis(kForwardBackwardAxis), 
      joystick_1->GetRawAxis(kRotAxis), 
      joystick_1->GetRawButton(kTurboButton), 
      joystick_1->GetRawButton(kTurtleButton),
      0.05
    );
  }

  std::cout << "Climb Subsystem Teleoperated Periodic Call" << std::endl;
  if(kClimbEnabled)
  {
    if(joystick_1->GetRawButton(kExtendClimbButton))
    {
      climber->SetTelescope(0.5);
    }
    else if(joystick_1->GetRawButton(kRetractClimbButton))
    {
      climber->SetTelescope(-0.5);
    }
    else
    {
      climber->SetTelescope(0.0);
    }

    if(joystick_1->GetRawButton(y_button))
    {
      climber->SetWinch(0.2);
    }
    else if(joystick_1->GetRawButton(b_button))
    {
      climber->SetWinch(-0.2);
    }
    else
    {
      climber->SetWinch(0.0);
    }
  }

  std::cout << "Rotate To Angle Feature Teleoperated Periodic Call" << std::endl;
  if(kRotateToAngleEnabled)
  {
    frc::SmartDashboard::PutNumber("Current Angle", spark_drive->GetGyroscope()->GetYaw());
    //Check if vision is actually seeing anything
    if(frc::SmartDashboard::GetNumber("detectionCount", lastCount) == lastCount)
    {
      staleCount++; //A variable to show how "stale" the detectionCount is
    }
    else
    {
      //if vision does see a target, then the count is no longer stale
      staleCount = 0;
    }

    //update the latest count, for use on next loop iteration
    lastCount = frc::SmartDashboard::GetNumber("detectionCount", lastCount);

    //if we are done turning (not currently turning), then update angle from vision
    if(teleop_functions->GetTurnStatus())
    {
      selectedAngle = (spark_drive->GetGyroscope()->GetYaw() + frc::SmartDashboard::GetNumber("selectedAngle", 0.0));
    }

    //Only turn when we hold the button, and we have seen the target recently
    if(joystick_1->GetRawButton(kAutoAimButton) && staleCount < 5)
    {
      teleop_functions->TurnToAngle(selectedAngle, .002);
      staleCount = 0;
      manipulator->PrepareShot(1000, 10);
    }
    else if(!joystick_1->GetRawButtonReleased(kAutoAimButton))
    {
      //when we release the button, then set motors to zero
      //this eliminates the constant turn after turn is done.
      spark_drive->TankDrive(0,0,false,false);
      teleop_functions->SetTurnStatus(true);
    }
  }

  std::cout << "Color Wheel Subsystem Teleoperated Periodic Call" << std::endl;
  if(kColorWheelEnabled)
  {
    //To do: ColorWheel class is taking care of button presses, but we will need
    //To do: We also probably need to pass in the selenoid, we can consider passing all of these
    //into the colorwheel constructor which seems to be the pattern for the other classes
    // color_wheel->RotateToNumber(color_motor, joystick_2);
    // //To do: Get color target from smart dashboard, as this value will be given to us
    // //from field during play
    // frc::Color *targetcolor = new frc::Color(kGreenTarget);
    // color_wheel->RotateToColor(color_motor, joystick_2, targetcolor);

    //Old code left here for reference until we are sure we don't want any of it
    // color_wheel->GetCurrentColor();
    // if(joystick_1->GetRawButton(kDeployColorWheelButton)){
    //   color_wheel->SetExtended(true);
    // }
    // else if(joystick_1->GetRawButton(kRetractColorWheelButton)){
    //   color_wheel->SetExtended(false);
    // }
    // else if(joystick_1->GetRawButton(kColorWheelColorControl)){
    //   color_wheel->TurnToColor(kRedTarget);
    // }
  }
}

void Robot::TestPeriodic() {
  std::cout << "Value is:" << joystick_1->GetRawAxis(1)<< std::endl;
  test->run();
}
#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
