#include <Diagnostic.h>
#include <RobotUtilities.h>
#include <Intake.h>
#include <Robot.h>

Diagnostic::Diagnostic(frc::Joystick* joystick_1,
frc::Joystick* joystick_2,
 SparkDrive* sparkdrive,
 Intake* kIntakeMotorID,
 rev::CANSparkMax* kGenevaMotorID,
 rev::CANSparkMax* kFlyWheelMotorID,
 rev::CANSparkMax* kAimMotorID, 
 ctre::phoenix::motorcontrol::can::WPI_TalonSRX* kColorWheelMotorID,
 Climber * kClimbMotorID,
 frc::Solenoid* kPunchSolenoidID
 //frc::Solenoid* solcolor
 ){
        this->js1 = joystick_1;
        this->js2 = joystick_2;
        this->sparkdrive = sparkdrive;
        this->IN = kIntakeMotorID;
        this->Gen= kGenevaMotorID;
        this->shooty = kFlyWheelMotorID;
        this->aim = kAimMotorID;
        this->color = kColorWheelMotorID;
        this->climb = kClimbMotorID;
        this->solup = kPunchSolenoidID;
        //this->solcolor =kColorWheelSolenoidID;
}

void Diagnostic::run(){
    if((fabs(js1->GetRawAxis(1))) >= 0.025 || (fabs(js1->GetRawAxis(2)))>=0.025){
        if(js1->GetRawButton(10)){
        sparkdrive->TankDrive(AxisY1,AxisX1,true,false);
        }
        else if(js1->GetRawButton(9)){
        sparkdrive->TankDrive(AxisY1,AxisX1,false,true);
        }
        else{
        sparkdrive->TankDrive(AxisY1,AxisX1,false,false);
        }
        }
    else{
    sparkdrive->TankDrive(0,0,false,false);
    }
    if(js1->GetRawButton(8)){
        climb->SetWinch(0.1);
    }
    else if(js1->GetRawButton(7)){
        climb->SetWinch(-0.1);
    }
   if(js2->GetRawButton(3)){
       shooty->Set(0.5);
   }
   else{
       shooty->Set(0);
   }
   if(js2->GetRawButton(1) || js2->GetRawButton(2)){
   if(js2->GetRawButton(1)){
       IN->SetSpeed(0.2);
   }
   else if(js2->GetRawButton(2)){
       IN->SetSpeed(-0.2);
   }
   else{
       IN->SetSpeed(0);
   }
   }
   if(js2->GetRawButton(5) || js2->GetRawButton(6)){
       if (js2->GetRawButton(5))
       {
        Gen->Set(0.2);
       }
       else if(js2->GetRawButton(6)){
           Gen->Set(-0.2);
       }
       else{
           Gen->Set(0);
       }
   }
}
