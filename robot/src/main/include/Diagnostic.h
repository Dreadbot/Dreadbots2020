#pragma once

#include <frc/Joystick.h>
#include <rev/CANSparkMax.h>

#include "RobotUtilities.h"
class Diagnostic
{
    public:
        Diagnostic(frc::Joystick *joystick_1);
        void run();


    private:
    frc::Joystick * js1;
    rev::CANSparkMax * M1l;
    rev::CANSparkMax * M2l;
    rev::CANSparkMax * M4r;
    rev::CANSparkMax * M5r;
};