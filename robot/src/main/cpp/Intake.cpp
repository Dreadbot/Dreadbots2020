#include <Intake.h>

Intake::Intake(){
    intake_motor = new rev::CANSparkMax(3, rev::CANSparkMax::MotorType::kBrushless);
     
}

void Intake::Start(){
    intake_motor->Set(.5);
}

void Intake::Stop(){
    intake_motor->Set(0);
}