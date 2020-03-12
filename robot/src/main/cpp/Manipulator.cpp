#include <Manipulator.h>

Manipulator::Manipulator(Intake *intake, Feeder *feeder, Shooter *shooter)
  :  m_intake(intake), m_feeder(feeder), m_shooter(shooter)
{
  shooter_state = kRamping;
  num_punches = 0;
}

void Manipulator::PrepareShot(int rpm, double aimPosition)
{
  m_shooter->Shoot(-rpm);
  m_shooter->SetAdjusterPosition(aimPosition);
}

int Manipulator::GetSelectedRPM(double inches)
{
  inches /= 12;
  return ((-0.0029 * inches * inches) + (0.188026 * inches) + 1.7676)*1000.;
}

double Manipulator::GetSelectedHoodPosition(double inches)
{
  inches /= 12;
  return ((-0.0941 * inches * inches) + (4.96271 * inches) + 2.08)/100.;
}

void Manipulator::ContinuousShoot(double aim_position, double geneva_speed, int shooting_rpm)
{
  frc::SmartDashboard::PutNumber("GetShootingSpeed(): ", -m_shooter->GetShootingSpeed());
  frc::SmartDashboard::PutNumber("ShooterState: ", shooter_state);
  int speedDifference = abs(m_shooter->GetShootingSpeed()) - shooting_rpm;

  //Finite State Machine logic to switch between states
  //Change state based on a speed requirement of the flywheel
  if(shooter_state == kRamping && speedDifference < 300 && speedDifference > 0)
  {
    shooter_state = kPunching;
  }
  else if(shooter_state == kRamping && speedDifference > -100 && speedDifference < 0)
  {
    shooter_state = kPunching;
  }
  //Change state based on a counter so that the solenoid has time to extend
  else if(shooter_state == kPunching && state_change_counter > kCountsToExtend)
  {
    shooter_state = kRetracting;
    state_change_counter = 0;
  }

  //Change state based on a counter so that the solenoid has time to retract
  else if(shooter_state == kRetracting && m_feeder->GetPunchSwitchState())
  {
    shooter_state = kAdvance;
    state_change_counter = 0;
  }
  //Change state if the geneva drive has rotated away from the limit switch
  else if(shooter_state == kAdvance && !m_feeder->GetGenevaSwitchState())
  {
    shooter_state = kAdvancing;
  }
  //Change state back to the start once the geneva drive has made it back to the limit switch
  else if(shooter_state == kAdvancing && m_feeder->GetGenevaSwitchState())
  {
    shooter_state = kRamping;
  }

  //Choose behavior based on the Finite State Machine
  switch (shooter_state){
    case kRamping:
      m_feeder->SetSpin(0);//Turn off the geneva drive
    break;
    case kPunching:
      m_feeder->SetPunchExtension(true);//Extend the punch
      state_change_counter++;
    break;
    case kRetracting:
      if(last_shooter_state != kRetracting)
      {
        ++num_punches;//The first time you encounter this each loop, increment this var
      }
      m_feeder->SetPunchExtension(false);//Retract the punch
    break;
    case kAdvance:
      //Fall through intended
    case kAdvancing:
      m_feeder->SetSpin(geneva_speed);//Turn the motor on
    break;
  }
    
    //Set the position of the aim plate and always drive the flywheel
    m_shooter->SetAdjusterPosition(aim_position);
    m_shooter->Shoot(-shooting_rpm);
    m_shooter->SetVisionLight(true);
    last_shooter_state = shooter_state;
}

int Manipulator::GetNumPunches()
{
    return this->num_punches;
}

void Manipulator::ResetManipulatorElements()
{
  //This function should be called continuously if the system is not shooting power cells or collecting power cells
  //The function will get the system back into a state where the punch is retracted and the geneva drive is lined up
  //in order to prepare for more shooting

  //If the punch is extended, retract it
  if(m_feeder->GetPunchExtension()){
    m_feeder->SetPunchExtension(false);
  }

 //Once the geneva drive reaches a limit switch, stop it
  else if(m_feeder->GetGenevaSwitchState())
  {
    m_feeder->SetSpin(0);
  }

  //Reset variables
  shooter_state = kRamping;
  m_shooter->SetShootingPercentOutput(0);
  num_punches = 0;
  m_shooter->SetVisionLight(true);
  if(m_shooter->GetAimReadiness())
  {
    m_shooter->SetAdjusterPosition(0);
  }
}

void Manipulator::SensorAdvanceGeneva(bool spin, bool forward)
{
  double genevaSpeed = 0.4;
  if(genevaState == kStopped && spin){
    if(forward)
    {
      m_feeder->SetSpin(-genevaSpeed);
      genevaDirection = kForward;
    }
    else
    {
      m_feeder->SetSpin(genevaSpeed);
      genevaDirection = kBackward;
    }
    genevaState = kMove;
  }

  else if(genevaState == kMove && !m_feeder->GetGenevaSwitchState())
  {
    genevaState = kMoving;
  }

  else if(genevaState == kMoving && m_feeder->GetGenevaSwitchState())
  {
    m_feeder->SetSpin(0);
    genevaState = kStopped;
  }

  if(genevaState == kMove || genevaState == kMoving){
    if(genevaDirection == kForward)
    {
      m_feeder->SetSpin(-genevaSpeed);
    }
    else
    {
      m_feeder->SetSpin(genevaSpeed);
    }
  }
  frc::SmartDashboard::PutNumber("camNumber", 1);
}

void Manipulator::GenevaSetSpin(double power)
{
  m_feeder->SetSpin(power);
}

int Manipulator::GetSensorAdvanceGenevaState()
{
  return m_feeder->GetSensorAdvanceGenevaState();
}

Intake* Manipulator::GetIntake()
{
  return m_intake;
}

Feeder* Manipulator::GetFeeder()
{
  return m_feeder;
}

Shooter* Manipulator::GetShooter()
{
    return m_shooter;
}