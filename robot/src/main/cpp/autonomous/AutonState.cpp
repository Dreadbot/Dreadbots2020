#include "autonomous/AutonState.h"

AutonState::AutonState()
    : is_done( false )
{}

void AutonState::AutonomousInit()
{}

void AutonState::AutonomousPeriodic()
{}

void AutonState::CheckState()
{
  
}

bool AutonState::IsDone()
{
  return this->is_done;
}

void AutonState::SetDone(bool is_done_)
{
  this->is_done = is_done_;
}