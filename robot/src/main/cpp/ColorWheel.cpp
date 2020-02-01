#include <ColorWheel.h>


ColorWheel::ColorWheel(WPI_TalonSRX *colorMotor)
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
}