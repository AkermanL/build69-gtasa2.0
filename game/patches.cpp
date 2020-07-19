#include "../main.h"
#include "../util/armhook.h"
#include "common.h"

char* PLAYERS_REALLOC = nullptr;

struct _ATOMIC_MODEL
{
	uintptr_t func_tbl;
	char data[56];
};
struct _ATOMIC_MODEL *ATOMIC_MODELS;

void ApplyGlobalPatches()
{
	  FLog("ApplyGlobalPatches");

    PLAYERS_REALLOC = (( char* (*)(int))(g_GTASAAdr + 0x198A70))(404*1004);
    ARMHook::unprotect(g_GTASAAdr + 0x6783C8);
    *(char**)(g_GTASAAdr + 0x6783C8) = PLAYERS_REALLOC;

    ARMHook::writeMemory(g_GTASAAdr + 0x3F3648, (uintptr_t)"\x06\x20", 2); // CGame::InitialiseOnceBeforeRW

    ARMHook::makeNOP(g_GTASAAdr + 0x56C150, 4); // CBoat::ProcessControl

    ARMHook::makeNOP(g_GTASAAdr + 0x3FCD34, 2); // CRealTimeShadowManager::ReturnRealTimeShadow ~ CPhysical
    ARMHook::writeMemory(g_GTASAAdr + 0x408AE6, (uintptr_t)"\x4F\xF4\x7A\x61\x00", 4); // CPlaceable::InitMatrixArray

    ARMHook::unprotect(g_GTASAAdr + 0x41C33C); // CStats::GetFatAndMuscleModifier
    *(uint32_t *)(g_GTASAAdr + 0x41C33C) = 0x43300000;

    ARMHook::unprotect(g_GTASAAdr + 0x2BD94C); // CWidgetPlayerInfo::RenderArmorBar
    *(uint32_t *)(g_GTASAAdr + 0x2BD94C) = 0x43300000; 

    ARMHook::makeNOP(g_GTASAAdr + 0x3056D6, 4); // CEntryExit::GenerateAmbientPeds
    ARMHook::makeNOP(g_GTASAAdr + 0x43FE0A, 2); // CSprite2d::Draw
    ARMHook::makeNOP(g_GTASAAdr + 0x44095E, 2); // CSprite2d::Draw
    ARMHook::makeNOP(g_GTASAAdr + 0x5D87A6, 2); // CCamera::CamShake
    ARMHook::makeNOP(g_GTASAAdr + 0x5D8734, 2); // CCamera::CamShake
    // --------------------------------------------------------------------------------------------------

    ATOMIC_MODELS = new _ATOMIC_MODEL[20000];
    for (int i = 0; i < 20000; i++)
    {
      // CBaseModelInfo::CBaseModelInfo
      ((void(*)(_ATOMIC_MODEL*))(g_GTASAAdr + 0x384F89))(&ATOMIC_MODELS[i]);
      ATOMIC_MODELS[i].func_tbl = g_GTASAAdr + 0x667454;
      memset(ATOMIC_MODELS[i].data, 0, sizeof(ATOMIC_MODELS->data));
    }
	
  	// CAudioEngine::StartLoadingTune
  	ARMHook::makeNOP(g_GTASAAdr + 0x5E4916, 2);
	 
    // ~
  	ARMHook::makeRET(g_GTASAAdr + 0x3976AC);	// CAEGlobalWeaponAudioEntity::ServiceAmbientGunFire
  	ARMHook::makeRET(g_GTASAAdr + 0x4211A0);	// CPlaceName::Process
  	ARMHook::makeRET(g_GTASAAdr + 0x538C8C);	// CTaskSimplePlayerOnFoot::PlayIdleAnimations
  	ARMHook::makeRET(g_GTASAAdr + 0x50AA58);	// CCarEnterExit::SetPedInCarDirect
  	   
    // generator
    ARMHook::makeRET(g_GTASAAdr + 0x56E350);  // CTheCarGenerators::Process
    ARMHook::makeRET(g_GTASAAdr + 0x4CF26C);  // CPopulation::AddPed
  	ARMHook::makeRET(g_GTASAAdr + 0x2E82CC);	// CCarCtrl::GenerateRandomCars
    ARMHook::makeRET(g_GTASAAdr + 0x579214);  // CPlane::DoPlaneGenerationAndRemoval
    ARMHook::makeRET(g_GTASAAdr + 0x46B548);  // CFileLoader::LoadPickup
    ARMHook::makeRET(g_GTASAAdr + 0x306EC0);  // CEntryExit::GenerateAmbientPeds

    // hud patch
    ARMHook::makeRET(g_GTASAAdr + 0x436FCC);   // CHud::SetHelpMessageStatUpdate
    ARMHook::makeRET(g_GTASAAdr + 0x438634);  // CHud::DrawVehicleName
    ARMHook::makeRET(g_GTASAAdr + 0x436F5C); // CHud::SetHelpMessage

    // shadow patch
  	ARMHook::makeNOP(g_GTASAAdr + 0x3FCD34, 2);	   // ReturnRealTimeShadow
  	ARMHook::makeNOP(g_GTASAAdr + 0x3FCD74, 2);	  // ReturnRealTimeShadow
  	ARMHook::makeRET(g_GTASAAdr + 0x5B83FC);	   // CRealTimeShadowManager::Update

    // CPed::RemoveWeaponWhenEnteringVehicle
  	ARMHook::makeNOP(g_GTASAAdr + 0x4A5328, 6);		
  
  	// fps debug
  	*(uint8_t *)(g_GTASAAdr + 0x98F1AD) = 1;

    // DefaultPCSaveFileName
    ARMHook::unprotect(g_GTASAAdr+0x6B0130);
    memcpy((void*)(g_GTASAAdr+0x6B0130), "GTASAMP", 8);

    // FPS
    ARMHook::writeMemory(g_GTASAAdr + 0x5E4978, (uintptr_t)"\x64", 1); // \x64
    ARMHook::writeMemory(g_GTASAAdr + 0x5E4990, (uintptr_t)"\x64", 1); // \x64

    // CStreaming::ms_memoryAvailable patch
    ARMHook::unprotect(g_GTASAAdr+0x46BE0A);
    *(uintptr_t*)(g_GTASAAdr+0x46BE0A) = 0x10000000;
    ARMHook::unprotect(g_GTASAAdr+0x46BE10);
    *(uintptr_t*)(g_GTASAAdr+0x46BE10) = 0x10000000;

    // Draw distance hack
    ARMHook::unprotect(g_GTASAAdr+0x41F300);
    *(uint32_t *)(g_GTASAAdr+0x41F300) = 0x41C80000;
    ARMHook::unprotect(g_GTASAAdr+0x41FF6C);
    *(uint32_t *)(g_GTASAAdr+0x41FF6C) = 0x40A00000;

    // _rwOpenGLRasterCreate
    ARMHook::writeMemory(g_GTASAAdr + 0x1AE95E, (uintptr_t)"\x01\x22", 2);
}

void ApplySCAndPatches()
{
	FLog("ApplySCAndPatches");

	// SocialClub Patch #1
  	ARMHook::unprotect(g_SCANDAdr+0x31C149);
  	*(bool*)(g_SCANDAdr+0x31C149) = true;
}

void ApplySAMPPatchesInGame()
{
	FLog("ApplySAMPPatchesInGame");

  // CTheZones::ZonesVisited[100]
	memset((void*)(g_GTASAAdr + 0x98D252), 1, 100);

	// CTheZones::ZonesRevealed
	*(uint32_t*)(g_GTASAAdr + 0x98D2B8) = 100;
}

/*
all 2.0 patches

PLAYERS_REALLOC = ((int (__fastcall *)(_DWORD))(g_GTASAAdr + 0x198A70))(405616);
  unprotect(g_GTASAAdr + 0x6783C8);
  *(_DWORD *)(g_GTASAAdr + 0x6783C8) = PLAYERS_REALLOC;
  LOG("CWorld::Player new address = 0x%X", PLAYERS_REALLOC);
  return writeMem(g_GTASAAdr + 0x3F3648, &dword_14B40, 2);// 20 06 >> \x06\x20
*/

/*
 ((void (__fastcall *)(int, signed int))NOP)(g_GTASAAdr + 0x56C150, 4);
  v4 = g_GTASAAdr;
  *(_DWORD *)(g_GTASAAdr + 0x6B0130) = 5262657;
  *(_DWORD *)(v4 + 7012652) = 0x53415447;
  ((void (__fastcall *)(int, signed int))NOP)(g_GTASAAdr + 0x3FCD34, 2);
  return writeMem(g_GTASAAdr + 0x408AE6, dword_14BD4, 4);// 0x61 7A F4 4F, 0
                                                // 
                                                // \x4F\xF4\x7A\x61\x00
*/

/*
sub_E284("Installing patches (ingame)..");
  _aeabi_memset(g_GTASAAdr + 0x98D252, 100, 1);
  *(_DWORD *)(g_GTASAAdr + 0x98D2B8) = 100;
  unprotect(g_GTASAAdr + 0x41C33C);
  v0 = g_GTASAAdr;
  *(_DWORD *)(g_GTASAAdr + 0x41C33C) = 0x43300000;
  unprotect(v0 + 0x2BD94C);
  v1 = g_GTASAAdr;
  *(_DWORD *)(g_GTASAAdr + 0x2BD94C) = 0x43300000;
  unprotect(v1 + 0x3056D6);
  ((void (__fastcall *)(_DWORD, _DWORD))NOP)(g_GTASAAdr + 0x3056D6, 4);
  unprotect(g_GTASAAdr + 0x43FE0A);
  ((void (__fastcall *)(_DWORD, _DWORD))NOP)(g_GTASAAdr + 0x43FE0A, 2);
  unprotect(g_GTASAAdr + 0x44095E);
  ((void (__fastcall *)(_DWORD, _DWORD))NOP)(g_GTASAAdr + 0x44095E, 2);
  ((void (__fastcall *)(_DWORD, _DWORD))NOP)(g_GTASAAdr + 0x5D87A6, 2);
  return ((int (__fastcall *)(_DWORD, _DWORD))NOP)(g_GTASAAdr + 0x5D8734, 2);
*/

/* arz
    //ARMHook::unprotect(g_GTASAAdr + 0x6B0130);
    //ARMHook::unprotect(g_GTASAAdr + 0x6B012C);
    //*(uint32_t*)(g_GTASAAdr + 0x6B0130) = 0x504D41;
    //*(uint32_t*)(g_GTASAAdr + 0x6B012C) = 0x53415447;
  
    // ARMHook::makeNOP(g_GTASAAdr + 0x2A7258, 2);
  
    // ARMHook::writeMemory(g_GTASAAdr + 0x4C3673, (uintptr_t)"\xB3", 1); // \xB3
    // ARMHook::makeNOP(g_GTASAAdr + 0x40BED6, 2);  // CPlayerInfo::KillPlayer
// ARMHook::makeNOP(g_GTASAAdr + 0x4C5EFA, 2);  
    // ARMHook::makeRET(g_GTASAAdr + 0x445E98); // Interior_c::AddPickups
    // ARMHook::makeRET(g_GTASAAdr + 0x448984); // InteriorGroup_c::Exit

    // ARMHook::unprotect(g_GTASAAdr + 0x677654);
    // *(uint32_t *)(g_GTASAAdr + 0x677654) = 0;
    // ARMHook::writeMemory(g_GTASAAdr + 0x1AE95E, (uintptr_t)"\x01\x22", 2); // ?
  
    // ARMHook::makeNOP(g_GTASAAdr + 0x564CC0, 1); // BIKE AI?
    
    // markers
    // ARMHook::makeNOP(g_GTASAAdr + 0x43FE08, 3);
    // ARMHook::makeNOP(g_GTASAAdr + 0x44095C, 3);
    // ARMHook::makeNOP(g_GTASAAdr + 0x3F6C8C, 2);

    //ARMHook::writeMemory(g_GTASAAdr + 0x5E4978, (uintptr_t)"\x64", 1); // \x64
    //ARMHook::writeMemory(g_GTASAAdr + 0x5E4990, (uintptr_t)"\x64", 1); // \x64
//
    //ARMHook::writeMemory(g_GTASAAdr + 0x44212C, (uintptr_t)"\x30\x46", 2); // \x30\x46
    //ARMHook::writeMemory(g_GTASAAdr + 0x440470, (uintptr_t)"\x3A\xE0", 2); // \x3A\xE0 
    //ARMHook::writeMemory(g_GTASAAdr + 0x4404E8, (uintptr_t)"\x30\x46", 2); // \x30\x46
    //
    //ARMHook::writeMemory(g_GTASAAdr + 0x43FB36, (uintptr_t)"\x48\x46", 2); // \x48\x46 
    //ARMHook::writeMemory(g_GTASAAdr + 0x2AB556, (uintptr_t)"\x00\x21", 2); // \x00\x21
    //ARMHook::writeMemory(g_GTASAAdr + 0x40AC28, (uintptr_t)"\x8F\xF5\x3A\xEF\x00", 4); // \x8F\xF5\x3A\xEF\x00

    ARMHook::writeMemory(g_GTASAAdr + 0x44212C, (uintptr_t)"\x30\x46", 2); // \x30\x46
    ARMHook::writeMemory(g_GTASAAdr + 0x440470, (uintptr_t)"\x3A\xE0", 2); // \x3A\xE0 
    ARMHook::writeMemory(g_GTASAAdr + 0x4404E8, (uintptr_t)"\x30\x46", 2); // \x30\x46
    
    ARMHook::writeMemory(g_GTASAAdr + 0x43FB36, (uintptr_t)"\x48\x46", 2); // \x48\x46 
    ARMHook::writeMemory(g_GTASAAdr + 0x2AB556, (uintptr_t)"\x00\x21", 2); // \x00\x21
    // ARMHook::writeMemory(g_GTASAAdr + 0x40AC28, (uintptr_t)"\x8F\xF5\x3A\xEF\x00", 4); // \x8F\xF5\x3A\xEF\x00

    // ARMHook::makeNOP(g_GTASAAdr + 0x40AC30, 2);
    // ARMHook::makeRET(g_GTASAAdr + 0x40B028); // CPlayerInfo::FindObjectToSteal
    ARMHook::makeRET(g_GTASAAdr + 0x445E98);  // Interior_c::AddPickups
    ARMHook::makeRET(g_GTASAAdr + 0x448984);  // InteriorGroup_c::Exit

    // ARMHook::writeMemory(g_GTASAAdr + 0x4420D0, (unsigned int)sub_4FDE4, 2u, v29);
    // ARMHook::writeMemory(g_GTASAAdr + 0x43FB0E, (uintptr_t)"", 2);

  // reallocate CWorld::Players[]
    // WORLD_PLAYERS = new char[215880];  

  // ARMHook::unprotect(g_GTASAAdr+0x6783C8);
  // *(char**)(g_GTASAAdr+0x6783C8) = WORLD_PLAYERS;
  // FFLog("CWorld::Player new address = 0x%X", WORLD_PLAYERS);

  //ATOMIC_MODELS = new _ATOMIC_MODEL[20000];
  //for (int i = 0; i < 20000; i++)
  //{
  //  // CBaseModelInfo::CBaseModelInfo
  //  ((void(*)(_ATOMIC_MODEL*))(g_GTASAAdr + 0x384F89))(&ATOMIC_MODELS[i]);
  //  ATOMIC_MODELS[i].func_tbl = g_GTASAAdr + 0x667454;
  //  memset(ATOMIC_MODELS[i].data, 0, sizeof(ATOMIC_MODELS->data));
  //}

  // ARMHook::writeMemory(g_GTASAAdr + 0x468B7E, (uintptr_t)"\x4F\xF4\x00\x30\x00", 4);// \x4F\xF4\x00\x30\x00
    // ARMHook::writeMemory(g_GTASAAdr + 0x468B88, (uintptr_t)"\xF7\x20", 2); // \xF7\x20
    // ARMHook::writeMemory(g_GTASAAdr + 0x468B8A, (uintptr_t)"\xF7\x25", 2); // \xF7\x25
    // ARMHook::writeMemory(g_GTASAAdr + 0x468BCC, (uintptr_t)"\xF7\x28", 2); // \xF7\x28
    */