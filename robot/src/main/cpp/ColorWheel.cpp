#include <ColorWheel.h>

ColorWheel::ColorWheel(WPI_TalonSRX *colorMotor, frc::Solenoid *sol)
{
    spinState = WheelState::NotSpinning;
    NumSpins = 0;

    m_colorMotor = colorMotor;
    m_colorMotor->SetNeutralMode(NeutralMode::Brake);
    m_sol = sol;
    m_colorSensor = new rev::ColorSensorV3(frc::I2C::Port::kOnboard);
    m_colorMatch = new rev::ColorMatch();

    m_colorMatch->AddColorMatch(kBlueTarget);
    m_colorMatch->AddColorMatch(kRedTarget);
    m_colorMatch->AddColorMatch(kYellowTarget);
    m_colorMatch->AddColorMatch(kGreenTarget);
}

frc::Color ColorWheel::GetCurrentColor(){
    return m_colorMatch->MatchClosestColor(m_colorSensor->GetColor(), confidence);
}

void ColorWheel::TurnToColor(frc::Color targetColor){
  if(targetColor == GetCurrentColor()){
      m_colorMotor->Set(ControlMode::PercentOutput, 0);
      SetColorMatched(true);
  }
  else{
      m_colorMotor->Set(ControlMode::PercentOutput, .5);
      std::cout << "Driving the color motor" << std::endl;
  }
} 

void ColorWheel::SetColorMatched(bool color_matched){
    this->color_matched = color_matched;
}

bool ColorWheel::GetColorMatched(){
    return color_matched;
}

void ColorWheel::RotateToNumber(){
    //Call this only when GetRotationsComplete() returns false and a button is held, 
    //SetRotationsComplete() may be called after a waiting period to allow this to be done more than once per match
    if (spinState == NotSpinning && !rotations_complete)
    {
        spinState = InitSpinning;

    }
    if (spinState == Spinning && rotations_complete)
    {
            spinState = NotSpinning;
    }
    if(NumSpins > 6)
        rotations_complete = true;

    switch(spinState){
        case(NotSpinning):
            m_colorMotor->Set(ControlMode::PercentOutput, 0.0);
        break;
        case(InitSpinning):
            NumSpins = 0;
            spinState = Spinning;
        break;
        case(Spinning):
            m_colorMotor->Set(ControlMode::PercentOutput, 0.2);
            if(GetCurrentColor() == kRedTarget)
                NumSpins++;
        break;
    }    
}
void ColorWheel::SetRotationsComplete(bool rotations_complete){
    this->rotations_complete = rotations_complete;
}
bool ColorWheel::GetRotationsComplete(){
    return rotations_complete;
}
void ColorWheel::SetExtended(bool extended){
    m_sol->Set(extended);
}
bool ColorWheel::GetExtended(){
    return m_sol->Get();
}

