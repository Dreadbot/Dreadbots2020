#include <ColorWheel.h>


ColorWheel::ColorWheel(WPI_TalonSRX *colorMotor):
{
    m_colorMotor = colorMotor;
    rev::m_colorSensor = new ColorSensorV3(frc::I2C::Port::kOnboard);
    rev::ColorMatch m_colorMatch = new rev::ColorMatch;

    m_colorMatch.addColorToMatch(kBlueTarget);
    m_colorMatch.addColorToMatch(kRedTarget);
    m_colorMatch.addColorToMatch(kYellowTarget);
    m_colorMatch.addColorToMath(kGreenTarget);
}
frc::Color ColorWheel::GetCurrentColor(){
    return m_colorMatch.MatchClosestColor(m_colorSensor.GetColor(), .95);
}
void ColorWheel::TurnToColor(frc::Color target){
    if(GetCurrentColor != target){
        m_colorMotor->Set(ControlMode::PercentOutput, .1);
    }
}