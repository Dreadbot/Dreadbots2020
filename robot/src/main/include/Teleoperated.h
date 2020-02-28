#pragma once

#include <frc/Joystick.h>

#include "Climber.h"
#include "ColorWheel.h"
#include "Manipulator.h"
#include "RobotUtilities.h"
#include "SparkDrive.h"
#include "TeleopFunctions.h"

class Teleoperated
{
 public:
  Teleoperated(
    frc::Joystick* joystick_1_,
    frc::Joystick* joystick_2_,
    Manipulator* manipulator_,
    SparkDrive* spark_drive_,
    Climber* climber_,
    TeleopFunctions* teleop_functions_,
    ColorWheel* color_wheel_);

  void HandleTeleopInitIntake();
  void HandleTeleopInitDrive();

  void HandleIntakeInputs();
  void HandleFeederInputs();
  void HandleShooterInputs();
  void HandleDriveInputs();
  void HandleClimbInputs();
  void HandleRotateToAngleInputs();
  void HandleColorWheelInputs();
 
 private:
  frc::Joystick* joystick_1;
  frc::Joystick* joystick_2;

  Manipulator* manipulator;

  SparkDrive* spark_drive;

  Climber* climber;

  TeleopFunctions* teleop_functions;

  ColorWheel* color_wheel;

  //Rotate to angle variables
  double selectedAngle = 0;
  int lastCount = 0;
  int staleCount = 0;

  // Color Wheel Robot Objects
  WPI_TalonSRX *color_motor;
  frc::Solenoid *color_sol;
};