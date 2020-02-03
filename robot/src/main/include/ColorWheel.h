#pragma once

#include "rev/ColorSensorV3.h"
#include "rev/ColorMatch.h"
#include "frc/I2C.h"
#include "frc/util/color.h"
#include "ctre/Phoenix.h"
#include "RobotUtilities.h"

class ColorWheel
{
    public:
        ColorWheel(WPI_TalonSRX *colorMotor);
        void TurnToColor(frc::Color targetColor);
    private:  
        frc::Color GetCurrentColor();
        WPI_TalonSRX *m_colorMotor;
        rev::ColorSensorV3 *m_colorSensor;
        rev::ColorMatch *m_colorMatch;
        double confidence;

       
     
};