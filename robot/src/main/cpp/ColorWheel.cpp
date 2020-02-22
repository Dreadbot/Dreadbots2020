#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Joystick.h>
#include <ColorWheel.h>
#include <iostream>
#include <string>
using namespace std;

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

int NumColorSamples = 0;

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
    frc::SmartDashboard::PutNumber("SpinState", spinState);
    
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

        if (joystick->GetRawButtonPressed(1) || NumSpins>7)
        {
            motor->Set(ControlMode::PercentOutput,0.0);
            spinState = WheelState::NotSpinning;
            return;
        }

        //A value betwen 0 and 1, 1 being absolute perfect color match
        double colorConfidence = 0.0;
        frc::Color detectedColor = m_colorSensor.GetColor();
        frc::Color matchedColor = m_colorMatcher.MatchClosestColor(detectedColor, colorConfidence); 
        PrintColor(matchedColor, colorConfidence);
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
    frc::SmartDashboard::PutNumber("SpinState", spinState);
    if (spinState == WheelState::NotSpinning && joystick->GetRawButton(2))
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
        PrintColor(matchedColor, colorConfidence);
        if (matchedColor == *targetcolor && colorConfidence >= ColorConfidenceTarget){
            if (NumColorSamples > 25){
            spinState = WheelState::NotSpinning;
            motor->Set(ControlMode::PercentOutput, 0.0);
            NumColorSamples = 0;
            }
            else {
                NumColorSamples += 1;
            }

        }
    }
   
}

void ColorWheel::PrintColor(frc::Color color, double colorConfidence){
  
        if(color == kBlueTarget){
            frc::SmartDashboard::PutString("color","Blue"); 
            cout << "Blue\t" << colorConfidence << endl;
        }
        else if(color == kRedTarget){
            frc::SmartDashboard::PutString("color","Red");
            cout << "Red\t" << colorConfidence << endl;
        }
        else if(color == kYellowTarget){
            frc::SmartDashboard::PutString("color","Yellow");
            cout << "Yellow\t" << colorConfidence << endl;
        }
        else if(color == kGreenTarget){
            frc::SmartDashboard::PutString("color","Green");
            cout << "Green\t" << colorConfidence << endl;
        }
        else{
            frc::SmartDashboard::PutString("color","No color detected");
            cout << "no color detected" << endl;
        }

    }
