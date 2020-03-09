/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "frc/util/color.h"

enum JoystickInputs 
{ 
  x_axis = 0, y_axis = 1, z_axis = 2, w_axis = 3,
  x_button = 1, a_button = 2, b_button = 3, y_button = 4,
  left_bumper = 5, right_bumper = 6, left_trigger = 7, right_trigger = 8,
  back_button = 9, start_button = 10, l_trigger = 11, r_trigger = 12
};
enum Joystick1Layout{
    kForwardBackwardAxis = y_axis, kRotAxis = z_axis, kExtendClimbButton = start_button, 
    kRetractClimbButton = back_button, kWinchButton = x_button, kTurtleButton = right_bumper, 
    kTurboButton = right_trigger
}; 
enum Joystick2Layout{
    kAimShootButton = b_button, kIntakeButton = x_button, kOuttakeButton = a_button, kShootButton = y_button,
    kRegressGenevaButton = left_bumper, kAdvanceGenevaButton = right_bumper, kRetractColorWheelButton = left_trigger,
    kDeployColorWheelButton = right_trigger, kColorWheelColorControl = start_button, kColorWheelRotationControl = back_button, 
    kIncreaseAimOffsetPOV = 270, kDecreaseAimOffsetPOV = 90, kCameraSwitchPOV = 0
};
enum CameraView{
  kVisionCam, kGenevaCam, kIntakeCam, kDriveCam
};

const frc::Color kBlueTarget = frc::Color(0.143, 0.427, 0.429);
const frc::Color kGreenTarget = frc::Color(0.197, 0.561, 0.240);
const frc::Color kRedTarget = frc::Color(0.561, 0.232, 0.114);
const frc::Color kYellowTarget = frc::Color(0.361, 0.524, 0.113);

const int kPrimaryDriverJoystickID = 0;
const int kSecondaryDriverJoystickID = 1;

const int kUltraLeftFrontMotorID = 1;
const int kUltraRightFrontMotorID = 2;
const int kUltraLeftBackMotorID = 3;
const int kUltraRightBackMotorID = 4;

const int kBigSlinkLeftFrontMotorID = 1;
const int kBigSlinkRightFrontMotorID = 2;
const int kBigSlinkLeftBackMotorID = 3;
const int kBigSlinkRightBackMotorID = 4;

const int kIntakeMotorID = 5;
const int kGenevaMotorID = 6;
const int kFlyWheelMotorID = 7;
const int kAimMotorID = 8;
const int kColorWheelMotorID = 9;
const int kClimbWinchMotorID = 10; //10 on comp bot 11 on practice

const int kIntakePinID = 0;
const int kColorWheelSolenoidID = 1;

const int kPunchSolenoidID = 2;
const int kClimbTelescopeSolenoidID = 3; //4 on practice, 3 on comp

