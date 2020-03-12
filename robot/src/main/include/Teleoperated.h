#pragma once

#include <frc/Joystick.h>

#include "Climber.h"
#include "ColorWheel.h"
#include "Manipulator.h"
#include "RobotUtilities.h"
#include "SparkDrive.h"
#include "TeleopFunctions.h"

enum AimShootStates{
  aiming,
  shooting
};

class Teleoperated
{
 public:
  Teleoperated(frc::Joystick* primary_driver_joystick_,
    frc::Joystick* secondary_driver_joystick_,
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

  void AimingContinuousShoot(double distance, 
    double pValue, 
    double target_angle, 
    double geneva_speed);
  void AimingContinuousShoot(double rpm, 
    double hoodPosition, 
    double pValue, 
    double target_angle, 
    double geneva_speed);
  void ResetAimCounts();
 
 private:
  frc::Joystick* primary_driver_joystick;
  frc::Joystick* secondary_driver_joystick;

  Manipulator* manipulator;

  SparkDrive* spark_drive;

  Climber* climber;

  TeleopFunctions* teleop_functions;

  ColorWheel* color_wheel;

  //Combination Aim/Shoot Functions/Variables
  int aim_shoot_state;
  int aim_counts;
  const int max_aim_counts = 150;

  //Rotate to angle variables
  double selectedAngle = 0;
  int lastCount = 0;
  int staleCount = 0;
  double distance = 120;

  //Drive Variables
  double rotSpeed = 0;

  // Color Wheel Robot Objects
  rev::CANSparkMax *color_motor;
  frc::Solenoid *color_sol;
};