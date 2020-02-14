#pragma once

#include "SparkDrive.h"

class Autonomous
{
 public:
  Autonomous(SparkDrive *Sparkdrive);
  
  void RightRight();
  void RightCenter();
  void RightLeft();

 private:
 SparkDrive *m_SparkDrive;
};