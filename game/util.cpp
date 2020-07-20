#include "../main.h"
#include "game.h"

#define PI 3.14159265

uintptr_t dwPlayerPedPtrs[PLAYER_PED_SLOTS];

extern char* PLAYERS_REALLOC;

// 2.0
PED_TYPE* GamePool_FindPlayerPed()
{
	return ((PED_TYPE*(*)(int))(g_GTASAAdr + 0x40B288 + 1))(-1);
}

// 2.0
PED_TYPE* GamePool_Ped_GetAt(int iID)
{
	return (( PED_TYPE* (*)(int))(g_GTASAAdr+0x483D49))(iID);
}

// 2.0
int GamePool_Ped_GetIndex(PED_TYPE *pActor)
{
    return (( int (*)(PED_TYPE*))(g_GTASAAdr+0x483D3B))(pActor);
}

// 2.0
VEHICLE_TYPE *GamePool_Vehicle_GetAt(int iID)
{
	return (( VEHICLE_TYPE* (*)(int))(g_GTASAAdr+0x483D2F))(iID);
}

// 2.0
int GamePool_Vehicle_GetIndex(VEHICLE_TYPE *pVehicle)
{
    return (( int (*)(VEHICLE_TYPE*))(g_GTASAAdr+0x483D21))(pVehicle);
}

// 2.0
ENTITY_TYPE *GamePool_Object_GetAt(int iID)
{
	ENTITY_TYPE* (*GetPoolObj)(int iID);
	*(void **) (&GetPoolObj) = (void*)(g_GTASAAdr+0x483D63);
	return (GetPoolObj)(iID);
} 

int LineOfSight(VECTOR* start, VECTOR* end, void* colpoint, uintptr_t ent,
	char buildings, char vehicles, char peds, char objects, char dummies, bool seeThrough, bool camera, bool unk)
{
	return 0;
}

// 0.3.7
bool IsPedModel(unsigned int iModelID)
{
	if(iModelID < 0 || iModelID > 20000) return false;
    uintptr_t *dwModelArray = (uintptr_t*)(g_GTASAAdr+0x91DCB8);
    
    uintptr_t ModelInfo = dwModelArray[iModelID];
    if(ModelInfo && *(uintptr_t*)ModelInfo == (uintptr_t)(g_GTASAAdr+0x667668/*CPedModelInfo vtable*/))
        return true;

    return false;
}

// 0.3.7
bool IsValidModel(unsigned int uiModelID)
{
    if(uiModelID < 0 || uiModelID > 20000) return false;
    uintptr_t *dwModelArray = (uintptr_t*)(g_GTASAAdr+0x91DCB8);

    uintptr_t dwModelInfo = dwModelArray[uiModelID];
    if(dwModelInfo && *(uintptr_t*)(dwModelInfo+0x34/*pRwObject*/))
        return true;

    return false;
}

uint16_t GetModelReferenceCount(int nModelIndex)
{
	uintptr_t *dwModelarray = (uintptr_t*)(g_GTASAAdr+0x91DCB8);
	uint8_t *pModelInfoStart = (uint8_t*)dwModelarray[nModelIndex];
	
	return *(uint16_t*)(pModelInfoStart+0x1E);
}

void InitPlayerPedPtrRecords()
{
	memset(&dwPlayerPedPtrs[0], 0, sizeof(uintptr_t) * PLAYER_PED_SLOTS);
}

void SetPlayerPedPtrRecord(uint8_t bytePlayer, uintptr_t dwPedPtr)
{
	dwPlayerPedPtrs[bytePlayer] = dwPedPtr;
}

uint8_t FindPlayerNumFromPedPtr(uintptr_t dwPedPtr)
{
	uint8_t x = 0;
	while(x != PLAYER_PED_SLOTS)
	{
		if(dwPlayerPedPtrs[x] == dwPedPtr) return x;
		x++;
	}

	return 0;
}

uintptr_t GetTexture(const char* texture)
{
	FLog("GetTexture: %s", texture);

	// GetTexture
	uintptr_t pRwTexture = (( uintptr_t (*)(const char*))(g_GTASAAdr+0x1E9CE5))(texture);

	if(pRwTexture) 
	{
		int iTextureCount = *(int*)(pRwTexture + 0x54);
		iTextureCount++;
		*(int*)(pRwTexture + 0x54) = iTextureCount;
	}

	return pRwTexture;
}

uintptr_t LoadTextureFromDB(const char* dbname, const char* texture)
{
	// TextureDatabaseRuntime::GetDatabase(dbname)
	uintptr_t db_handle = (( uintptr_t (*)(const char*))(g_GTASAAdr+0x1BF530+1))(dbname);
	if(!db_handle)
	{
		FLog("Error: Database not found! (%s)", dbname);
		return 0;
	}
	// TextureDatabaseRuntime::Register(TextureDatabaseRuntime*) .text 001E9BC8 0000009C 00000020 00000000 R . . . B T .
	(( void (*)(uintptr_t))(g_GTASAAdr+0x1E9BC8+1))(db_handle);
	uintptr_t tex = GetTexture(texture);

	if(!tex) FLog("Error: Texture (%s) not found in database (%s)", dbname, texture);

	// TextureDatabaseRuntime::Unregister(TextureDatabaseRuntime*) .text 001E9C80 00000058 00000008 FFFFFFF8 R . . . . T .
	(( void (*)(uintptr_t))(g_GTASAAdr+0x1E9C80+1))(db_handle);

	return tex;
}

void DefinedState2d()
{
	return (( void (*)())(g_GTASAAdr+0x5D0C65))();
}

void SetScissorRect(void* pRect)
{
	// 0x2B3E55
}

float DegToRad(float fDegrees)
{
    if (fDegrees > 360.0f || fDegrees < 0.0f) return 0.0f;
    
    if (fDegrees > 180.0f) return (float)(-(PI - (((fDegrees - 180.0f) * PI) / 180.0f)));
    else return (float)((fDegrees * PI) / 180.0f);
}

// 0.3.7
float FloatOffset(float f1, float f2)
{   
    if(f1 >= f2) return f1 - f2;
    else return (f2 - f1);
}

bool GetModelInfoById(int iModel)
{
	uintptr_t *dwModelArray = (uintptr_t*)(g_GTASAAdr+0x91DCB8);

	if(iModel < 0 || iModel > 20000) {
		return false;
	}

	if(dwModelArray[iModel] == 0) {
    	return false;
    }

    return true;
}