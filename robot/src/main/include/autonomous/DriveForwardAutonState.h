#include "autonomous/AutonState.h"

#include "SparkDrive.h"

class DriveForwardAutonState : public AutonState
{
 public:
  DriveForwardAutonState(
    SparkDrive* spark_drive_, 
    double encoder_determinant_);

  void AutonomousInit() override;
  void AutonomousPeriodic() override;
  void CheckState() override;

 private:
  SparkDrive* spark_drive;

  double encoder_determinant;

  double left_front_encoder_zero_value;
};