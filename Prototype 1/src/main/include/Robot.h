/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>

#include "AHRS.h"
#include "ctre/Phoenix.h"
#include "Drive.h"

#include <frc/DriverStation.h>
#include <frc/Joystick.h>
#include <frc/livewindow/LiveWindow.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>
#include <frc/TimedRobot.h>
#include <frc/Preferences.h>
#include <frc/SmartDashboard/SendableChooser.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Ultrasonic.h>

Joystick *JoyA = new Joystick(0);

TalonSRX LF_motor = {0};
TalonSRX LR_motor = {1};
TalonSRX RF_motor = {2};
TalonSRX RR_motor = {3};
WPI_TalonSRX *lFront = new WPI_TalonSRX(0); //left front
WPI_TalonSRX *rFront = new WPI_TalonSRX(2); //right front
WPI_TalonSRX *lBack = new WPI_TalonSRX(1); //left rear
WPI_TalonSRX *rBack = new WPI_TalonSRX(3); //right rear

Drive *drive = new Drive(lFront, lBack, rFront, rBack);
//Drive *drive = new Drive(&LF_motor, &LR_motor, &RF_motor, &RR_motor);
AHRS *ahrs; 		// Declares the base variable for us to use the navX MXP

Ultrasonic *ultra1 = new Ultrasonic(0,1); // Ultrasonic Sensor definition

Preferences *prefs; // Define preferences to be configurable without having to change the code, values change on the driver station

float yaw;
float changefactor = .2; // Used as a control multiplier in setting the rate of change for the gyro control
float degreechangefactor = 2;

class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;
  void RobotPeriodic() override;
  void AutonomousInit() override;
  void AutonomousPeriodic() override;
  void TeleopInit() override;
  void TeleopPeriodic() override;
  void TestPeriodic() override;

	// ***********************************************************************************************************************************************//
	// The Mecanum function will fill an array of floating point numbers with the motor speeds needed to drive the robot
	// We will pass the three joystick controls to the function.  This will allow us to drive the robot via a joystick or autonomously as needed.
	// Note that the Joystick values maybe different than you think.  For instance to go forward you push the FWandBackStick forward which returns a negative number from the controller
	// ***********************************************************************************************************************************************//
	void Mecanum(float TurnStick, float LEFTandRIGHTStick, float FWandBackStick) {
		// Define variable for use in holding the motor speed values; set default values to zero
	    float LF = 0; //Float to hold calculation for Left Front Motor
	    float LR = 0; //Float to hold calculation for Left Rear Motor
	    float RF = 0; //Float to hold calculation for Right Front Motor
	    float RR = 0; //Float to hold calculation for Right Rear Motor

	    // Add turn to turn variables to be used in the cases below
	    float T_LF = TurnStick;
	    float T_LR = TurnStick;
	    float T_RF = -TurnStick;
	    float T_RR = -TurnStick;

	    // Case One: Positive Forward; Positive Right
	    if ((LEFTandRIGHTStick >= 0) && (FWandBackStick >=0))
	    {
	    	LF = LEFTandRIGHTStick+FWandBackStick + T_LF;
	        LR = LEFTandRIGHTStick-FWandBackStick + T_LR;
	        RF = LEFTandRIGHTStick-FWandBackStick + T_RF;
	        RR = LEFTandRIGHTStick+FWandBackStick + T_RR;
	    }

	    // Case Two: Negative Forward; Positive Right
	    if ((LEFTandRIGHTStick <= 0) && (FWandBackStick >=0))
	    {
	    	LF = LEFTandRIGHTStick-FWandBackStick + T_LF;
	        LR = LEFTandRIGHTStick+FWandBackStick + T_LR;
	        RF = LEFTandRIGHTStick+FWandBackStick + T_RF;
	        RR = LEFTandRIGHTStick-FWandBackStick + T_RR;
	    }

	    // Case Three: Positive Case Forward; Negative Case Right
	    if ((LEFTandRIGHTStick >= 0) && (FWandBackStick <=0))
	    {
	    	LF = LEFTandRIGHTStick+FWandBackStick + T_LF;
	        LR = LEFTandRIGHTStick-FWandBackStick + T_LR;
	        RF = LEFTandRIGHTStick-FWandBackStick + T_RF;
	        RR = LEFTandRIGHTStick+FWandBackStick + T_RR;
	    }

	    // Case Four: Negative Case Forward; Negative Case Right
	    if ((LEFTandRIGHTStick <= 0) && (FWandBackStick <=0))
	    {
	    	LF = LEFTandRIGHTStick-FWandBackStick + T_LF;
	        LR = LEFTandRIGHTStick+FWandBackStick + T_LR;
	        RF = LEFTandRIGHTStick+FWandBackStick + T_RF;
	        RR = LEFTandRIGHTStick-FWandBackStick + T_RR;
	    }

	    // Normalize the data so that we don't exceed our motors control parameters of -1 to 1
	    if (LF>1) LF=1;
	    if (LR>1) LR=1;
	    if (RF>1) RF=1;
	    if (RR>1) RR=1;
	    if (LF<-1) LF=-1;
	    if (LR<-1) LR=-1;
	    if (RF<-1) RF=-1;
	    if (RR<-1) RR=-1;

	    // Set motors speed based on Mecanum Drive calculations for two axis stick
	    LF_motor.Set(ControlMode::PercentOutput, LF);
	    RF_motor.Set(ControlMode::PercentOutput, -1*RF);  // The right side motors sit reversed from the left side motors and hence we have to reverse the direction of the motor to make it all work
	    LR_motor.Set(ControlMode::PercentOutput, LR);
	    RR_motor.Set(ControlMode::PercentOutput, -1*RR);  // The right side motors sit reversed from the left side motors and hence we have to reverse the direction of the motor to make it all work

		}

	// Function to calculate and set drive settings until ultrasound target is hit
	int UltraDrive (int inchWall){
		frc::SmartDashboard::PutNumber("UltraSound_Front", ultra1->GetRangeInches());
		//frc::SmartDashboard::PutNumber("Desired_Distance_to_wall", inchWall);
		frc::SmartDashboard::PutNumber("Gyro", ahrs->GetCompassHeading());

		// Find the current yaw angle and check it to see if we need to change our rotation
		float yawrate = ahrs->GetRawGyroZ(); // This function returns the rate of change of yaw in degrees/second
		float yawnew = ahrs->GetYaw();  // This is the current yaw setting.  We will use this to compare with the existing setting to see if we need to make a change
		float yawdiff = yawnew - yaw;

		float yawadjust = yawrate / 180 * changefactor;
		float normyawdiff = yawdiff / 180 * degreechangefactor;
		yawadjust = yawadjust + normyawdiff;

		if (yawadjust < -1) yawadjust = -1;
		if (yawadjust > 1) yawadjust = 1;

		float TurnStick = JoyA->GetRawAxis(4) * -.75; // left joystick x axis (reverse the stick to get the expected behavior from the control; .75 to get 3/4 speed as it is typically to fast.)
		float FWandBackStick = JoyA->GetRawAxis(0) * -1; // right joystick y axis
		float LEFTandRIGHTStick = JoyA->GetRawAxis(1) * -1; // right joystick x axis

		// Use UltraSound Ranging to get to a specific location - Overrides the Forward and Back Stick Control
		int range = ultra1->GetRangeInches(); // reads the range on the ultrasonic sensor
		if(range > 200)
			range = 200;
		int gap = 0;
		if (inchWall > range)
			gap = inchWall - range;
		if (range > inchWall)
			gap = range - inchWall;
		// We need to adjust the divider <rangecomp> we are using to calculate the power of the drive.
		// We need more power as we get closer and less when we are a long way away.
		// This helps with the fine tuning and makes the final stop more accurate from long ranges.
		int rangecomp = 300;
		int rangecompclose = 1;
		if(gap < 40)
			rangecomp = 200;
		if(gap < 20)
			rangecomp = 170;
		if(gap < 10)
			rangecomp = 75;
		if(gap < 5)
			rangecomp = 45;
		FWandBackStick = ((float)inchWall-(float)range)/(float)rangecomp*(float)rangecompclose;
		if(FWandBackStick > .3)
			FWandBackStick = .3;
		if(FWandBackStick < -.3)
			FWandBackStick = -.3;
		if(gap < 30 && FWandBackStick > .15)
			FWandBackStick = .15;
		if(gap < 30 && FWandBackStick < -.15)
			FWandBackStick = -.15;

		TurnStick = yawadjust;

		frc::SmartDashboard::PutNumber("InchWall", inchWall);
		frc::SmartDashboard::PutNumber("Range", range);

		Mecanum(TurnStick, FWandBackStick, LEFTandRIGHTStick);

		return gap;
	}

 private:
  frc::SendableChooser<std::string> m_chooser;
  const std::string kAutoNameDefault = "Default";
  const std::string kAutoNameCustom = "My Auto";
  std::string m_autoSelected;
};
