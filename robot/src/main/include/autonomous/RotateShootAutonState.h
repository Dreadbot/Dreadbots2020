#pragma once

#include "Manipulator.h"
#include "AutonState.h"

class RotateShootAutonState : public AutonState
{
 public:
  RotateShootAutonState(
    Manipulator* manipulator_);

  void AutonomousInit() override;
  void AutonomousPeriodic() override;
  void CheckState() override;

 private:
  Manipulator* manipulator;
};