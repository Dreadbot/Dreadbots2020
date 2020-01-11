  
#pragma once

#include "rev/CANSparkMax.h"

class SparkDrive 
{
    public:
        SparkDrive(rev::CANSparkMax *lFront_in, rev::CANSparkMax *lBack_in, rev::CANSparkMax *rFront_in, rev::CANSparkMax *rBack_in);

        void MecDrive(double xAxis, double yAxis, double rot, bool turboButton, bool slowButton);

    private:
        rev::CANEncoder lFrontEncoder;
        rev::CANEncoder rFrontEncoder;
        rev::CANEncoder lBackEncoder;
        rev::CANEncoder rBackEncoder;

        rev::CANPIDController lFrontPID;
        rev::CANPIDController lBackPID;
        rev::CANPIDController rFrontPID;
        rev::CANPIDController rBackPID;

        rev::CANSparkMax *lFront;
        rev::CANSparkMax *lBack;
        rev::CANSparkMax *rFront;
        rev::CANSparkMax *rBack;
};