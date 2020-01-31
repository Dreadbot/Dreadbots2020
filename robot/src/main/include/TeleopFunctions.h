#pragma once
#include <Shooter.h>
#include <frc/Joystick.h>
class TeleopFunctions
{
    public:
        TeleopFunctions(frc::Joystick *joystick_1, Shooter *shooter);
        void ShooterFunction();
    private:
     frc::Joystick* js1;
     Shooter* shooter;
};