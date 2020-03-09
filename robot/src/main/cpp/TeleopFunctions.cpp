#include <TeleopFunctions.h>

 TeleopFunctions::TeleopFunctions( frc::Joystick *joystick_1, Shooter *shooter, SparkDrive *sparkDrive){
    this->js1 = joystick_1;
    this->shooter = shooter;
    m_sparkDrive = sparkDrive;

    p = 0.015;
    i = 0.0;
    d = 0.0;

    frc::SmartDashboard::PutNumber("Turn P Value", p);
    frc::SmartDashboard::PutNumber("Turn I Value", i);
    frc::SmartDashboard::PutNumber("Turn D Value", d);

    pid_controller = new frc2::PIDController(p, i, d, 20 * 1_ms);

    pid_controller->SetSetpoint(0.0);
}
void TeleopFunctions::TurnToAngle(double targetAngle, double proportion){
    targetAngle += frc::SmartDashboard::GetNumber("Turn Fudge Factor", 0.0);
    //If a button is pressed, reset the counter, and signal that a turn is initiiated
    min_rotation_speed = frc::SmartDashboard::GetNumber("Min Rot Speed", 0.15);
    //frc::SmartDashboard::PutBoolean("turn complete?", turn_complete);
    //Find the difference between the current angle and the target angle, multiply by a set value, and use that to find the rate
    double error = (double)m_sparkDrive->GetGyroscope()->GetYaw() - targetAngle;
    std::cout << "Error: " << error;
    current_rotation_rate = error * proportion;
    std::cout << " 1: " << current_rotation_rate; 
    
    //Set the lower bound of the rotation speed so it is not less than the power necessary to turn the robot
    if(current_rotation_rate > 0)
        current_rotation_rate += min_rotation_speed;
    else if(current_rotation_rate < 0)
        current_rotation_rate -= min_rotation_speed;
    std::cout << " 2: " << current_rotation_rate;
    //Set the upper bound of the rotation rate
    current_rotation_rate = (current_rotation_rate > 1)? 1 : current_rotation_rate;
    current_rotation_rate = (current_rotation_rate < -1)? -1 : current_rotation_rate;
    std::cout << " 3: " << current_rotation_rate;

    //if we are not within the slop, then we are not done with the turn
    if(fabs(error) > slop){
        turn_complete = false;
        TURN_BUTTON_TIMEOUT = 0;
    }
    
    //If the turn has made it within the allowable error constant, increment the count
    if (fabs(error) < slop){
       TURN_BUTTON_TIMEOUT++;
    }

    frc::SmartDashboard::PutNumber("Error", error);
    frc::SmartDashboard::PutNumber("Current Rotation Rate", current_rotation_rate);
    frc::SmartDashboard::PutNumber("Target Angle", targetAngle);
    
    //Drive the robot using the SparkDrive::TankDrive function, with the forward/backward axis still based on
    //controller input, but the rotation axis of the drive base based on the rotation rate found
    m_sparkDrive->TankDrive(
        js1->GetRawAxis(kPrimaryDriverJoystickID), 
            -current_rotation_rate, 
        js1->GetRawButton(right_bumper), 
        js1->GetRawButton(left_bumper),
        0.0
    );

    //If the difference between the current angle and the target angle is within an allowable constant, 
    //and enough time has elapsed in while within that bound to allow for the turning to settle, 
    //declare the turn finished and reset the gyro
    if(fabs(error) < slop && TURN_BUTTON_TIMEOUT > timeToAdjust){
        turn_complete = true;
        //m_sparkDrive->GetGyroscope()->ZeroYaw();
        m_sparkDrive->TankDrive(0,0,false,false);
    }
}

void TeleopFunctions::WPITurnToAngle(double target_angle)
{
  //If a button is pressed, reset the counter, and signal that a turn is initiiated
  //frc::SmartDashboard::PutBoolean("turn complete?", turn_complete);
  //Find the difference between the current angle and the target angle, multiply by a set value, and use that to find the rate
  double error = ((double) m_sparkDrive->GetGyroscope()->GetYaw()) - target_angle;
  //std::cout << "Error: " << error;

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
  m_sparkDrive->TankDrive(
      js1->GetRawAxis(kPrimaryDriverJoystickID), 
      -current_rotation_rate, 
      js1->GetRawButton(right_bumper), 
      js1->GetRawButton(left_bumper),
      0.0
  );

  //If the difference between the current angle and the target angle is within an allowable constant, 
  //and enough time has elapsed in while within that bound to allow for the turning to settle, 
  //declare the turn finished and reset the gyro
  if(fabs(error) < slop && TURN_BUTTON_TIMEOUT > timeToAdjust){
      turn_complete = true;
      //m_sparkDrive->GetGyroscope()->ZeroYaw();
      m_sparkDrive->TankDrive(0,0,false,false);
  }
}

double TeleopFunctions::CalculateTurnToAngle(double target_angle)
{
  //If a button is pressed, reset the counter, and signal that a turn is initiiated
  //frc::SmartDashboard::PutBoolean("turn complete?", turn_complete);
  //Find the difference between the current angle and the target angle, multiply by a set value, and use that to find the rate
  double error = ((double) m_sparkDrive->GetGyroscope()->GetYaw()) - target_angle;
  //std::cout << "Error: " << error;

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


  //If the difference between the current angle and the target angle is within an allowable constant, 
  //and enough time has elapsed in while within that bound to allow for the turning to settle, 
  //declare the turn finished and reset the gyro
  if(fabs(error) < slop && TURN_BUTTON_TIMEOUT > timeToAdjust){
      turn_complete = true;
      //m_sparkDrive->GetGyroscope()->ZeroYaw();
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

bool TeleopFunctions::GetTurnStatus() {
    return turn_complete;
}
void TeleopFunctions::SetTurnStatus(bool turnStatus){
    turn_complete = turnStatus;
}

void TeleopFunctions::ShooterFunction(){
       // printf("joystick_addr = %d \n",js1);
        if(js1->GetRawButton(shooterButton)){
         //Hard coding power rn
         shooter->Shoot(1000);
        }
        else{
             shooter->Shoot(0);
         }
         
         
    
}