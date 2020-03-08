#pragma once

class AutonState
{
 public:
  AutonState();

  virtual void AutonomousInit();
  virtual void AutonomousPeriodic();
  
  virtual void CheckState();

  bool IsDone();
  void SetDone(bool is_done_);

 private:
  bool is_done;
};