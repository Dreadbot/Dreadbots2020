#include <Shooter.h>

Shooter::Shooter(int shooter_motor_id, int aim_motor_id){
  shooting_motor = new rev::CANSparkMax(shooter_motor_id, rev::CANSparkMax::MotorType::kBrushless);
  shooting_motor_pid_controller = new rev::CANPIDController(shooting_motor->GetPIDController());
  shooter_encoder = new rev::CANEncoder(shooting_motor->GetEncoder());
  //Needs to be calibrated
  shooting_motor_pid_controller->SetP(6e-5);
  shooting_motor_pid_controller->SetI(1e-6);
  shooting_motor_pid_controller->SetD(0.3);
  shooting_motor_pid_controller->SetIZone(0);  
  shooting_motor_pid_controller->SetFF(0.000015);
  shooting_motor_pid_controller->SetOutputRange(-1.0, 1.0);
  speed = 0.0; 


  aiming_motor = new rev::CANSparkMax(aim_motor_id, rev::CANSparkMax::MotorType::kBrushless);
  aiming_motor_pid_controller = new rev::CANPIDController(aiming_motor->GetPIDController());
  //Needs to be calibrated
  aiming_motor_pid_controller->SetP(0.1); //6e-5
  aiming_motor_pid_controller->SetI(0); //1e-6
  aiming_motor_pid_controller->SetD(0);  //0.3
  aiming_motor_pid_controller->SetIZone(0);  
  aiming_motor_pid_controller->SetFF(0);  //000015
  aiming_motor_pid_controller->SetOutputRange(-1.0, 1.0);

  upper_limit_switch = new frc::DigitalInput(1);
  lower_limit_switch = new frc::DigitalInput(2);
}

Shooter::Shooter(rev::CANSparkMax *shooterMotor, rev::CANSparkMax *aimMotor) 
{
  shooting_motor = shooterMotor;
  shooting_motor_pid_controller = new rev::CANPIDController(shooting_motor->GetPIDController());
  shooter_encoder = new rev::CANEncoder(shooting_motor->GetEncoder());
  //Needs to be calibrated
  shooting_motor_pid_controller->SetP(6e-5);
  shooting_motor_pid_controller->SetI(1e-6);
  shooting_motor_pid_controller->SetD(0.3);
  shooting_motor_pid_controller->SetIZone(0);  
  shooting_motor_pid_controller->SetFF(0.000015);
  shooting_motor_pid_controller->SetOutputRange(-1.0, 1.0);
  speed = 0.0; 


  aiming_motor = aimMotor;
  aiming_motor_pid_controller = new rev::CANPIDController(aiming_motor->GetPIDController());
  //Needs to be calibrated
  aiming_motor_pid_controller->SetP(0); //6e-5
  aiming_motor_pid_controller->SetI(0); //1e-6
  aiming_motor_pid_controller->SetD(0);  //0.3
  aiming_motor_pid_controller->SetIZone(0);  
  aiming_motor_pid_controller->SetFF(0.000015);  //000015
  aiming_motor_pid_controller->SetOutputRange(-1.0, 1.0);

  upper_limit_switch = new frc::DigitalInput(1);
  lower_limit_switch = new frc::DigitalInput(2);
}

void Shooter::Shoot(int rpm){
    shooting_motor_pid_controller->SetReference(rpm, rev::ControlType::kVelocity);
}
void Shooter::AimHeight(double rotations) {
  aiming_motor_pid_controller->SetReference(rotations, rev::ControlType::kPosition);
}

void Shooter::SetAimHeightP(double p){
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
  std::cout << "limit switch logic" << std::endl;
  if(upper_limit_switch->Get())
  {
    if(percent_output > 0.0)
    {
      percent_output = 0.0;
    }
  }

  // When the lower limit switch is reading true,
  // we no longer want to go past the limit switch.
  if(lower_limit_switch->Get())
  {
    if(percent_output < 0.0)
    {
      percent_output = 0.0;
    }
  }
  
  // After both safety checks, set the motor speed
  // accordingly.
  aiming_motor->Set(percent_output);
}
int Shooter::GetShootingSpeed(){
  //Return the actual velocity value of the encoder in rpms
  return shooter_encoder->GetVelocity();
}