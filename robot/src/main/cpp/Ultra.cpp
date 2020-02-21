#include "Ultra.h"

Ultra::Ultra()
{
    m_UltraGen->SetAutomaticMode(true);
}

float Ultra::GetDistanceGen()
{
    return m_UltraGen->GetDistanceUnits();
}