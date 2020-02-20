#include <Shooter.h>
Shooter::Shooter(int shooter_motor_id, int aim_motor_id){
    m_shooterMotor = new rev::CANSparkMax(shooter_motor_id, rev::CANSparkMax::MotorType::kBrushless);
    m_shooterPid = new rev::CANPIDController(m_shooterMotor->GetPIDController());
    //Needs to be calibrated
    m_shooterPid->SetP(6e-5);
    m_shooterPid->SetI(1e-6);
    m_shooterPid->SetD(0.3);
    m_shooterPid->SetIZone(0);  
    m_shooterPid->SetFF(0.000015);
    m_shooterPid->SetOutputRange(-1.0, 1.0);
    speed = 0.0; 


    m_aimMotor = new rev::CANSparkMax(aim_motor_id, rev::CANSparkMax::MotorType::kBrushless);
    m_aimPid = new rev::CANPIDController(m_aimMotor->GetPIDController());
    //Needs to be calibrated
    m_aimPid->SetP(0.1); //6e-5
    m_aimPid->SetI(0); //1e-6
    m_aimPid->SetD(0);  //0.3
    m_aimPid->SetIZone(0);  
    m_aimPid->SetFF(0);  //000015
    m_aimPid->SetOutputRange(-1.0, 1.0);

    upper_limit_switch = new frc::DigitalInput(1);
    lower_limit_switch = new frc::DigitalInput(2);
}
Shooter::Shooter(rev::CANSparkMax *shooterMotor, rev::CANSparkMax *aimMotor) 
{
    m_shooterMotor = shooterMotor;
    m_shooterPid = new rev::CANPIDController(m_shooterMotor->GetPIDController());
    //Needs to be calibrated
    m_shooterPid->SetP(6e-5);
    m_shooterPid->SetI(1e-6);
    m_shooterPid->SetD(0.3);
    m_shooterPid->SetIZone(0);  
    m_shooterPid->SetFF(0.000015);
    m_shooterPid->SetOutputRange(-1.0, 1.0);
    speed = 0.0; 


    m_aimMotor = aimMotor;
    m_aimPid = new rev::CANPIDController(m_aimMotor->GetPIDController());
    //Needs to be calibrated
    m_aimPid->SetP(0); //6e-5
    m_aimPid->SetI(0); //1e-6
    m_aimPid->SetD(0);  //0.3
    m_aimPid->SetIZone(0);  
    m_aimPid->SetFF(0);  //000015
    m_aimPid->SetOutputRange(-1.0, 1.0);
 
}

void Shooter::Shoot(double shooterSpeed){
    speed = shooterSpeed;
    m_shooterMotor->Set(speed);
}
void Shooter::AimHeight(double rotations) {
    m_aimPid->SetReference(rotations, rev::ControlType::kPosition); 
}

void Shooter::SetAimHeightP(double p){
    m_aimPid->SetP(p);
}

void Shooter::SetShootingPercentOutput(double percent_output)
{
  m_shooterMotor->Set(percent_output);
}

void Shooter::SetAdjusterPercentOutput(double percent_output)
{
  if(upper_limit_switch->Get())
  {
    if(percent_output > 0.0)
    {
      percent_output = 0.0;
    }
  }

  if(lower_limit_switch->Get())
  {
    if(percent_output < 0.0)
    {
      percent_output = 0.0;
    }
  }
  
  m_aimMotor->Set(percent_output);
}