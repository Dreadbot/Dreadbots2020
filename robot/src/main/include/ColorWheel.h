#pragma once

#include "rev/ColorSensorV3.h"
#include "rev/ColorMatch.h"
#include "frc/I2C.h"
#include "frc/util/color.h"
#include "ctre/Phoenix.h"
#include "RobotUtilities.h"
#include <frc/Joystick.h>
#include <frc/Solenoid.h>

class ColorWheel
{
    public:
        ColorWheel(WPI_TalonSRX *colorMotor, frc::Solenoid *sol);
        frc::Color GetCurrentColor();
        void TurnToColor(frc::Color targetColor);
        void SetColorMatched(bool color_matched);
        bool GetColorMatched();
        void RotateToNumber();
        void SetRotationsComplete(bool rotations_compelete);
        bool GetRotationsComplete();
        void SetExtended(bool extended);
        bool GetExtended();
    private:  
        
        WPI_TalonSRX *m_colorMotor;
        frc::Solenoid *m_sol;
        rev::ColorSensorV3 *m_colorSensor;
        rev::ColorMatch *m_colorMatch;
        double confidence;
        bool color_matched;

        
        enum WheelState{
            NotSpinning,
            InitSpinning,
            Spinning    
        };
        WheelState spinState;
        frc::Color colorState;
        int NumSpins;
        bool rotations_complete;
};
