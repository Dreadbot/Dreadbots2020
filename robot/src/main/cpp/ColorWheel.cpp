#include <frc/Joystick.h>
#include <ColorWheel.h>


enum WheelState{
    NotSpinning,
    InitSpinning,
    Spinning
};
WheelState spinState = WheelState::NotSpinning;

frc::Color ColorState;


int NumSpins = 0;

bool OnRed = false;

ColorWheel::ColorWheel(){
  //Add color match code here
}

void ColorWheel::RotateToNumber(WPI_TalonSRX *motor, frc::Joystick *joystick, rev::ColorSensorV3 *sensor){
    if (spinState == WheelState::NotSpinning && joystick->GetRawButton(1))
    {
        spinState = WheelState::InitSpinning;

    }
    if (spinState == WheelState::InitSpinning) 
    {
        NumSpins = 0;
        motor->Set(ControlMode::PercentOutput,0.2);
        spinState = WheelState::Spinning;
    }
    if (spinState == WheelState::Spinning)
    {   
        ColorState = sensor->GetColor();
        if (joystick->GetRawButton(1) || NumSpins>6)
        {
            motor->Set(ControlMode::PercentOutput,0.0);
            spinState = WheelState::NotSpinning;
            return;
        }
        if (sensor->GetColor()== kRedTarget && OnRed == false){
            NumSpins = NumSpins+1;
            OnRed = true;
        }
        else if (!(sensor->GetColor()== kRedTarget)) {
            OnRed = false;
        }
    }
}

/*ColorWheel::ColorWheel(WPI_TalonSRX *colorMotor)
{
    m_colorMotor = colorMotor;
    m_colorSensor = new rev::ColorSensorV3(frc::I2C::Port::kOnboard);

    m_colorMatch->AddColorMatch(kBlueTarget);
    m_colorMatch->AddColorMatch(kRedTarget);
    m_colorMatch->AddColorMatch(kYellowTarget);
    m_colorMatch->AddColorMatch(kGreenTarget);
}
frc::Color ColorWheel::GetCurrentColor(){
    return m_colorMatch->MatchClosestColor(m_colorSensor->GetColor(), confidence);
}
void ColorWheel::TurnToColor(frc::Color targetColor){
  if(targetColor == GetCurrentColor()){}
  else{
      m_colorMotor->Set(ControlMode::PercentOutput, .1);
  }   
}*/
