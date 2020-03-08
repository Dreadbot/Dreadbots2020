#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Joystick.h>
#include "ColorWheel.h"
#include <iostream>
#include <string>
#include <RobotUtilities.h>
#include <rev/CANSparkMax.h>

using namespace std;

enum WheelState{
    NotSpinning,
    InitSpinning,
    Spinning
};
int spinState = NotSpinning;

Joystick2Layout deployColorWheelButton = Joystick2Layout::kDeployColorWheelButton;

Joystick2Layout RotateToNumberButton = Joystick2Layout::kColorWheelRotationControl;

Joystick2Layout RotateToColorButton = Joystick2Layout::kColorWheelColorControl;

frc::Color CurrentColor;
int NumSpins = 0;

bool OnRed = false;

bool OnTargetColor = false;

double ColorConfidenceTarget = 0.9;

int NumColorSamples = 0;

int CurrentButton = 0;

//Notes after some research Saturday: We probably want to use the ColorMatch class to detect color rather than the 
//ColorSensor. See example code https://github.com/REVrobotics/Color-Sensor-v3-Examples/blob/master/C%2B%2B/Color%20Match/src/main/cpp/Robot.cpp
static constexpr auto i2cPort = frc::I2C::Port::kOnboard;
rev::ColorSensorV3 m_colorSensor(i2cPort);
rev::ColorMatch m_colorMatcher;
rev::CANSparkMax *colormotor;
frc::Joystick *colorjoystick;
frc::Solenoid *colorsolenoid;

ColorWheel::ColorWheel(rev::CANSparkMax *motor, frc::Joystick *joystick, frc::Solenoid *solenoid){
    m_colorMatcher.AddColorMatch(kBlueTarget);
    m_colorMatcher.AddColorMatch(kGreenTarget);
    m_colorMatcher.AddColorMatch(kRedTarget);
    m_colorMatcher.AddColorMatch(kYellowTarget);
    colormotor = motor;
    colorjoystick = joystick;
    colorsolenoid = solenoid;
 } 

void ColorWheel::ControlSolenoid(){
    if (colorjoystick->GetRawButtonPressed(deployColorWheelButton)){
        colorsolenoid->Set(true);
    }
    else if (colorjoystick->GetRawButtonPressed(kRetractColorWheelButton)){
        colorsolenoid->Set(false);
    }

}

//Update RotateToNumber to not take in the sensor and get current color from m_colorMatch
void ColorWheel::RotateToNumber(){
    frc::SmartDashboard::PutNumber("Number Spins", NumSpins);
    //Put in some smart dashboard output to be helpful for debugging
    frc::SmartDashboard::PutNumber("SpinState", spinState);
    
    if (spinState == NotSpinning)
    {
        spinState = InitSpinning;
        //colorsolenoid->Set(true);

    }
    else if (spinState == InitSpinning) 
    {
        NumSpins = 0;
        colormotor->Set(0.7);
        spinState = Spinning;
    }
    else if (spinState == Spinning)
    {   

        if (NumSpins>7) //colors show up twice, so 7 rotations = 3.5 spins
        {
            colormotor->Set(0.0);
            //colorsolenoid->Set(false);
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
            NumSpins++;
            OnRed = true;
        }
        else if (!(matchedColor == kRedTarget)) {
            OnRed = false;
        }
    }
}

void ColorWheel::SetRotationState(int state){
    spinState = state;
}


void ColorWheel::RotateToColor(frc::Color *targetcolor){
    double colorConfidence = 0.0;
    frc::Color detectedColor = m_colorSensor.GetColor();
    frc::Color matchedColor = m_colorMatcher.MatchClosestColor(detectedColor, colorConfidence); 
    frc::SmartDashboard::PutNumber("SpinState", spinState);
    if (spinState == WheelState::NotSpinning && colorjoystick->GetRawButton(RotateToColorButton))
    {
        spinState = WheelState::InitSpinning;
        CurrentButton = 2;
        //colorsolenoid->Set(true);
    }
    if (spinState == WheelState::InitSpinning && CurrentButton == 2)
    {
        spinState = WheelState::Spinning;
        colormotor->Set(0.2);
    }
    if (spinState == WheelState::Spinning && CurrentButton == 2)
    {
        PrintColor(matchedColor, colorConfidence);
        if (matchedColor == *targetcolor && colorConfidence >= ColorConfidenceTarget){
            if (NumColorSamples > 5){
            spinState = WheelState::NotSpinning;
            colormotor->Set(0.0);
            NumColorSamples = 0;
            CurrentButton = 0;
            //colorsolenoid->Set(false);
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
        frc::SmartDashboard::PutNumber("NumSpins", NumSpins);
        frc::SmartDashboard::PutNumber("Confidence", colorConfidence);

    }

