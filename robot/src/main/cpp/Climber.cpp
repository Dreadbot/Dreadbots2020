#include "Climber.h"

Climber::Climber(frc::Solenoid *telescope_solenoid_, 
  rev::CANSparkMax *winch_motor_controller_)
  : telescope_solenoid( telescope_solenoid_ ),
    winch_motor_controller( winch_motor_controller_ )
{}

void Climber::SetTelescopeSolenoidExtended(bool is_solenoid_extended_)
{
  telescope_solenoid->Set(is_solenoid_extended_);
}

void Climber::SetWinchMotorControllerPercentOutput(double winch_motor_controller_percent_output_)
{
  winch_motor_controller->Set(winch_motor_controller_percent_output_);
}