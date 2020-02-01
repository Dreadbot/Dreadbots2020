#pragma once
#include "RobotUtilites.h"
#include "rev/ColorSensorV3"
#include "rev/ColorMatch.h"
#include <frc/WPILib.h>
#include "frc/I2C.h"
#include "frc/util/color.h"

class ColorWheel
{
    public:
        ColorWheel(WPI_TalonSRX *colorMotor);
        void TurnToColor(colors targetColor);
    private:  
        frc::Color GetCurrentColor();
        WPI_TalonSRX *m_colorMotor;
        rev::ColorSensorV3 *m_colorSensor;
        rev::ColorMatch *m_colorMatch;

       
     
};