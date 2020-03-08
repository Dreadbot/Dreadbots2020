#pragma once

#include <rev/ColorSensorV3.h>
#include <rev/ColorMatch.h>
#include <frc/I2C.h>
#include <frc/util/color.h>
#include <ctre/Phoenix.h>
#include <RobotUtilities.h>
#include <frc/Solenoid.h>

class ColorWheel{
    public:
     ColorWheel(WPI_TalonSRX *motor, frc::Joystick *joystick, frc::Solenoid *solenoid);
     void RotateToNumber();
     void RotateToColor(frc::Color *targetcolor);
     void ControlSolenoid();
    private:
     void PrintColor(frc::Color color, double colorConfidence);
     frc::Solenoid *Solenoid;

};

/*class ColorWheel
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
};*/
