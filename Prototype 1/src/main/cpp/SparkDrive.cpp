#include "SparkDrive.h"
#include <iostream>

SparkDrive::SparkDrive(rev::CANSparkMax *lFront_in, rev::CANSparkMax *lBack_in, rev::CANSparkMax *rFront_in, rev::CANSparkMax *rBack_in) 
: lFrontEncoder(lFront_in->GetEncoder()), rFrontEncoder(rFront_in->GetEncoder()), lBackEncoder(lBack_in->GetEncoder()), rBackEncoder(rBack_in->GetEncoder()), lFrontPID(lFront_in->GetPIDController()), rFrontPID(rFront_in->GetPIDController()), lBackPID(lBack_in->GetPIDController()), rBackPID(rBack_in->GetPIDController())
{
    lFront = lFront_in;
    lBack = lBack_in;
    rFront = rFront_in;
    rBack = rBack_in;
	
	lFrontPID.SetP(0.2);
    lFrontPID.SetI(1e-4);
    lFrontPID.SetD(1);
    lFrontPID.SetIZone(0.1);
    lFrontPID.SetFF(0);
    lFrontPID.SetOutputRange(-1, 1);

	rFrontPID.SetP(0.2);
    rFrontPID.SetI(1e-4);
    rFrontPID.SetD(1);
    rFrontPID.SetIZone(0.1);
    rFrontPID.SetFF(0);
    rFrontPID.SetOutputRange(-1, 1);

	lBackPID.SetP(0.2);
    lBackPID.SetI(1e-4);
    lBackPID.SetD(1);
    lBackPID.SetIZone(0.1);
    lBackPID.SetFF(0);
    lBackPID.SetOutputRange(-1, 1);

	rBackPID.SetP(0.2);
    rBackPID.SetI(1e-4);
    rBackPID.SetD(1);
    rBackPID.SetIZone(0.1);
    rBackPID.SetFF(0);
    rBackPID.SetOutputRange(-1, 1);
}

void SparkDrive::MecDrive(double xAxis, double yAxis, double rot, bool turboButton, bool slowButton) //homemade mecanum drive!
	{
		double noMove = 0.2; //Dead area of the axes
		double maxSpeed = .5; //normal speed (not turbo)

		if (fabs(xAxis) < noMove)
			xAxis = 0.0;

		if (fabs(yAxis) < noMove)
			yAxis = 0.0;

		if (fabs(rot) < noMove)
			rot = 0.0;

		if (turboButton)
			maxSpeed = 1;

		else if (slowButton)
			maxSpeed = .2;

		else
			maxSpeed = .5;

		double lFrontSpeed = -yAxis - xAxis - rot;
		double rFrontSpeed = +yAxis - xAxis - rot;
		double rBackSpeed = +yAxis + xAxis - rot;
		double lBackSpeed = -yAxis + xAxis - rot;

		if (fabs(lFrontSpeed) > 1)
			lFrontSpeed = fabs(lFrontSpeed) / lFrontSpeed;

		if (fabs(lBackSpeed) > 1)
			lBackSpeed = fabs(lBackSpeed) / lBackSpeed;

		if (fabs(rFrontSpeed) > 1)
			rFrontSpeed = fabs(rFrontSpeed) / rFrontSpeed;

		if (fabs(rBackSpeed) > 1)
			rBackSpeed = fabs(rBackSpeed) / rBackSpeed;

		lFront -> Set(-1*lFrontSpeed*maxSpeed);
		lBack -> Set(-1*lBackSpeed*maxSpeed);
		rFront -> Set(-1*rFrontSpeed*maxSpeed);
		rBack -> Set(-1*rBackSpeed*maxSpeed);
	}