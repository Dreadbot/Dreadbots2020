#include <TeleopFunctions.h>

 TeleopFunctions::TeleopFunctions( frc::Joystick *joystick_1, Shooter *shooter, SparkDrive *sparkDrive){
    this->js1 = joystick_1;
    this->shooter = shooter;
    m_sparkDrive = sparkDrive;
}
void TeleopFunctions::TurnToAngle(double targetAngle, double proportion){
    //If a button is pressed, reset the counter, and signal that a turn is initiiated
    // if(js1->GetRawButton(a_button)){
    //   TURN_BUTTON_TIMEOUT = 0;
    //   turn_complete = false;
    // }

    //Find the difference between the current angle and the target angle, multiply by a set value, and use that to find the rate
    double error = ((double) m_sparkDrive->GetGyroscope()->GetYaw()) - targetAngle;
    //std::cout << "Error: " << error;
    current_rotation_rate = error * proportion;
    
    //Set the lower bound of the rotation speed so it is not less than the power necessary to turn the robot
    if(current_rotation_rate > 0)
        current_rotation_rate += min_rotation_speed;
    else if(current_rotation_rate < 0)
        current_rotation_rate -= min_rotation_speed;
    
    //Set the upper bound of the rotation rate
    current_rotation_rate = (current_rotation_rate > 1)? 1 : current_rotation_rate;
    current_rotation_rate = (current_rotation_rate < -1)? -1 : current_rotation_rate;

    if(fabs(error) > slop){
        turn_complete = false;
        TURN_BUTTON_TIMEOUT = 0;
    }
    
    //If the turn has made it within the allowable error constant, increment the count
    if (fabs(error) < slop)
       TURN_BUTTON_TIMEOUT++;

    frc::SmartDashboard::PutNumber("Error", error);
    frc::SmartDashboard::PutNumber("Current Rotation Rate", current_rotation_rate);
    frc::SmartDashboard::PutNumber("Target Angle", targetAngle);
    
    //Drive the robot using the SparkDrive::TankDrive function, with the forward/backward axis still based on
    //controller input, but the rotation axis of the drive base based on the rotation rate found
    m_sparkDrive->TankDrive(
        js1->GetRawAxis(kPrimaryDriverJoystickID), 
        current_rotation_rate, 
        js1->GetRawButton(right_bumper), 
        js1->GetRawButton(left_bumper),
        0.0
    );

    //If the difference between the current angle and the target angle is within an allowable constant, 
    //and enough time has elapsed in while within that bound to allow for the turning to settle, 
    //declare the turn finished and reset the gyro
    if(fabs(error) < slop && TURN_BUTTON_TIMEOUT > timeToAdjust){
        turn_complete = true;
        m_sparkDrive->GetGyroscope()->ZeroYaw();
        m_sparkDrive->TankDrive(0,0,false,false);
    }
}

bool TeleopFunctions::GetTurnStatus() {
    return turn_complete;
}
void TeleopFunctions::ShooterFunction(){
       // printf("joystick_addr = %d \n",js1);
        if(js1->GetRawButton(shooterButton)){
         //Hard coding power rn
         shooter->Shoot(0.2);
        }
        else{
             shooter->Shoot(0.00);
         }
         
         
    
}