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

frc::Color PreviousColor;

int NumSpins = 0;

bool OnRed = false;

double ColorConfidenceTarget = 0.9;

int NumColorSamples = 0;

int CurrentButton = 0;



static constexpr auto i2cPort = frc::I2C::Port::kOnboard;
rev::ColorSensorV3 m_colorSensor(i2cPort);
rev::ColorMatch m_colorMatcher;
WPI_TalonSRX *colormotor;
frc::Joystick *colorjoystick;
frc::Solenoid *colorsolenoid;

ColorWheel::ColorWheel(WPI_TalonSRX *motor, frc::Joystick *joystick, frc::Solenoid *solenoid){
    m_colorMatcher.AddColorMatch(kBlueTarget);
    m_colorMatcher.AddColorMatch(kGreenTarget);
    m_colorMatcher.AddColorMatch(kRedTarget);
    m_colorMatcher.AddColorMatch(kYellowTarget);
    colormotor = motor;
    colorjoystick = joystick;
    colorsolenoid = solenoid;
 } 

frc::Color ColorWheel::getPreviousColor(frc::Color color){
    //Color order: Red, Yellow, Blue, Green
    if (color == kGreenTarget)
    {
        return kBlueTarget;
    }
    else if (color == kBlueTarget)
    {
        return kYellowTarget;
    }
    else if (color == kYellowTarget)
    {
        return kRedTarget;
    }
    else if (color == kRedTarget)
    {
        return kGreenTarget;
    }
}
frc::Color ColorWheel::getNextColor(frc::Color color){
    if (color == kGreenTarget)
    {
        return kRedTarget;
    }
    else if (color == kBlueTarget)
    {
        return kGreenTarget;
    }
    else if (color == kYellowTarget)
    {
        return kBlueTarget;
    }
    else if (color == kRedTarget)
    {
        return kYellowTarget;
    }
}
frc::Color ColorWheel::getSpinTargetColor(frc::Color color){
    if (color == kGreenTarget)
    {
        return kYellowTarget;
    }
    else if (color == kBlueTarget)
    {
        return kRedTarget;
    }
    else if (color == kYellowTarget)
    {
        return kGreenTarget;
    }
    else if (color == kRedTarget)
    {
        return kBlueTarget;
    }

}
void ColorWheel::ControlSolenoid(){
    if (colorjoystick->GetRawButtonPressed(3)){
        bool isup = colorsolenoid->Get();

        if(isup == true){
            colorsolenoid->Set(false);
        }
        else{
            colorsolenoid->Set(true);
        }
    }

}


void ColorWheel::RotateToNumber(){
    if (spinState == WheelState::NotSpinning && colorjoystick->GetRawButtonPressed(1))
    {
        spinState = WheelState::InitSpinning;
        CurrentButton = 1;

    }
    if (spinState == WheelState::InitSpinning && CurrentButton == 1) 
    {
        NumSpins = 0;
        colormotor->Set(ControlMode::PercentOutput,0.7);
        spinState = WheelState::Spinning;
    }
    if (spinState == WheelState::Spinning && CurrentButton == 1)
    {   

        if (colorjoystick->GetRawButtonPressed(1) || NumSpins>7)
        {
            colormotor->Set(ControlMode::PercentOutput,0.0);
            spinState = WheelState::NotSpinning;
            CurrentButton = 0;
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


void ColorWheel::RotateToColor(frc::Color *targetcolor){
    double colorConfidence = 0.0;
    frc::Color spinTargetColor = getSpinTargetColor(*targetcolor);
    frc::Color detectedColor = m_colorSensor.GetColor();
    frc::Color matchedColor = m_colorMatcher.MatchClosestColor(detectedColor, colorConfidence); 
    frc::SmartDashboard::PutNumber("SpinState", spinState);
    if (spinState == WheelState::NotSpinning && colorjoystick->GetRawButton(2))
    {
        spinState = WheelState::InitSpinning;
        CurrentButton = 2;
    }
    if (spinState == WheelState::InitSpinning && CurrentButton == 2)
    {
        spinState = WheelState::Spinning;
        colormotor->Set(ControlMode::PercentOutput, 0.2);
        //*********
        // check to make sure we're getting a unique copy of matched color 
        //*********
        PreviousColor = getPreviousColor(matchedColor);
        CurrentColor = matchedColor;
    }
    if (spinState == WheelState::Spinning && CurrentButton == 2)
    {
        PrintColor(matchedColor, colorConfidence);
        if (!(matchedColor == CurrentColor))
        {
            if (matchedColor == getNextColor(PreviousColor))
            {
                if (matchedColor == spinTargetColor && colorConfidence >= ColorConfidenceTarget)
                {
                    if (NumColorSamples > 1)
                    {
                        spinState = WheelState::NotSpinning;
                        colormotor->Set(ControlMode::PercentOutput, 0.0);
                        NumColorSamples = 0;
                        CurrentButton = 0;
                    }
                    else 
                    {
                        NumColorSamples += 1;
                    }
                }
                else 
                {
                    PreviousColor = getPreviousColor(matchedColor);
                    CurrentColor = matchedColor;
                }
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
        frc::SmartDashboard::PutNumber("SpinState", spinState);

    }

