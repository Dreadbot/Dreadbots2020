#include <TeleopFunctions.h>

TeleopFunctions::TeleopFunctions( frc::Joystick *joystick_1, Shooter *shooter, SparkDrive *spark_drive)
  :  js1(joystick_1), m_shooter(shooter), m_spark_drive(spark_drive)
{
  p = 0.021;
  i = 0.019;
  d = 0.0;

  frc::SmartDashboard::PutNumber("Turn P Value", p);
  frc::SmartDashboard::PutNumber("Turn I Value", i);
  frc::SmartDashboard::PutNumber("Turn D Value", d);
  pid_controller = new frc2::PIDController(p, i, d, 20 * 1_ms);
  pid_controller->SetSetpoint(0.0);

  min_rotation_speed = 0;
}

void TeleopFunctions::TurnToAngle(double target_angle, double proportion)
{
  target_angle += frc::SmartDashboard::GetNumber("Turn Fudge Factor", 0.0);
  min_rotation_speed = frc::SmartDashboard::GetNumber("Min Rot Speed", 0.15);

  //Find the difference between the current angle and the target angle, multiply by a set value, and use that to find the rate
  double error = (double)m_spark_drive->GetGyroscope()->GetYaw() - target_angle;
  current_rotation_rate = error * proportion; 

  //Set the lower bound of the rotation speed so it is not less than the power necessary to turn the robot
  if(current_rotation_rate > 0)
  {
    current_rotation_rate += min_rotation_speed;
  }
  else if(current_rotation_rate < 0)
  {
    current_rotation_rate -= min_rotation_speed;
  }

  //Set the upper bound of the rotation rate
  current_rotation_rate = (current_rotation_rate > 1)? 1 : current_rotation_rate;
  current_rotation_rate = (current_rotation_rate < -1)? -1 : current_rotation_rate;

  //if we are not within the slop, then we are not done with the turn
  if(fabs(error) > slop)
  {
    turn_complete = false;
    turn_button_timeout = 0;
  }
    
  //If the turn has made it within the allowable error constant, increment the count
  if(fabs(error) < slop)
  {
    turn_button_timeout++;
  }

  frc::SmartDashboard::PutNumber("Error", error);
  frc::SmartDashboard::PutNumber("Current Rotation Rate", current_rotation_rate);
  frc::SmartDashboard::PutNumber("Target Angle", target_angle);
    
  //Drive the robot using the SparkDrive::TankDrive function, with the forward/backward axis still based on
  //controller input, but the rotation axis of the drive base based on the rotation rate found
  m_spark_drive->TankDrive(
    js1->GetRawAxis(kPrimaryDriverJoystickID), 
    -current_rotation_rate, 
    js1->GetRawButton(right_bumper), 
    js1->GetRawButton(left_bumper),
    0.0
  );

  //If the difference between the current angle and the target angle is within an allowable constant, 
  //and enough time has elapsed in while within that bound to allow for the turning to settle, 
  //declare the turn finished and reset the gyro
  if(fabs(error) < slop && turn_button_timeout > time_to_adjust)
  {
    turn_complete = true;
    m_spark_drive->TankDrive(0,0,false,false);
  }
}

void TeleopFunctions::WPITurnToAngle(double target_angle)
{
  //If a button is pressed, reset the counter, and signal that a turn is initiiated
  //Find the difference between the current angle and the target angle, multiply by a set value, and use that to find the rate
  double error = ((double) m_spark_drive->GetGyroscope()->GetYaw()) - target_angle;
  UpdatePIDController();

  current_rotation_rate = pid_controller->Calculate(error);
    
  //Set the upper bound of the rotation rate
  current_rotation_rate = (current_rotation_rate > 1)? 1 : current_rotation_rate;
  current_rotation_rate = (current_rotation_rate < -1)? -1 : current_rotation_rate;
  
  frc::SmartDashboard::PutNumber("Error", error);
  frc::SmartDashboard::PutNumber("Current Rotation Rate", current_rotation_rate);
  frc::SmartDashboard::PutNumber("Target Angle", target_angle);

  //Drive the robot using the SparkDrive::TankDrive function, with the forward/backward axis still based on
  //controller input, but the rotation axis of the drive base based on the rotation rate found
  m_spark_drive->TankDrive(
    js1->GetRawAxis(kPrimaryDriverJoystickID), 
    -current_rotation_rate, 
    js1->GetRawButton(right_bumper), 
    js1->GetRawButton(left_bumper),
    0.0
  );

  //If the difference between the current angle and the target angle is within an allowable constant, 
  //and enough time has elapsed in while within that bound to allow for the turning to settle, 
  //declare the turn finished and reset the gyro
  if(fabs(error) < slop && turn_button_timeout > time_to_adjust)
  {
    turn_complete = true;
    m_spark_drive->TankDrive(0,0,false,false);
  }
}

double TeleopFunctions::CalculateTurnToAngle(double target_angle)
{
  //Find the difference between the current angle and the target angle, multiply by a set value, and use that to find the rate
  double error = ((double) m_spark_drive->GetGyroscope()->GetYaw()) - target_angle;

  UpdatePIDController();

  current_rotation_rate = pid_controller->Calculate(error);
    
  //Set the upper bound of the rotation rate
  current_rotation_rate = (current_rotation_rate > 1)? 1 : current_rotation_rate;
  current_rotation_rate = (current_rotation_rate < -1)? -1 : current_rotation_rate;
  
  frc::SmartDashboard::PutNumber("Error", error);
  frc::SmartDashboard::PutNumber("Current Rotation Rate", current_rotation_rate);
  frc::SmartDashboard::PutNumber("Target Angle", target_angle);

  //If the difference between the current angle and the target angle is within an allowable constant, 
  //and enough time has elapsed in while within that bound to allow for the turning to settle, 
  //declare the turn finished and reset the gyro
  if(fabs(error) < slop && turn_button_timeout > time_to_adjust)
  {
      turn_complete = true;
      current_rotation_rate = 0;
  }
  return current_rotation_rate;
}

void TeleopFunctions::UpdatePIDController()
{
  p = frc::SmartDashboard::GetNumber("Turn P Value", 0.0);
  i = frc::SmartDashboard::GetNumber("Turn I Value", 0.0);
  d = frc::SmartDashboard::GetNumber("Turn D Value", 0.0); 

  pid_controller->SetP(p);
  pid_controller->SetI(i);
  pid_controller->SetD(d);

}

bool TeleopFunctions::GetTurnStatus() 
{
    return turn_complete;
}

void TeleopFunctions::SetTurnStatus(bool turnStatus)
{
    turn_complete = turnStatus;
}

void TeleopFunctions::ShooterFunction()
{
       // printf("joystick_addr = %d \n",js1);
        if(js1->GetRawButton(shooterButton)){
         //Hard coding power rn
         m_shooter->Shoot(1000);
        }
        else{
             m_shooter->Shoot(0);
         }
         
         
    
}