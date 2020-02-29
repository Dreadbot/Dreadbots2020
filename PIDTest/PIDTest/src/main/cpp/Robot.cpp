/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <frc/Encoder.h>
#include <frc/Joystick.h>
#include <frc/PWMVictorSPX.h>
#include <frc/TimedRobot.h>
#include <frc/controller/ProfiledPIDController.h>
#include <frc/trajectory/TrapezoidProfile.h>
#include <units/units.h>
#include <wpi/math>
#include "Intake.h"

class Robot : public frc::TimedRobot {
 public:
  static constexpr units::second_t kDt = 20_ms;

  Robot() {
    //m_encoder.SetDistancePerPulse(1.0 / 360.0 * 2.0 * wpi::math::pi * 1.5);

  }

  void TeleopInit() override {
    m_intake = new Intake(1);
  }

  void TeleopPeriodic() override {
    if (abs(m_joystick.GetRawAxis(1)) > .25) {
      m_intake.SetSpeed(5000);
    } else {
      m_intake.Stop();
    }
  }

 private:
  frc::Joystick m_joystick{1};
  frc::Encoder m_encoder{1, 2};
  frc::PWMVictorSPX m_motor{1};
  Intake m_intake;

  // Create a PID controller whose setpoint's change is subject to maximum
  // velocity and acceleration constraints.
  frc::TrapezoidProfile<units::meters>::Constraints m_constraints{1.75_mps,
                                                                  0.75_mps_sq};
  frc::ProfiledPIDController<units::meters> m_controller{1.3, 0.0, 0.7,
                                                         m_constraints, kDt};
};

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
