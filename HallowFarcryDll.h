#ifndef __HALLOWFARCRYDLL_H__
#define __HALLOWFARCRYDLL_H__

/** INCLUDES **/
#include "FarcryCheat.h"
//#include "FarcryMenu.h"
#include "FarcryEsp.h"
#include "FarcryAim.h"
#include "FarcryEntity.h"
#include "..\HallowHook\Engine.h"
#include "..\HallowHook\Inline.h"
#include "..\HallowHook\VTable.h"
#include "..\HallowDirectX\Vector.h"
#include "..\HallowDirectX\Renderer.h"
//#include "..\HallowDirectX\D3D9Primitive.h"
#include "..\HallowFarcry\FarcryDef.h"
#include "..\HallowMem\HallowMem\Process.h"
#include "..\HallowMem\HallowMem\Search.h"

/** DEFINES **/
// Globals
HANDLE h_process = NULL;
HANDLE thread_1 = NULL;
HANDLE thread_2 = NULL;
HANDLE thread_3 = NULL;
HANDLE crygame_dll = NULL;
FILE *fd_debug;
long lock_count;
CRITICAL_SECTION cs_lock_count;
HookEngine *hook_engi = NULL;
HookVTable *hv_d3d9 = NULL;
HookVTable *hv_xr_d3d9 = NULL;

DWORD Base_addr = NULL;
DWORD CXClient_addr = NULL;
DWORD CXGame_addr = NULL;
DWORD IEntitySystem_addr = NULL;
DWORD IRenderer_addr = NULL;

int width, height;
Renderer *RendererPtr = new Renderer();
//D3D9Primitive *D3D9PrimitivePtr = new D3D9Primitive();
CFarcryCheat *FarcryCheatPtr = new CFarcryCheat(RendererPtr);
CFarcryMenu *FarcryMenuPtr = NULL;
CFarcryEsp *FarcryEspPtr = NULL;
CFarcryAimbot *FarcryAimbotPtr = NULL;
CFarcryTriggerbot *FarcryTriggerbotPtr = NULL;

// D3d9
typedef HRESULT(WINAPI *Reset)(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters); // 16
typedef HRESULT(WINAPI *Present)(IDirect3DDevice9 *pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion); // 17
typedef HRESULT(WINAPI *BeginScene)(IDirect3DDevice9 *pDevice); // 41
typedef HRESULT(WINAPI *EndScene)(IDirect3DDevice9 *pDevice); // 42
typedef HRESULT(WINAPI *DrawIndexedPrimitive)(IDirect3DDevice9 *pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount); // 82

// CPlayer
typedef void(__fastcall *FireGrenade)(void *_this, void *_EDX, Vec3D *origin, Vec3D *angles, void *pIShooter);
typedef void(__fastcall *SetWeapon)(void *_this, void *_EDX, int iClsID);

/** CLASSES **/
/* CXGame */
class CXGame
{
public:
	IEntity* GetMyPlayer(); // 28
private:
};

/* HookD3D9 */
class HookD3D9
{
public:
private:
};

CXGame *CXGamePtr = NULL;
IRenderer *IRendererPtr = NULL;
IEntitySystem *IEntitySystemPtr = NULL;
CXEntityProcessingCmd *CXEntityProcessingCmdPtr = NULL;
IEntity *local_entity = NULL;
CPlayer *local_player = NULL;

/** FUNCTIONS **/
//DWORD WINAPI check_dll_lock_count();
void init_farcry_addr();
//void init_farcry_class();
void hook_d3d9_vtable();
//void hook_xr_d3d9_vtable();
void hook_farcry_inline();
void reset_addr();
void hack_player();

// D3d9
HRESULT WINAPI hookedReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters);
HRESULT WINAPI hookedPresent(LPDIRECT3DDEVICE9 pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);
HRESULT WINAPI hookedBeginScene(LPDIRECT3DDEVICE9 pDevice);
HRESULT WINAPI hookedEndScene(LPDIRECT3DDEVICE9 pDevice);
HRESULT WINAPI hookedDrawIndexedPrimitive(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount);

// CPlayer
void __fastcall hookedFireGrenade(void *_this, void *_EDX, Vec3D *origin, Vec3D *angles, void *pIShooter);
void __fastcall hookedSetWeapon(void *_this, void *_EDX, int iClsID);

#endif