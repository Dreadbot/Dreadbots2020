#include "Diagnostic.h"

Diagnostic::Diagnostic(frc::Joystick* primary_driver_joystick_)
  : primary_driver_joystick( primary_driver_joystick_ ),
    left_front_motor_controller( 
      new rev::CANSparkMax(kUltraLeftFrontMotorID, 
        rev::CANSparkMax::MotorType::kBrushless)),
    right_front_motor_controller( 
      new rev::CANSparkMax(kUltraRightFrontMotorID, 
        rev::CANSparkMax::MotorType::kBrushless)),
    left_back_motor_controller( 
      new rev::CANSparkMax(kUltraLeftBackMotorID, 
        rev::CANSparkMax::MotorType::kBrushless)),
    right_back_motor_controller( 
      new rev::CANSparkMax(kUltraRightBackMotorID, 
        rev::CANSparkMax::MotorType::kBrushless))
{}

void Diagnostic::RunDiagnostic()
{
  if(primary_driver_joystick->GetRawButton(1)){
    left_front_motor_controller->Set(0.1);
    right_front_motor_controller->Set(0.1);
  }
  else
  {
    left_front_motor_controller->Set(0.0);
    right_front_motor_controller->Set(0.0);
  }

  if(primary_driver_joystick->GetRawButton(2))
  {
    left_back_motor_controller->Set(0.1);
    right_back_motor_controller->Set(0.1);
  }
  else
  {
    left_back_motor_controller->Set(0.0);
    right_back_motor_controller->Set(0.0);
  }
}