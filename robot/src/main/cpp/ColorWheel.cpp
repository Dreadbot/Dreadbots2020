#include <frc/Joystick.h>
#include <ColorWheel.h>


enum WheelState{
    NotSpinning,
    InitSpinning,
    Spinning
};
WheelState spinState = WheelState::NotSpinning;

frc::Color CurrentColor;


int NumSpins = 0;

bool OnRed = false;

bool OnTargetColor = false;

//Notes after some research Saturday: We probably want to use the ColorMatch class to detect color rather than the 
//ColorSensor. See example code https://github.com/REVrobotics/Color-Sensor-v3-Examples/blob/master/C%2B%2B/Color%20Match/src/main/cpp/Robot.cpp
static constexpr auto i2cPort = frc::I2C::Port::kOnboard;
rev::ColorSensorV3 m_sensor(i2cPort);
rev::ColorMatch m_colorMatch;

ColorWheel::ColorWheel(){
    
  
}

//Update RotateToNumber to not take in the sensor and get current color from m_colorMatch
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


void ColorWheel::RotateToColor(WPI_TalonSRX *motor, frc::Joystick *joystick, rev::ColorSensorV3 *sensor, frc::Color *targetcolor){
    CurrentColor = sensor->GetColor();
    if (spinState == WheelState::NotSpinning && joystick->GetRawButton(1))
    {
        spinState = WheelState::InitSpinning;
    }
    if (spinState == WheelState::InitSpinning)
    {
        spinState = WheelState::Spinning;
        motor->Set(ControlMode::PercentOutput, 0.2);
    }
    if (spinState == WheelState::Spinning)
    {
      //  if (!(sensor->GetColor() == targetcolor)){

        //}
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
