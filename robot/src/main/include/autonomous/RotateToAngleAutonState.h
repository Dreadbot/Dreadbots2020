#include "autonomous/AutonState.h"

#include "TeleopFunctions.h"

#pragma once

class RotateToAngleAutonState : public AutonState
{
 public:
  RotateToAngleAutonState(TeleopFunctions* teleop_functions_, double angle_);

  void AutonomousInit() override;
  void AutonomousPeriodic() override;
  void CheckState() override;

 private:
  TeleopFunctions* teleop_functions;

  double angle;
};