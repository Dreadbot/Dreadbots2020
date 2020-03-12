#include "ColorWheel.h"

ColorWheel::ColorWheel(frc::Joystick *secondary_driver_joystick_)
  : color_wheel_manipulation_motor_controller( 
      new rev::CANSparkMax(kColorWheelRotationControl, 
        rev::CANSparkMax::MotorType::kBrushless) ),
    color_wheel_extension_solenoid( new frc::Solenoid(kColorWheelSolenoidID) ),
    color_sensor( new rev::ColorSensorV3(i2cPort) ),
    color_matcher( new rev::ColorMatch() ),
    secondary_driver_joystick( secondary_driver_joystick_ )
{
  color_matcher->AddColorMatch(kBlueTarget);
  color_matcher->AddColorMatch(kGreenTarget);
  color_matcher->AddColorMatch(kRedTarget);
  color_matcher->AddColorMatch(kYellowTarget);
} 

void ColorWheel::HandleColorWheelExtensionSolenoidInputs()
{
  if (secondary_driver_joystick->GetRawButtonPressed(kDeployColorWheelButton))
  {
    color_wheel_extension_solenoid->Set(true);
  }
  else if (secondary_driver_joystick->GetRawButtonPressed(kRetractColorWheelButton))
  {
    color_wheel_extension_solenoid->Set(false);
  }
}

//Update RotateToNumber to not take in the sensor and get current color from m_colorMatch
void ColorWheel::RotateToNumber()
{
  frc::SmartDashboard::PutNumber("Number Spins", number_of_spins);
  //Put in some smart dashboard output to be helpful for debugging
  frc::SmartDashboard::PutNumber("Spin State", current_spin_state);
  
  if (current_spin_state == not_spinning)
  {
    current_spin_state = init_spinning;
  }
  else if (current_spin_state == init_spinning) 
  {
    number_of_spins = 0;
    color_wheel_manipulation_motor_controller->Set(0.7);
    current_spin_state = spinning;
  }
  else if (current_spin_state == spinning)
  {   
    if (number_of_spins > 7) //colors show up twice, so 7 rotations = 3.5 spins
    {
      color_wheel_manipulation_motor_controller->Set(0.0);
      return;
    }

    //A value betwen 0 and 1, 1 being absolute perfect color match
    current_color_confidence = 0.0;

    current_color_reading = color_sensor->GetColor();
    color_match = color_matcher->MatchClosestColor(current_color_reading, current_color_confidence); 
    
    PrintColor(current_color_reading, current_color_confidence);
    
    //do we need to check confidence number in this condition to see how confident the color matcher 
    //thinks the color is red? A value close to one means more confident. 
    if (color_match == kRedTarget && 
      !is_target_color_red && 
      current_color_confidence >= color_confidence_setting)
    {
      number_of_spins++;
      is_target_color_red = true;
    }
    else if (!(kRedTarget == color_match)) {
      is_target_color_red = false;
    }
  }
}

void ColorWheel::SetRotationState(WheelState rotation_state_)
{
  current_spin_state = rotation_state_;
}

void ColorWheel::RotateToColor(frc::Color* target_color_)
{
  current_color_confidence = 0.0;
  current_color_reading = color_sensor->GetColor();
  color_match = color_matcher->MatchClosestColor(current_color_reading, current_color_confidence);
  
  frc::SmartDashboard::PutNumber("Spin State", current_spin_state);

  if (current_spin_state == not_spinning && 
    secondary_driver_joystick->GetRawButton(kColorWheelColorControl))
  {
    current_spin_state = init_spinning;
    current_button_setting = 2;
  }
  if (current_spin_state == init_spinning && 
    current_button_setting == 2)
  {
    current_spin_state = spinning;
    color_wheel_manipulation_motor_controller->Set(0.2);
  }
  if (current_spin_state == spinning && current_button_setting == 2)
  {
    PrintColor(color_match, current_color_confidence);
    if (color_match == *target_color_ && current_color_confidence >= color_confidence_setting)
    {
      if (number_of_color_samples > 5)
      {
        current_spin_state = not_spinning;
        
        color_wheel_manipulation_motor_controller->Set(0.0);

        number_of_color_samples = 0;
        current_button_setting = 0;
      }
      else {
        number_of_color_samples += 1;
      }
    }
  }
}

void ColorWheel::PrintColor(frc::Color color, 
  double colorConfidence)
{
  if(color == kBlueTarget)
  {
    frc::SmartDashboard::PutString("color", "Blue"); 
    std::cout << "Blue\t" << colorConfidence << std::endl;
  }
  else if(color == kRedTarget)
  {
    frc::SmartDashboard::PutString("color", "Red");
    std::cout << "Red\t" << colorConfidence << std::endl;
  }
  else if(color == kYellowTarget)
  {
    frc::SmartDashboard::PutString("color", "Yellow");
    std::cout << "Yellow\t" << colorConfidence << std::endl;
  }
  else if(color == kGreenTarget)
  {
    frc::SmartDashboard::PutString("color", "Green");
    std::cout << "Green\t" << colorConfidence << std::endl;
  }
  else
  {
    frc::SmartDashboard::PutString("color", "No color detected");
    std::cout << "no color detected" << std::endl;
  }

  frc::SmartDashboard::PutNumber("Confidence", colorConfidence);
}