#include <Shooter.h>

Shooter::Shooter(int shootermotorid,int aimmotorid){
    
    shooterMotor = new rev::CANSparkMax(shootermotorid, rev::CANSparkMax::MotorType::kBrushless);
    shooterPid = new rev::CANPIDController(shooterMotor->GetPIDController());
    //Needs to be calibrated
    shooterPid->SetP(6e-5);
    shooterPid->SetI(1e-6);
    shooterPid->SetD(0.3);
    shooterPid->SetIZone(0);  
    shooterPid->SetFF(0.000015);
    shooterPid->SetOutputRange(-1.0, 1.0);
    speed = 0.0; 


    aimMotor = new rev::CANSparkMax(aimmotorid, rev::CANSparkMax::MotorType::kBrushless);
    aimPid = new rev::CANPIDController(aimMotor->GetPIDController());
    //Needs to be calibrated
    aimPid->SetP(6e-5);
    aimPid->SetI(1e-6);
    aimPid->SetD(0.3);
    aimPid->SetIZone(0);  
    aimPid->SetFF(0.000015);
    aimPid->SetOutputRange(-1.0, 1.0);
 
}

void Shooter::Shoot(double shooterSpeed){
    speed = shooterSpeed;
    shooterMotor->Set(speed);
}
void Shooter::AimHeight(double rotations) {
    aimPid->SetReference(rotations, rev::ControlType::kPosition); 
}