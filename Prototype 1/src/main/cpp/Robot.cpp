/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"
#include "frc/WPILIB.h"
#include "Drive.h"

using namespace std::chrono;
using namespace std;

// Global Variables defined to make them available to all methods
static std::chrono::high_resolution_clock::time_point starttimeAuto;
static std::chrono::high_resolution_clock::time_point currenttimeAuto;
static bool firstinit;
static bool secondinit;
static bool thirdinit;
int countnew = 0;

nt::NetworkTableEntry DTW;

void Robot::RobotInit() {


  m_chooser.SetDefaultOption(kAutoNameDefault, kAutoNameDefault);
  m_chooser.AddOption(kAutoNameCustom, kAutoNameCustom);

  auto inst = nt::NetworkTableInstance::GetDefault();
  auto table = inst.GetTable("datatable");
  DTW = table->GetEntry("DTW");
 
  //frc::SmartDashboard::PutData("Auto Modes", &m_chooser);
	ultra1->SetAutomaticMode(true); // Enable the Ultrasonic Sensor to always be active
	prefs = Preferences::GetInstance();
  CameraServer::GetInstance()->StartAutomaticCapture();

  try
	{
		// *********************************************************************** //
		// navX-MXP:
		// - Communication via RoboRIO MXP (SPI, I2C, TTL UART) and USB.
		// - See http://navx-mxp.kauailabs.com/guidance/selecting-an-interface.
		//
		// navX-Micro:
		// - Communication via I2C (RoboRIO MXP or Onboard) and USB.
		// - See http://navx-micro.kauailabs.com/guidance/selecting-an-interface.
		//
		// Multiple navX-model devices on a single robot are supported.
		// *********************************************************************** //
		ahrs = new AHRS(SPI::Port::kMXP);
	}
	catch (std::exception& ex)
	{
		std::string err_string = "Error instantiating navX MXP:  ";
		err_string += ex.what();
		DriverStation::ReportError(err_string.c_str());
	}
	if ( ahrs )
	{
//		LiveWindow::GetInstance()->AddSensor("IMU", "Gyro", ahrs);
	}
}

/**
 * This function is called every robot packet, no matter the mode. Use
 * this for items like diagnostics that you want ran during disabled,
 * autonomous, teleoperated and test.
 *
 * <p> This runs after the mode specific periodic functions, but before
 * LiveWindow and SmartDashboard integrated updating.
 */
void Robot::RobotPeriodic() {}

/**
 * This autonomous (along with the chooser code above) shows how to select
 * between different autonomous modes using the dashboard. The sendable chooser
 * code works with the Java SmartDashboard. If you prefer the LabVIEW Dashboard,
 * remove all of the chooser code and uncomment the GetString line to get the
 * auto name from the text box below the Gyro.
 *
 * You can add additional auto modes by adding additional comparisons to the
 * if-else structure below with additional strings. If using the SendableChooser
 * make sure to add them to the chooser code above as well.
 */
void Robot::AutonomousInit() {
  m_autoSelected = m_chooser.GetSelected();
  // m_autoSelected = SmartDashboard::GetString("Auto Selector",
  //     kAutoNameDefault);
  std::cout << "Auto selected: " << m_autoSelected << std::endl;

  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
  } else {
    // Default Auto goes here
  }

  ahrs->Reset(); // zero out the NavX Yaw gyro so that we can start with the front of the robot at 0 degrees
	yaw = 0;  // set the current yaw angle to the yaw variable.  We will use this to measure change between code executions / loops
	firstinit = false;
	secondinit = false;
	thirdinit = false;
}

void Robot::AutonomousPeriodic() {
  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
  } else {
    // Default Auto goes here
  }

  Mecanum(0, -.2, 0);
	Wait(2000);
	Mecanum(0, 0, 0);

	// Find the current yaw angle and check it to see if we need to change our rotation
	float yawrate = ahrs->GetRawGyroZ(); // This function returns the rate of change of yaw in degrees/second
	float yawnew = ahrs->GetYaw();  // This is the current yaw setting.  We will use this to compare with the existing setting to see if we need to make a change
	float yawdiff = yawnew - yaw;

	float yawadjust = yawrate / 180 * changefactor;
	float normyawdiff = yawdiff / 180 * degreechangefactor;
	yawadjust = yawadjust + normyawdiff;

	if (yawadjust < -.15) yawadjust = -.15;
	if (yawadjust > .15) yawadjust = .15;

	if(!firstinit)
	{
		int gap = UltraDrive(20);
		if(gap < 3)
		{
			firstinit = true;
			starttimeAuto = std::chrono::high_resolution_clock::now();
			ahrs->Reset(); // zero out the NavX Yaw gyro so that we can start with the front of the robot at 0 degrees
			yaw = 0;  // set the current yaw angle to the yaw variable.  We will use this to measure change between code executions / loops
		}
	}
	if(!secondinit && firstinit)
	{
		// Get the current time and compare it to the start time so that we can see how long we have been in Autonomous
		currenttimeAuto = std::chrono::high_resolution_clock::now();
		milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(currenttimeAuto-starttimeAuto);
		int difftime = duration.count();

		yaw = 90;

		if (difftime > 2500)
		{
			secondinit = true;
			yaw = 0;  // set the current yaw angle to the yaw variable.  We will use this to measure change between code executions / loops
			ahrs->Reset(); // zero out the NavX Yaw gyro so that we can start with the front of the robot at 0 degrees
			yawadjust = 0;
		}

		Mecanum(yawadjust, 0, 0);
	}

	if(!thirdinit && secondinit && firstinit)
	{
		int gap = UltraDrive(20);
		if(gap < 3)
		{
			thirdinit = true;
		}
	}
}

void Robot::TeleopInit() {
 	ahrs->Reset(); // zero out the NavX Yaw gyro so that we can start with the front of the robot at 0 degrees
	yaw = 0;  // set the current yaw angle to the yaw variable.  We will use this to measure change between code executions / loops
}

void Robot::TeleopPeriodic() {

  int inchWall = frc::SmartDashboard::GetNumber("Desired_Distance_to_wall", 20); 
  bool tokyodrift = frc::SmartDashboard::GetBoolean("Tokyo_Drift", false);
  frc::SmartDashboard::PutBoolean("Tokyo_Drift", tokyodrift);
  frc::SmartDashboard::PutNumber("UltraSound_Front", ultra1->GetRangeInches());
  frc::SmartDashboard::PutNumber("Desired_Distance_to_wall", inchWall);
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
  float FWandBackStick = JoyA->GetRawAxis(1) * -1; // right joystick y axis
  float LEFTandRIGHTStick = JoyA->GetRawAxis(0) * -1; // right joystick x axis

  // Use UltraSound Ranging to get to a specific location - Overrides the Forward and Back Stick Control
  if(JoyA->GetRawButton(7))
  {
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
    FWandBackStick = -1*((float)inchWall-(float)range)/(float)rangecomp*(float)rangecompclose;
    if(FWandBackStick > .3)
      FWandBackStick = .3;
    if(FWandBackStick < -.3)
      FWandBackStick = -.3;
    if(gap < 30 && FWandBackStick > .15)
      FWandBackStick = .15;
    if(gap < 30 && FWandBackStick < -.15)
      FWandBackStick = -.15;

    TurnStick = yawadjust;

    //frc::SmartDashboard::PutNumber("InchWall", inchWall);
    //frc::SmartDashboard::PutNumber("Range", range);
    //frc::SmartDashboard::PutNumber("Calculation", (((float)inchWall - (float)range)/(float)rangecomp));
    //frc::SmartDashboard::PutNumber("FWandBackStick", FWandBackStick);
    //frc::SmartDashboard::PutNumber("Gap", gap);
    //frc::SmartDashboard::PutNumber("RangeComp", rangecomp);
  }


  if (!tokyodrift)
  {
    if(TurnStick < .1 && TurnStick > -.1)
      TurnStick = yawadjust;

    if(TurnStick > .1 || TurnStick < -.1)
    {
      ahrs->Reset();
      yaw = 0;
    }
  }

  cout << FWandBackStick << endl;
  Mecanum(TurnStick, LEFTandRIGHTStick, FWandBackStick);
}

void Robot::TestPeriodic() {
  double speed = 0.1;
  double targetAngle = 90;
  double currentAngle = ahrs->GetYaw();
  drive->RotateToAngle(speed, targetAngle, currentAngle);
}
  
#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
