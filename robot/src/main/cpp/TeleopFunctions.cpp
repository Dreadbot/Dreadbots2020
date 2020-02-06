#include <TeleopFunctions.h>

int shooterButton = 1;
//TeleopFunctions::TeleopFunctions(frc::Joystick *joystick_1, Shooter *shooter){
 TeleopFunctions::TeleopFunctions( frc::Joystick *joystick_1, Shooter *shooter){
    this->js1 = joystick_1;
    this->shooter = shooter;
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