#include "Teleoperated.h"

Teleoperated::Teleoperated(
  frc::Joystick* joystick_1_,
  frc::Joystick* joystick_2_,
  Manipulator* manipulator_,
  SparkDrive* spark_drive_,
  Climber* climber_,
  TeleopFunctions* teleop_functions_,
  ColorWheel* color_wheel_)
    : joystick_1( joystick_1_ ),
      joystick_2( joystick_2_ ),
      manipulator( manipulator_ ),
      spark_drive( spark_drive_ ),
      climber( climber_ ),
      teleop_functions( teleop_functions_ ),
      color_wheel( color_wheel_ )
{
  aim_counts = 0;
  aim_shoot_state = kAiming;
}

void Teleoperated::HandleTeleopInitIntake()
{
  manipulator->GetIntake()->DeployIntake();
  manipulator->GetIntake()->SetIntakeArms(false);
}

void Teleoperated::HandleTeleopInitDrive()
{
  spark_drive->GetGyroscope()->ZeroYaw();
}

void Teleoperated::HandleIntakeInputs()
{
  // X Button for Intake
    if(joystick_2->GetRawButton(kIntakeButton))
    {
      // Set Speed to -3750 RPM (Negative -> Intake)
      manipulator->GetIntake()->SetSpeed(-4000);
    }
    // A Button for Outtake
    else if(joystick_2->GetRawButton(kOuttakeButton))
    {
      // Set Speed to 3750 RPM (Positive -> Outtake)
      manipulator->GetIntake()->SetSpeed(4000);
    }
    else
    {
      // When no Intake/Outtake Buttons Are Pressed, Set Intake Motor to 0 RPM.
      manipulator->GetIntake()->SetPercentOutput(0);
    }

    if(joystick_2->GetRawButton(kDeployColorWheelButton)){
      manipulator->GetIntake()->SetIntakeArms(true);
    }
    else if(joystick_2->GetRawButton(kRetractColorWheelButton)){
      manipulator->GetIntake()->SetIntakeArms(false);
    }
}

// void Teleoperated::HandleShooterInputs()
// {
//   // Utility for Adjusting Hood or Aim Motor.
//     //manipulator->GetShooter()->SetAdjusterPercentOutput(joystick_2->GetRawAxis(w_axis));

//     // B Button for Shoot
//     if(joystick_2->GetRawButton(kShootButton))
//     {
//       manipulator->GetShooter()->SetShootingPercentOutput(-0.8);
//       // Continually Shoot
//       manipulator->ContinuousShoot(0, 0.4, 5000);
//     }
//     else
//     {
//       // Default Shooting PercentOutput to Avoid Ramp-Up Time
//       manipulator->GetShooter()->SetShootingPercentOutput(0);

//       // If The Geneva State is Stoppped, Stop the Spin.
//       if(manipulator->GetSensorAdvanceGenevaState() == 2)
//       {
//         // Set to 0 RPM
//         manipulator->GenevaSetSpin(0);
//       }
//     }

//     // Internal Check for Advancing Geneva without Shooting
//     if(joystick_2->GetRawButton(kAdvanceGenevaButton)){
//       manipulator->SensorAdvanceGeneva(joystick_2->GetRawButton(kAdvanceGenevaButton), true);
//     }
//     else if(joystick_2->GetRawButton(kRegressGenevaButton)){
//       manipulator->SensorAdvanceGeneva(joystick_2->GetRawButton(kRegressGenevaButton), false);
//     }
    
// }

void Teleoperated::HandleDriveInputs()
{
  // Call SparkDrive::TankDrive() using the drivetrain motors
  spark_drive->TankDrive
  (
    joystick_1->GetRawAxis(kForwardBackwardAxis), 
    (joystick_1->GetRawAxis(kRotAxis)-rotSpeed), 
    joystick_1->GetRawButton(kTurboButton), 
    joystick_1->GetRawButton(kTurtleButton),
    0.05
  );
}

void Teleoperated::
HandleClimbInputs()
{
  if(joystick_1->GetRawButton(kExtendClimbButton))
  {
    climber->SetTelescope(true);
  }
  else if(joystick_1->GetRawButton(kRetractClimbButton))
  {
    climber->SetTelescope(false);
  }

  if(joystick_1->GetRawButton(kWinchButton))
  {
    climber->SetWinch(0.4);
  }
  else if(joystick_1->GetRawButton(b_button))
  {
    climber->SetWinch(-0.4);
  }
  else
  {
    climber->SetWinch(0.0);
  }
}

void Teleoperated::HandleShooterInputs()
{
  frc::SmartDashboard::PutNumber("Current Angle", spark_drive->GetGyroscope()->GetYaw());
    double hood_position = frc::SmartDashboard::GetNumber("Hood Position", 0.5);
    //manipulator->GetSelectedHoodPosition(index);
    //int rpm = manipulator->GetSelectedRPM(index);
    double pValue = frc::SmartDashboard::GetNumber("Turn P Value", 0.002);

  //std::cout << "Current Angle: " << spark_drive->GetGyroscope()->GetYaw() << std::endl;
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

  if(aim_shoot_state == kAiming){
    distance = frc::SmartDashboard::GetNumber("selectedDistance", 120);
  }
  //update the latest count, for use on next loop iteration
  lastCount = frc::SmartDashboard::GetNumber("detectionCount", lastCount);

  //if we are done turning (not currently turning), then update angle from vision
  if(teleop_functions->GetTurnStatus())
  {
    selectedAngle = (spark_drive->GetGyroscope()->GetYaw() - frc::SmartDashboard::GetNumber("selectedAngle", 0.0));//Flipped sign because NavX and vision are opposite signs
  }
  //Only turn and shoot when we hold the button, and we have seen the target recently

  if(joystick_2->GetRawButton(kShootButton))
  {
    // Continually Shoot
    double shooting_hood_position  = frc::SmartDashboard::GetNumber("Hood Position", 0.5);
    std::cout << "Cont Shooting" << std::endl;
    manipulator->ContinuousShoot(shooting_hood_position, 0.4, frc::SmartDashboard::GetNumber("Target Speed", 0));
    frc::SmartDashboard::PutNumber("camNumber", 0);
  }
  else if(joystick_2->GetRawButton(kAimShootButton) && staleCount < 5){
    AimingContinuousShoot(distance, pValue, selectedAngle, 0.4);
    frc::SmartDashboard::PutNumber("camNumber", 0);
    staleCount = 0;
  }
  else if(joystick_2->GetRawButton(kAimShootButton)){
    manipulator->GetShooter()->SetVisionLight(true);
    frc::SmartDashboard::PutNumber("camNumber", 0);
  }
  else if(joystick_2->GetRawButton(kAdvanceGenevaButton)){
    manipulator->SensorAdvanceGeneva(true, true);
  }
  else if(joystick_2->GetRawButton(kRegressGenevaButton)){
    manipulator->SensorAdvanceGeneva(true, false);
  }
  else if(manipulator->GetSensorAdvanceGenevaState() == 2){
    //std::cout << "Reseting" << std::endl;
    manipulator->ResetManipulatorElements();
    teleop_functions->SetTurnStatus(true);
    aim_counts = 0;
    aim_shoot_state = kAiming;
    manipulator->SensorAdvanceGeneva(false, false);
    rotSpeed = 0;
  }
  else
  {
    frc::SmartDashboard::PutNumber("camNumber", 1);
  }
  
  //when we release the button, then set motors to zero
  //this eliminates the constant turn after turn is done.
}


void Teleoperated::HandleColorWheelInputs()
{
  //To do: ColorWheel class is taking care of button presses, but we will need
  //To do: We also probably need to pass in the selenoid, we can consider passing all of these
  //into the colorwheel constructor which seems to be the pattern for the other classes
  if(joystick_2->GetRawButton(kColorWheelRotationControl)){
    color_wheel->RotateToNumber();
  }
  else{
    color_wheel->SetRotationState(0);
  }
  //To do: Get color target from smart dashboard, as this value will be given to us
  //from field during play
  frc::Color *targetcolor = new frc::Color(kGreenTarget);
  if(joystick_2->GetRawButton(kColorWheelColorControl)){
    color_wheel->RotateToColor(targetcolor);
  }
  

  color_wheel->ControlSolenoid();

  //Old code left here for reference until we are sure we don't want any of it
  //color_wheel->GetCurrentColor();
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
void Teleoperated::AimingContinuousShoot(double distance, double pValue, double target_angle, double geneva_speed){
    //std::cout << "RPM: " << rpm << "Hood Position: " <<hood_position << std::endl;
    double rpm = manipulator->GetSelectedRPM(distance);
    double hoodPosition = manipulator->GetSelectedHoodPosition(distance);
    //std::cout << "rpm: " << rpm << " HoodPostion: " << hoodPosition << std::endl;
    if(aim_counts < max_aim_counts){
        aim_shoot_state = kAiming;
    }
    if(aim_counts >= max_aim_counts){
        aim_shoot_state = kShooting;
    }
    switch(aim_shoot_state){
        case(kAiming):
            rotSpeed = teleop_functions->CalculateTurnToAngle(target_angle);
            manipulator->PrepareShot(rpm, hoodPosition);
            break;
        case(kShooting):
            spark_drive->TankDrive(0,0,false,false);
            manipulator->ContinuousShoot(hoodPosition, geneva_speed, rpm);
            break;
    }
    aim_counts++;
    //std::cout << "Aim counts:" << aim_counts << " Aim State: " << aim_shoot_state << std::endl;
}

void Teleoperated::AimingContinuousShoot(double rpm, double hoodPosition, double pValue, double target_angle, double geneva_speed){
    //std::cout << "RPM: " << rpm << "Hood Position: " <<hood_position << std::endl;
    frc::SmartDashboard::PutNumber("aim counts", aim_counts);
    // frc::SmartDashboard::PutNumber("Rotpermin", rpm);
    // frc::SmartDashboard::PutNumber("hoodpos", hoodPosition);

    if(aim_counts < max_aim_counts){
        aim_shoot_state = kAiming;
    }
    if(aim_counts >= max_aim_counts){
        aim_shoot_state = kShooting;
    }
    switch(aim_shoot_state){
        case(kAiming):
            teleop_functions->WPITurnToAngle(target_angle);
            manipulator->PrepareShot(rpm, hoodPosition);
            break;
        case(kShooting):
            spark_drive->TankDrive(0,0,false,false);
            manipulator->ContinuousShoot(hoodPosition, geneva_speed, rpm);
            break;
    }
    aim_counts++;
    //std::cout << "Aim counts:" << aim_counts << " Aim State: " << aim_shoot_state << std::endl;
}

void Teleoperated::ResetAimCounts(){
  aim_counts = 0;
}