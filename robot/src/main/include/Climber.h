#pragma once

#include <frc/Solenoid.h>

#include <rev/SparkMax.h>
#include <rev/CANSparkMax.h>

class Climber
{
 public:
  Climber(frc::Solenoid *telescope_solenoid_, 
    rev::CANSparkMax *winch_motor_controller_);

  void SetTelescopeSolenoidExtended(bool is_solenoid_extended_);
  void SetWinchMotorControllerPercentOutput(double winch_motor_controller_percent_output_);

 private:
  frc::Solenoid* telescope_solenoid;

  rev::CANSparkMax* winch_motor_controller;
};