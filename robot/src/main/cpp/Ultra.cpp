#include "Ultra.h"

Ultra::Ultra()
{
  m_UltraGen->SetAutomaticMode(true);
  m_UltraGen2->SetAutomaticMode(true);
}

float Ultra::GetDistanceGen()
{
  return m_UltraGen->GetDistanceUnits();
}

float Ultra::GetDistanceGen2()
{
  return m_UltraGen2->GetDistanceUnits();
}