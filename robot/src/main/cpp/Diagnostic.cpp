#include <Diagnostic.h>

Diagnostic::Diagnostic(frc::Joystick *joystick_1){
        this->js1 = joystick_1;
        M1l = new rev::CANSparkMax(kUltraLeftFrontMotorID, rev::CANSparkMax::MotorType::kBrushless);
        M2l = new rev::CANSparkMax(kUltraRightFrontMotorID, rev::CANSparkMax::MotorType::kBrushless);
        M4r = new rev::CANSparkMax(kUltraLeftBackMotorID, rev::CANSparkMax::MotorType::kBrushless);
        M5r = new rev::CANSparkMax(kUltraRightBackMotorID, rev::CANSparkMax::MotorType::kBrushless); 
}

void Diagnostic::run(){
    if(js1->GetRawButton(1)){
     M1l->Set(0.1);
     M2l->Set(0.1);
    }
    else{
     M1l->Set(0.0);
     M2l->Set(0.0);
    }
    if(js1->GetRawButton(2)){
     M4r->Set(0.1);
     M5r->Set(0.1);
    }
    else{
     M4r->Set(0.0);
     M5r->Set(0.0);
    }
}