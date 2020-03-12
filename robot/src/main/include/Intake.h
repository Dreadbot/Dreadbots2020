#pragma once

#include <rev/CANSparkMax.h>
#include <frc/Solenoid.h>

class Intake
{
 public:
  Intake(rev::CANSparkMax *intake_motor, frc::Solenoid *intake_pin, frc::Solenoid *intake_arms);
  void SetSpeed(double speed);
  void SetPercentOutput(double percent_output);
  void DeployIntake();
  void SetIntakeArms(bool value);
  void Start();
  void Stop();
 private:
  rev::CANSparkMax *m_intake_motor;
  rev::CANPIDController m_pidController;
  frc::Solenoid *m_intake_pin;
  frc::Solenoid *m_intake_arms;
  bool running;
};