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

double ColorConfidenceTarget = 0.9;

//Notes after some research Saturday: We probably want to use the ColorMatch class to detect color rather than the 
//ColorSensor. See example code https://github.com/REVrobotics/Color-Sensor-v3-Examples/blob/master/C%2B%2B/Color%20Match/src/main/cpp/Robot.cpp
static constexpr auto i2cPort = frc::I2C::Port::kOnboard;
rev::ColorSensorV3 m_colorSensor(i2cPort);
rev::ColorMatch m_colorMatcher;

ColorWheel::ColorWheel(){
    m_colorMatcher.AddColorMatch(kBlueTarget);
    m_colorMatcher.AddColorMatch(kGreenTarget);
    m_colorMatcher.AddColorMatch(kRedTarget);
    m_colorMatcher.AddColorMatch(kYellowTarget);
 }

//Update RotateToNumber to not take in the sensor and get current color from m_colorMatch
void ColorWheel::RotateToNumber(WPI_TalonSRX *motor, frc::Joystick *joystick){

    //Put in some smart dashboard output to be helpful for debugging
    
    if (spinState == WheelState::NotSpinning && joystick->GetRawButtonPressed(1))
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

        if (joystick->GetRawButtonPressed(1) || NumSpins>6)
        {
            motor->Set(ControlMode::PercentOutput,0.0);
            spinState = WheelState::NotSpinning;
            return;
        }

        //A value betwen 0 and 1, 1 being absolute perfect color match
        double colorConfidence = 0.0;
        frc::Color detectedColor = m_colorSensor.GetColor();
        frc::Color matchedColor = m_colorMatcher.MatchClosestColor(detectedColor, colorConfidence); 

        //do we need to check confidence number in this condition to see how confident the color matcher 
        //thinks the color is red? A value close to one means more confident. 
        if (matchedColor == kRedTarget && OnRed == false && colorConfidence >= ColorConfidenceTarget){
            NumSpins = NumSpins+1;
            OnRed = true;
        }
        else if (!(matchedColor == kRedTarget)) {
            OnRed = false;
        }
    }
}


void ColorWheel::RotateToColor(WPI_TalonSRX *motor, frc::Joystick *joystick, frc::Color *targetcolor){
    double colorConfidence = 0.0;
    frc::Color detectedColor = m_colorSensor.GetColor();
    frc::Color matchedColor = m_colorMatcher.MatchClosestColor(detectedColor, colorConfidence); 
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
        if (matchedColor == *targetcolor && colorConfidence >= ColorConfidenceTarget){
            spinState = WheelState::NotSpinning;
            motor->Set(ControlMode::PercentOutput, 0.0);
        }
    }
   
}


