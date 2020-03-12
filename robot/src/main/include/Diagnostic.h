#pragma once

#include <frc/Joystick.h>

#include <rev/CANSparkMax.h>

#include "RobotUtilities.h"

class Diagnostic
{
 public:
  Diagnostic(frc::Joystick* primary_driver_joystick_);

  void RunDiagnostic();

 private:
  frc::Joystick* primary_driver_joystick;

  rev::CANSparkMax* left_front_motor_controller;
  rev::CANSparkMax* right_front_motor_controller;
  rev::CANSparkMax* left_back_motor_controller;
  rev::CANSparkMax* right_back_motor_controller;
};