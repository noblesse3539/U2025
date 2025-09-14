#include "CGameMode.h"
#include "Global.h"

ACGameMode::ACGameMode()
{
	FHelpers::GetClass<APawn>(&DefaultPawnClass, "/Script/Engine.Blueprint'/Game/Player/BP_CPlayer.BP_CPlayer_C'");
}
