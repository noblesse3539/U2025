#include "Squads/CSquadAsset.h"
#include "Global.h"

FSquadType UCSquadAsset::GetSquadTypes(ESquadKey key)
{
    CheckFalseResult(SquadTypes.Contains(key), FSquadType());

    return SquadTypes[key];
}
