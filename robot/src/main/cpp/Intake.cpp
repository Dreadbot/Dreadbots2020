#include <Intake.h>

Intake::Intake(){ 
    intake_motor = new rev::CANSparkMax(3, rev::CANSparkMax::MotorType::kBrushless);
    *intakePid = intake_motor->GetPIDController();
    //intakePid = new rev::CANPIDController(intake_motor->GetPIDController());
    intakePid->SetP(6e-5);
    intakePid->SetI(1e-6);
    intakePid->SetD(0.3);
    intakePid->SetIZone(0);  
    intakePid->SetFF(0.000015);
    intakePid->SetOutputRange(-1.0, 1.0);
}

void Intake::Start(){
    intake_motor->Set(.5);
}

void Intake::Stop(){
    intake_motor->Set(0);
    running = false;
}
void Intake::SetSpeed(double speed) {
    if(!running){
        intakePid->SetReference(speed, rev::ControlType::kVelocity); 
        running = true;
    }
} 