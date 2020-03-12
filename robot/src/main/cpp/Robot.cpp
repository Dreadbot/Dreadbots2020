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
  m_chooser.AddOption("Straight Shoot", "Straight Shoot");
  m_chooser.AddOption("Move Backwards", "Move Backwards");
  m_chooser.AddOption("Shoot Right Side", "Shoot Right Side");
  frc::SmartDashboard::PutData("Auto Modes", &m_chooser);
  frc::SmartDashboard::PutNumber("Hood Position", 0.3);
  frc::SmartDashboard::PutNumber("Target Speed", 3550);
  frc::SmartDashboard::PutNumber("Shoot P value", .009);
  frc::SmartDashboard::PutNumber("Shoot I value", 0.0000005);
  frc::SmartDashboard::PutNumber("Shoot D value", 0);
  frc::SmartDashboard::PutNumber("Turn Fudge Factor", 0.0);

  // Initialize Timer Object
  timer = new frc::Timer();

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
    intake_pin = new frc::Solenoid(kIntakePinID);
    intake_arms = new frc::Solenoid(kIntakeArmsID);
    // Initialize Intake System Container Object using Intake Motor & Intake Holding Solenoid
    intake = new Intake(intake_motor, intake_pin, intake_arms);
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
    feeder = new Feeder(geneva_motor, punch);
  }

  // Initialize Manipulator Container Object using Intake, Feeder, and Shooter Objects
  manipulator = new Manipulator(intake, feeder, shooter);
  // Setup ColorWheel System
  std::cout << "Color Wheel Subsystem Setup..." << std::endl;
  std::cout << " ---> ENABLED: " << std::boolalpha << kColorWheelEnabled << std::endl;
  if(kColorWheelEnabled)
  {
    ++enabled_subsystems;
    
    // Define ColorWheel Object
    color_wheel = new ColorWheel(joystick_2);
  }

  // Setup Climber System
  std::cout << "Climb Subsystem Setup..." << std::endl;
  std::cout << " ---> ENABLED: " << std::boolalpha << kClimbEnabled << std::endl;
  if(kClimbEnabled)
  {
    ++enabled_subsystems;

    // Define Internal Subsystems to Pass into Climber Container Class
    climb_telescope = new frc::Solenoid(kClimbTelescopeSolenoidID);
    climb_winch = new rev::CANSparkMax(kClimbWinchMotorID, rev::CANSparkMax::MotorType::kBrushless);

    // Define Climber Object using Telescope & Winch Objects
    climber = new Climber(climb_telescope, climb_winch);
  }

  // Define the Autonomous & Teleoperated Container Class using SparkDrive and Robot's Timer Object.
  teleop_functions = new TeleopFunctions(joystick_2, shooter, spark_drive);
  teleoperated = new Teleoperated(joystick_1, 
    joystick_2,
    manipulator,
    spark_drive,
    climber,
    teleop_functions,
    color_wheel);

  autonomous = new Autonomous(teleoperated, teleop_functions, spark_drive, manipulator);

  std::cout << "Robot Intialized with " << enabled_subsystems << " Subsystems." << std::endl;
  frc::SmartDashboard::PutNumber("Min Rot Speed", 0.01);
}

void Robot::RobotPeriodic() {}

void Robot::HoodCalibration(){
    if(shooter->GetLowerLimitSwitch() && !shooter->GetLowerLimitBool())
    { 
      shooter->SetLowerLimit(shooter->GetHoodPosition());
      shooter->SetAdjusterPercentOutput(-0.5);
    }
    else if(shooter->GetUpperLimitSwitch() && !shooter->GetUpperLimitBool())
    {
      shooter->SetUpperLimit(shooter->GetHoodPosition());
      shooter->SetAdjusterPercentOutput(0.5);
    }
    else if (shooter->GetUpperLimitBool() && shooter->GetLowerLimitBool() && !shooter->GetAimReadiness())
    {
      shooter->SetAimReadiness(true);
      shooter->SetAdjusterPosition(0.5);
    }
    if(shooter->GetAimReadiness())
    {
      position = frc::SmartDashboard::GetNumber("Hood Position", 0.5);
    }
}

void Robot::AutonomousInit() {
  std::cout << "Robot Entering Autonomous Mode..." << std::endl;
  
  m_autoSelected = m_chooser.GetSelected();

  std::cout << "Constructing Autonomous Routine..." << std::endl;
  auton_tasklist = new std::vector<std::pair<AutonState, double>>(0);

  std::cout << "Adding Autonomous States..." << std::endl;

  if(!m_autoSelected.compare("Straight Shoot"))
  {
    AddAutonomousLogic(autonomous_blind_shoot_by_number_of_punches, 3);
    AddAutonomousLogic(autonomous_drive_forward_default, -5.0);
  }
  
  if(!m_autoSelected.compare("Move Backwards"))
  {
    AddAutonomousLogic(autonomous_drive_forward_default, -5.0);
  }

  if(!m_autoSelected.compare("Shoot Right Side"))
  {
    AddAutonomousLogic(autonomous_vision_aided_shoot_by_number_of_punches, 3);
    AddAutonomousLogic(autonomous_absolute_rotate, 0.0);
    AddAutonomousLogic(autonomous_drive_forward_default, -5.0);
  }
  
  std::cout << "Starting Autonomous" << std::endl;
  autonomous->AutonomousInit(auton_tasklist);

  std::cout << "Done With Autonomous Init..." << std::endl;
  frc::SmartDashboard::PutNumber("P value", .009);
  frc::SmartDashboard::PutNumber("I value", 0.0000005);
  frc::SmartDashboard::PutNumber("D value", 0);

  shooter_motor->RestoreFactoryDefaults();
  shooter->SetAdjusterPercentOutput(0.75);
  shooter->SetUpperBool(false);
  shooter->SetLowerBool(false);
  shooter->SetAimReadiness(false);
  teleoperated->ResetAimCounts();

  shooter->SetVisionLight(true);
}

void Robot::AutonomousPeriodic() 
{
  HoodCalibration();
  P = frc::SmartDashboard::GetNumber("Shoot P value", 9e-3);
  I = frc::SmartDashboard::GetNumber("Shoot I value", 5e-7);
  D = frc::SmartDashboard::GetNumber("Shoot D value", 0);
  shooter->SetPID(P, I, D);

  autonomous->AutonomousPeriodic();
}

void Robot::TeleopInit() 
{
  shooter_motor->RestoreFactoryDefaults();
  std::cout << "Robot Entering Teleoperated Mode..." << std::endl;
  
  // Zero Yaw on Gyro
  if(kDriveEnabled)
  {
    teleoperated->HandleTeleopInitDrive();
  }

  // Deploy the Intake Mechanism from its "Locked" State.
  if(kIntakeEnabled)
  {
    teleoperated->HandleTeleopInitIntake();
  }

  if(kShooterEnabled)
  {
    shooter->SetAdjusterPercentOutput(0.75);
    shooter->SetUpperBool(false);
    shooter->SetLowerBool(false);
    shooter->SetAimReadiness(false);
  }
}

void Robot::TeleopPeriodic() 
{
  // if(joystick_2->GetPOV(kCameraSwitchPOV)){
  //   frc::SmartDashboard::PutNumber("camNumber", kGenevaCam);
  // }
  // else{
  //   frc::SmartDashboard::PutNumber("camNumber", kDriveCam);
  // }
  if(kIntakeEnabled)
  {
    teleoperated->HandleIntakeInputs();
  }

  if(kShooterEnabled)
  {
    P = frc::SmartDashboard::GetNumber("Shoot P value", 9e-3);
    I = frc::SmartDashboard::GetNumber("Shoot I value", 5e-7);
    D = frc::SmartDashboard::GetNumber("Shoot D value", 0);
    shooter->SetPID(P, I, D);
    HoodCalibration();

    teleoperated->HandleShooterInputs();
  }

  if(kDriveEnabled && teleop_functions->GetTurnStatus())
  {
    teleoperated->HandleDriveInputs();
  }

  if(kClimbEnabled)
  {
    teleoperated->HandleClimbInputs();
  }

  if(kColorWheelEnabled)
  {
    teleoperated->HandleColorWheelInputs();
  }
}

void Robot::TestPeriodic() 
{
  spark_drive->Test(joystick_1);
}

void Robot::AddAutonomousLogic(AutonState auton_state_, double auxiliary_value_)
{
  auton_tasklist->push_back(std::pair<AutonState, double>(auton_state_, auxiliary_value_));
}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif