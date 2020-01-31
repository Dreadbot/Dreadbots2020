#include <Shooter.h>

Shooter::Shooter(){
    shooterMotor = new rev::CANSparkMax(3, rev::CANSparkMax::MotorType::kBrushless);
    shooterPid = new rev::CANPIDController(shooterMotor->GetPIDController());
    //Needs to be calibrated
    shooterPid->SetP(6e-5);
    shooterPid->SetI(1e-6);
    shooterPid->SetD(0.3);
    shooterPid->SetIZone(0);  
    shooterPid->SetFF(0.000015);
    shooterPid->SetOutputRange(-1.0, 1.0);
    speed = 0.0; 
}

void Shooter::Shoot(double shooterSpeed){
    speed = shooterSpeed;
    shooterMotor->Set(speed);
}