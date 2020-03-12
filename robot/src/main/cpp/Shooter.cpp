#include <Shooter.h>
Shooter::Shooter(rev::CANSparkMax *shooter_motor, rev::CANSparkMax *aim_motor)
  : shooting_motor(shooter_motor), aiming_motor(aim_motor)
{
  shooting_motor_pid_controller = new rev::CANPIDController(shooting_motor->GetPIDController());
  shooter_encoder = new rev::CANEncoder(shooting_motor->GetEncoder());
  shooting_motor->RestoreFactoryDefaults();
  shooting_motor_pid_controller->SetP(9e-3);
  shooting_motor_pid_controller->SetI(5e-7);
  shooting_motor_pid_controller->SetD(0);
  shooting_motor_pid_controller->SetIZone(0);  
  shooting_motor_pid_controller->SetFF(0.000015);
  shooting_motor_pid_controller->SetOutputRange(-1.0, 1.0);
  speed = 0.0; 

  aiming_motor_pid_controller = new rev::CANPIDController(aiming_motor->GetPIDController());
  aiming_motor_encoder = new rev::CANEncoder(aiming_motor->GetEncoder());
  aiming_motor_pid_controller->SetP(0.1);
  aiming_motor_pid_controller->SetI(0);
  aiming_motor_pid_controller->SetD(0);
  aiming_motor_pid_controller->SetIZone(0);  
  aiming_motor_pid_controller->SetFF(0.000015);
  aiming_motor_pid_controller->SetOutputRange(-1.0, 1.0);

  upper_limit_switch = new frc::DigitalInput(1);
  lower_limit_switch = new frc::DigitalInput(2);
  ready_to_aim = false;
  lower_limit_hit = false;
  upper_limit_hit = false;

  vision_light = new frc::Solenoid(7);
}

void Shooter::Shoot(int rpm)
{
  shooting_motor_pid_controller->SetReference(rpm, rev::ControlType::kVelocity);
}

void Shooter::AimHeight(double rotations)
{
  aiming_motor_pid_controller->SetReference(rotations, rev::ControlType::kPosition);
}

void Shooter::SetAimHeightP(double p)
{
  aiming_motor_pid_controller->SetP(p);
}

void Shooter::SetShootingPercentOutput(double percent_output)
{
  shooting_motor->Set(percent_output);
}

void Shooter::SetAdjusterPercentOutput(double percent_output)
{
  // When the upper limit switch is reading true,
  // we no longer want to go past the limit switch.
  //std::cout << "limit switch logic" << std::endl;

  // When the lower limit switch is reading true,
  // we no longer want to go past the limit switch.
  if(GetLowerLimitSwitch() && percent_output > 0)
  {
    percent_output = 0.0;
  }

  else if(GetUpperLimitSwitch() && percent_output < 0)
  {
    percent_output = 0.0;
  }
  
  // After both safety checks, set the motor speed
  // accordingly.
  aiming_motor->Set(percent_output);
}

int Shooter::GetShootingSpeed()
{
  //Return the actual velocity value of the encoder in rpms
  return shooter_encoder->GetVelocity();
}

void Shooter::SetAdjusterPosition(double position){ //Takes number 0 to 1
  if(position > 1)
  {
    position = 1;
  }
  else if (position < 0)
  {
    position = 0;
  }

  position = min_hood_position + (position * range);

  frc::SmartDashboard::PutNumber("Adjuster Position", GetHoodPosition());
  aiming_motor_pid_controller->SetReference(position, rev::ControlType::kPosition);
}

void Shooter::SetLowerLimit(int position)
{
  min_hood_position = position;
  lower_limit_hit = true;
}

void Shooter::SetUpperLimit(int position)
{
  max_hood_position = position;
  upper_limit_hit = true;
}

void Shooter::SetAimReadiness(bool ready)
{
  ready_to_aim = ready;
  if(ready)
  {
    range = max_hood_position - min_hood_position;
  }
}

void Shooter::SetUpperBool(bool value)
{
  upper_limit_hit = value;
}
void Shooter::SetLowerBool(bool value)
{
  lower_limit_hit = value;
}

bool Shooter::GetAimReadiness()
{
  return ready_to_aim;
}

int Shooter::GetHoodPosition()
{
  return aiming_motor_encoder->GetPosition();
}

bool Shooter::GetUpperLimitSwitch()
{
  return !upper_limit_switch->Get();
}

bool Shooter::GetLowerLimitSwitch()
{
  return !lower_limit_switch->Get();
}

bool Shooter::GetUpperLimitBool()
{
  return upper_limit_hit;
}

bool Shooter::GetLowerLimitBool()
{
  return lower_limit_hit;
} 

void Shooter::SetPID(double P, double I, double D)
{
  shooting_motor_pid_controller->SetP(P);
  shooting_motor_pid_controller->SetI(I);
  shooting_motor_pid_controller->SetD(D);
}

void Shooter::SetVisionLight(bool value)
{
  vision_light->Set(value);
}