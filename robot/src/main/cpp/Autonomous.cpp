#include "Autonomous.h"

Autonomous::Autonomous(frc::Timer* timer_, SparkDrive *spark_drive_)
{
  timer = timer_;
  spark_drive = spark_drive_;

  // Assigned Later
  // (Autonomous Routine Decided at Beginning
  // of Autonomous Period, not Starting of
  // Robot)
  time_state_map = nullptr;
}

void Autonomous::AutonomousInit(std::multimap<units::second_t, AutonState>* time_state_map_)
{
  // Definition of Autonomous Routine
  // (Autonomous Routine Decided at Beginning
  // of Autonomous Period, not Starting of
  // Robot)
  time_state_map = time_state_map_;

  // Start the Autonomous Timer
  timer->Start();
}

void Autonomous::AutonomousPeriodic()
{
  // Get the current state from the current time
  current_state = GetAutonStateFromCurrentTime();

  // Switch Statement to Differ Behavior for each
  // AutonState.
  switch(current_state)
  {
  case intake:
    break;
  case feeder:
    break;
  case shooter:
    break;
  case rotate_to_angle:
    break;
  case trajectory_following:
    break;
  }
}

AutonState Autonomous::GetAutonStateFromCurrentTime()
{
  // Get the current time in seconds
  current_time = units::time::second_t(timer->Get());

  // Iterate through map
  for(auto iterator = time_state_map->begin(); 
      iterator != time_state_map->end(); 
      iterator++)
  {
    // Get the Key of the Current Hash
    const auto this_time = units::time::second_t(iterator->first());

    // Iteratively search for the current activity.
    if (this_time < current_time)
    {
      lower_bound_time = this_time;
    }

    // Delete this_time to avoid unecessary memory leaks
    delete &this_time;
  }

  // Return the current state in the autonomous period
  return time_state_map->find(lower_bound_time)->second;
}