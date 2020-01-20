/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#define _USE_MATH_DEFINES

#include <math.h>

/**
 * Originally developed by Team 254.
 * From the Cheesy Poof's Trajectory Lib
 * 
 * Ported for C++ Trajectory Following
 * 
 * https://github.com/Team254/TrajectoryLib/blob/master/src/com/team254/lib/util/ChezyMath.java
 */ 
class TrajectoryMath
{
 public:
  /**
   * Empty Constructor
   */
  TrajectoryMath();
  
  /**
   * Implementation of atan
   * 
   * https://github.com/Team254/TrajectoryLib/blob/5847e12f484bee8a1ea765adb0392440b317b2bd/src/com/team254/lib/util/ChezyMath.java#L47
   */
  static double atan(double arg)
  {
    if (arg > 0) {
      return msatan(arg);
    }
    return -msatan(-arg);
  }
  
  /**
   * Implementation of atan2
   * 
   * https://github.com/Team254/TrajectoryLib/blob/5847e12f484bee8a1ea765adb0392440b317b2bd/src/com/team254/lib/util/ChezyMath.java#L55
   */
  static double atan2(double arg1, double arg2)
  {
    if (arg1 + arg2 == arg1) {
      if (arg1 >= 0) {
        return PIO2;
      }
      return -PIO2;
    }
    arg1 = atan(arg1 / arg2);
    if (arg2 < 0) {
      if (arg1 <= 0) {
        return arg1 + M_PI;
      }
      return arg1 - M_PI;
    }
    return arg1;
  }

  /**
   * Implementation of asin
   * 
   * https://github.com/Team254/TrajectoryLib/blob/5847e12f484bee8a1ea765adb0392440b317b2bd/src/com/team254/lib/util/ChezyMath.java#L74
   */
  static double asin(double arg)
  {
    double temp;
    int sign;

    sign = 0;
    if (arg < 0) {
      arg = -arg;
      sign++;
    }
    if (arg > 1) {
      return nan;
    }
    temp = sqrt(1 - arg * arg);
    if (arg > 0.7) {
      temp = PIO2 - atan(temp / arg);
    } else {
      temp = atan(arg / temp);
    }
    if (sign > 0) {
      temp = -temp;
    }
    return temp;
  }

  /**
   * Implementation of acos
   * 
   * https://github.com/Team254/TrajectoryLib/blob/5847e12f484bee8a1ea765adb0392440b317b2bd/src/com/team254/lib/util/ChezyMath.java#L99
   */
  static double acos(double arg)
  {
    if (arg > 1 || arg < -1) {
      return nan;
    }
    return PIO2 - asin(arg);
  }

  /**
   * Get the difference in angle between two angles.
   * 
   * @param from The first angle
   * @param to The second angle
   * @return The change in angle from the first argument necessary to line up with the second. Always between -Pi and Pi
   * 
   * https://github.com/Team254/TrajectoryLib/blob/5847e12f484bee8a1ea765adb0392440b317b2bd/src/com/team254/lib/util/ChezyMath.java#L114
   */
  static double GetDifferenceInAngleRadians(double from, double to)
  {
    return BoundAngleNegativePiToPiRadians(to - from);
  }

  /**
   * Get the difference in angle between two angles.
   * 
   * @param from The first angle
   * @param to The second angle
   * @return The change in angle from the first argument necessary to line up with the second. Always between -180 and 180
   * 
   * https://github.com/Team254/TrajectoryLib/blob/5847e12f484bee8a1ea765adb0392440b317b2bd/src/com/team254/lib/util/ChezyMath.java#L114
   */
  static double GetDifferenceInAngleDegrees(double from, double to)
  {
    return BoundAngleNegative180To180Degrees(to - from);
  }

  /**
   * Bounds an Angle within the 0 - 360 degree restriction for easier calculation
   * 
   * https://github.com/Team254/TrajectoryLib/blob/5847e12f484bee8a1ea765adb0392440b317b2bd/src/com/team254/lib/util/ChezyMath.java#L130
   */
  static double BoundAngle0to360Degrees(double angle)
  {
    while (angle >= 360.0) {
      angle -= 360.0;
    }
    while (angle < 0.0) {
      angle += 360.0;
    }
    return angle;
  }

  /**
   * Bounds an Angle within the 180 - (-180) degree restriction for easier calculation
   * 
   * https://github.com/Team254/TrajectoryLib/blob/5847e12f484bee8a1ea765adb0392440b317b2bd/src/com/team254/lib/util/ChezyMath.java#L141
   */
  static double BoundAngleNegative180To180Degrees(double angle)
  {
    while (angle >= 180.0) {
      angle -= 360.0;
    }
    while (angle < -180.0) {
      angle += 360.0;
    }
    return angle;
  }

  /**
   * Bounds an Angle within the 0 - 2M_PI degree restriction for easier calculation
   * 
   * https://github.com/Team254/TrajectoryLib/blob/5847e12f484bee8a1ea765adb0392440b317b2bd/src/com/team254/lib/util/ChezyMath.java#L141
   */
  static double BoundAngle0to2PiRadians(double angle)
  {
    while (angle >= 2.0 * M_PI) {
      angle -= 2.0 * M_PI;
    }
    while (angle < 0.0) {
      angle += 2.0 * M_PI;
    }
    return angle;
  }

  /**
   * Bounds an Angle within the M_PI - (-M_PI) degree restriction for easier calculation
   * 
   * https://github.com/Team254/TrajectoryLib/blob/5847e12f484bee8a1ea765adb0392440b317b2bd/src/com/team254/lib/util/ChezyMath.java#L141
   */
  static double BoundAngleNegativePiToPiRadians(double angle)
  {
    while (angle >= M_PI) {
      angle -= 2.0 * M_PI;
    }
    while (angle < -M_PI) {
      angle += 2.0 * M_PI;
    }
    return angle;
  }

 private:
  // Constants for Trig Math
  static const double sq2p1 = 2.414213562373095048802e0;
  static const double sq2m1 = .414213562373095048802e0;
  static const double p4 = .161536412982230228262e2;
  static const double p3 = .26842548195503973794141e3;
  static const double p2 = .11530293515404850115428136e4;
  static const double p1 = .178040631643319697105464587e4;
  static const double p0 = .89678597403663861959987488e3;
  static const double q4 = .5895697050844462222791e2;
  static const double q3 = .536265374031215315104235e3;
  static const double q2 = .16667838148816337184521798e4;
  static const double q1 = .207933497444540981287275926e4;
  static const double q0 = .89678597403663861962481162e3;
  static const double PIO2 = 1.5707963267948966135E0;
  static const double nan = (0.0 / 0.0);
  
  /**
   * Implementation of an Argument Reduction Function
   * 
   * https://github.com/Team254/TrajectoryLib/blob/5847e12f484bee8a1ea765adb0392440b317b2bd/src/com/team254/lib/util/ChezyMath.java#L26
   */
  static double mxatan(double arg)
  {
    double argsq, value;

    argsq = arg * arg;
    value = ((((p4 * argsq + p3) * argsq + p2) * argsq + p1) * argsq + p0);
    value = value / (((((argsq + q4) * argsq + q3) * argsq + q2) * argsq + q1) * argsq + q0);
    return value * arg;
  }

  /**
   * Implementation of an Argument Reduction Function
   * 
   * https://github.com/Team254/TrajectoryLib/blob/5847e12f484bee8a1ea765adb0392440b317b2bd/src/com/team254/lib/util/ChezyMath.java#L36
   */
  static double msatan(double arg)
  {
    if (arg < sq2m1) {
      return mxatan(arg);
    }
    if (arg > sq2p1) {
      return PIO2 - mxatan(1 / arg);
    }
    return PIO2 / 2 + mxatan((arg - 1) / (arg + 1));
  }
};