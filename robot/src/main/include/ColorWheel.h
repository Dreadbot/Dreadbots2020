#pragma once

#include <iostream>
#include <string>

#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/util/color.h>
#include <frc/I2C.h>
#include <frc/Joystick.h>
#include <frc/Solenoid.h>

#include <rev/CANSparkMax.h>
#include <rev/ColorSensorV3.h>
#include <rev/ColorMatch.h>

#include "RobotUtilities.h"

enum WheelState{
  not_spinning,
  init_spinning,
  spinning
};

class ColorWheel
{
 public:
  ColorWheel(frc::Joystick *secondary_driver_joystick_);

  void RotateToNumber();
  void RotateToColor(frc::Color *target_color_);

  void HandleColorWheelExtensionSolenoidInputs();

  void SetRotationState(WheelState rotation_state_);

  void PrintColor(frc::Color color_, 
    double color_confidence_factor_);

 private:
  // Notes after some research Saturday: 
  // We probably want to use the ColorMatch class 
  // to detect color rather than the 
  // ColorSensor. See example code: 
  // https://github.com/REVrobotics/Color-Sensor-v3-Examples/blob/master/C%2B%2B/Color%20Match/src/main/cpp/Robot.cpp
  const frc::I2C::Port i2cPort = frc::I2C::Port::kOnboard;
  const double color_confidence_setting = 0.9;

  rev::CANSparkMax* color_wheel_manipulation_motor_controller;
  frc::Solenoid* color_wheel_extension_solenoid;
  
  rev::ColorSensorV3* color_sensor;
  rev::ColorMatch* color_matcher;
  frc::Joystick* secondary_driver_joystick;

  frc::Color current_color_reading;
  frc::Color color_match;
  int number_of_spins;
  int number_of_color_samples;
  bool matches_target_color;
  bool is_target_color_red;
  double current_color_confidence;

  int current_button_setting;

  WheelState current_spin_state;
};