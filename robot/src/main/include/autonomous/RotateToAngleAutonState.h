#include "autonomous/AutonState.h"

#pragma once

class RotateToAngleAutonState : AutonState
{
 public:
  RotateToAngleAutonState();

  void AutonomousInit() override;
  void AutonomousPeriodic() override;
  void CheckState() override;
};