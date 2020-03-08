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
  int aim_shoot_state = kAiming;
}

void Teleoperated::HandleTeleopInitIntake()
{
  manipulator->GetIntake()->DeployIntake();
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
    joystick_1->GetRawAxis(kRotAxis), 
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

void Teleoperated::HandleShooterInputs()
{
  frc::SmartDashboard::PutNumber("Current Angle", spark_drive->GetGyroscope()->GetYaw());
    int index = manipulator->Round();
    double hood_position = frc::SmartDashboard::GetNumber("Hood Position", 0.5);
    //manipulator->GetSelectedHoodPosition(index);
    int rpm = manipulator->GetSelectedRPM(index);
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
  //update the latest count, for use on next loop iteration
  lastCount = frc::SmartDashboard::GetNumber("detectionCount", lastCount);

  //if we are done turning (not currently turning), then update angle from vision
  if(teleop_functions->GetTurnStatus())
  {
    selectedAngle = (spark_drive->GetGyroscope()->GetYaw() + frc::SmartDashboard::GetNumber("selectedAngle", 0.0));
  }
  //Only turn and shoot when we hold the button, and we have seen the target recently
  if(joystick_2->GetRawButton(kShootButton))
    {
      // Continually Shoot
      std::cout << "Cont Shooting" << std::endl;
      manipulator->ContinuousShoot(frc::SmartDashboard::GetNumber("Hood Position", 0.5), 0.4, frc::SmartDashboard::GetNumber("Target Speed", 0));
    }
    else if(joystick_2->GetRawButton(kAimShootButton) && staleCount < 5){
      std::cout << "Aim Cont Shooting" << std::endl;
      AimingContinuousShoot(hood_position, rpm, pValue, selectedAngle, 0.4);
      staleCount = 0;
    }
    else if(joystick_2->GetRawButton(kAdvanceGenevaButton)){
      manipulator->SensorAdvanceGeneva(true, true);
    }
    else if(joystick_2->GetRawButton(kRegressGenevaButton)){
      manipulator->SensorAdvanceGeneva(true, false);
    }
    else if(manipulator->GetSensorAdvanceGenevaState() == 2){
      std::cout << "Reseting" << std::endl;
      manipulator->ResetManipulatorElements();
      teleop_functions->SetTurnStatus(true);
      aim_counts = 0;
      aim_shoot_state = kAiming;
      manipulator->SensorAdvanceGeneva(false, false);
    }

    if(joystick_2->GetPOV() == 0)
    {
      manipulator->GetShooter()->SetAdjusterPercentOutput(-.4);
    }
    else if(joystick_2->GetPOV() == 180)
    {
      manipulator->GetShooter()->SetAdjusterPercentOutput(.4);
    }
    else
    {
      manipulator->GetShooter()->SetAdjusterPercentOutput(0.0);
    }
    
    //when we release the button, then set motors to zero
    //this eliminates the constant turn after turn is done.
}


void Teleoperated::HandleColorWheelInputs()
{
  //To do: ColorWheel class is taking care of button presses, but we will need
  //To do: We also probably need to pass in the selenoid, we can consider passing all of these
  //into the colorwheel constructor which seems to be the pattern for the other classes
  color_wheel->RotateToNumber();
  //To do: Get color target from smart dashboard, as this value will be given to us
  //from field during play
  frc::Color *targetcolor = new frc::Color(kGreenTarget);
  color_wheel->RotateToColor(targetcolor);

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
void Teleoperated::AimingContinuousShoot(double hood_position, int rpm, double pValue, double target_angle, double geneva_speed){
    //std::cout << "RPM: " << rpm << "Hood Position: " <<hood_position << std::endl;

    if(aim_counts < max_aim_counts)
        aim_shoot_state = kAiming;
    if(aim_counts >= max_aim_counts)
        aim_shoot_state = kShooting;
    
    switch(aim_shoot_state){
        case(kAiming):
            teleop_functions->TurnToAngle(target_angle, pValue);
            manipulator->PrepareShot(rpm, hood_position);
            break;
        case(kShooting):
            manipulator->ContinuousShoot(hood_position, geneva_speed, rpm);
            break;
    }
    aim_counts++;
    std::cout << "Aim counts:" << aim_counts << " Aim State: " << aim_shoot_state << std::endl;
}