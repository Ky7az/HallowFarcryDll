#include "stdafx.h"
#include "HallowFarcryDll.h"
#include "FarcryEntity.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			h_process = GetCurrentProcess();
			fd_debug = fopen("C:\\Users\\Kyrah\\Desktop\\HallowFarcry.log", "a");
			hook_engi = new HookEngine();
			thread_1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)init_farcry_addr, NULL, 0, NULL);
			thread_2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)hook_d3d9_vtable, NULL, 0, NULL);
			thread_3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)hook_farcry_inline, NULL, 0, NULL);
			break;

		case DLL_PROCESS_DETACH:
			while (lock_count)
				Sleep(100);
			delete hook_engi;
			TerminateThread(thread_1, 1);
			fclose(fd_debug);
			break;

		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
	}
	return TRUE;
}

/* D3d9 Hooked Functions */
HRESULT WINAPI hookedReset(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	InterlockedIncrement(&lock_count);
	HookVTableEntry *hv_res = hook_engi->search_hookvtable(L"D3D9", 16);
	Reset trampolineReset = (Reset)hv_res->get_original_function();

	RendererPtr->on_reset();
	HRESULT ret = trampolineReset(pDevice, pPresentationParameters);
	InterlockedDecrement(&lock_count);

	return ret;
}

HRESULT WINAPI hookedPresent(IDirect3DDevice9 *pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion)
{
	InterlockedIncrement(&lock_count);
	HookVTableEntry *hv_pres = hook_engi->search_hookvtable(L"D3D9", 17);
	Present trampolinePresent = (Present)hv_pres->get_original_function();

	HRESULT ret = trampolinePresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	InterlockedDecrement(&lock_count);

	return ret;
}

HRESULT WINAPI hookedBeginScene(IDirect3DDevice9 *pDevice)
{
	InterlockedIncrement(&lock_count);
	HookVTableEntry *hv_bs = hook_engi->search_hookvtable(L"D3D9", 41);
	BeginScene trampolineBeginScene = (BeginScene)hv_bs->get_original_function();

	HRESULT ret = trampolineBeginScene(pDevice);
	InterlockedDecrement(&lock_count);

	return ret;
}

HRESULT WINAPI hookedEndScene(IDirect3DDevice9 *pDevice)
{
	InterlockedIncrement(&lock_count);
	HookVTableEntry *hv_es = hook_engi->search_hookvtable(L"D3D9", 42);

	EndScene trampolineEndScene = (EndScene)hv_es->get_original_function();

	if (RendererPtr->get_d3d9_device() == NULL) {
		hv_d3d9->reset(*((DWORD*)pDevice));
		RendererPtr->on_setup(pDevice);
		RendererPtr->register_font(L"Consolas", 12, FONT_STYLE_BOLD);
	}

	/*if (D3D9PrimitivePtr->get_d3d9_device() == NULL) {
		D3D9PrimitivePtr->on_setup(pDevice);
	}*/

	//D3D9PrimitivePtr->render();

	RendererPtr->pre_frame();

	if (FarcryMenuPtr)
		FarcryMenuPtr->draw();

	if (local_entity)
		FarcryCheatPtr->scan_entities();

	RendererPtr->post_frame();

	HRESULT ret = trampolineEndScene(pDevice);
	InterlockedDecrement(&lock_count);

	return ret;
}

HRESULT WINAPI hookedDrawIndexedPrimitive(IDirect3DDevice9 *pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount)
{
	InterlockedIncrement(&lock_count);
	HookVTableEntry *hv_dip = hook_engi->search_hookvtable(L"D3D9", 82);
	DrawIndexedPrimitive trampolineDrawIndexedPrimitive = (DrawIndexedPrimitive)hv_dip->get_original_function();

	/*fprintf(fd_debug, "Type = %d, BaseVertexIndex = %d, MinIndex = %d, NumVertices = %d, StartIndex = %d, PrimitiveCount = %d\n", Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);
	fflush(fd_debug);*/

	//IDirect3DDevice9_SetRenderState(d3d9_device, D3DRS_ZENABLE, D3DZB_FALSE);
	//IDirect3DDevice9_SetTexture(d3d9_device, 0, NULL);

	HRESULT ret = trampolineDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);
	InterlockedDecrement(&lock_count);

	return ret;
}

/* Farcry Hooked Functions */
void __fastcall hookedFireGrenade(void *_this, void *_EDX, Vec3D *origin, Vec3D *angles, void *pIShooter)
{
	HookInline *hi_firegre = hook_engi->search_hookinline(L"CPlayer::FireGrenade");
	FireGrenade trampolineFireGrenade = (FireGrenade)hi_firegre->get_trampoline_function();

	//fprintf(fd_debug, "FireGrenade\n");
	//fflush(fd_debug);

	return trampolineFireGrenade(_this, _EDX, origin, angles, pIShooter);
}

void __fastcall hookedSetWeapon(void *_this, void *_EDX, int iClsID)
{
	HookInline *hi_setweap = hook_engi->search_hookinline(L"CPlayer::SetWeapon");
	SetWeapon trampolineSetWeapon = (SetWeapon)hi_setweap->get_trampoline_function();

	return trampolineSetWeapon(_this, _EDX, iClsID);
}

void init_farcry_addr()
{
	Sleep(15000);
	crygame_dll = LoadLibrary(L"CryGame.dll");
	
	process_read(h_process, (DWORD)crygame_dll + CryGameDll_Base_Offset, &Base_addr, sizeof(DWORD));
	process_read(h_process, Base_addr + Base_CXGame_Offset, &CXGame_addr, sizeof(DWORD));
	process_read(h_process, CXGame_addr + CXGame_IRenderer_Offset, &IRenderer_addr, sizeof(DWORD));
	process_read(h_process, CXGame_addr + CXGame_IEntitySystem_Offset, &IEntitySystem_addr, sizeof(DWORD));

	process_read(h_process, IRenderer_addr + IRenderer_Width_Offset, &width, sizeof(int));
	process_read(h_process, IRenderer_addr + IRenderer_Height_Offset, &height, sizeof(int));

	CXGamePtr = (CXGame*)CXGame_addr;
	IRendererPtr = (IRenderer*)IRenderer_addr;
	IEntitySystemPtr = (IEntitySystem*)IEntitySystem_addr;

	if (FarcryCheatPtr == NULL) {
		FarcryCheatPtr = new CFarcryCheat(RendererPtr);
	}
	FarcryCheatPtr->init();
	FarcryMenuPtr = FarcryCheatPtr->get_menu();
	FarcryEspPtr = dynamic_cast<CFarcryEsp*>(FarcryCheatPtr->search_module(1));
	FarcryAimbotPtr = dynamic_cast<CFarcryAimbot*>(FarcryCheatPtr->search_module(2));
	FarcryTriggerbotPtr = dynamic_cast<CFarcryTriggerbot*>(FarcryCheatPtr->search_module(3));

	while (local_entity == NULL) {
		Sleep(1000);
		local_entity = CXGamePtr->GetMyPlayer();
	}

	local_player = (CPlayer*)*(DWORD*)((DWORD)local_entity + IEntity_Container_Offset);
	CXClient_addr = *(DWORD*)((DWORD)CXGame_addr + CXGame_CXClient_Offset);
	CXEntityProcessingCmdPtr = (CXEntityProcessingCmd*)CXClient_addr + CXClient_CXEntityProcessingCmd_Offset;

	FarcryMenuPtr->on_setup(IRendererPtr);
	FarcryCheatPtr->on_setup(IEntitySystemPtr);
	FarcryEspPtr->on_setup(IRendererPtr, local_entity);
	FarcryAimbotPtr->on_setup(IRendererPtr, local_entity);
	FarcryTriggerbotPtr->on_setup(IRendererPtr, CXEntityProcessingCmdPtr, local_entity);

	/*fprintf(fd_debug, "LocalEntity = %08x\n", local_entity);
	fprintf(fd_debug, "LocalPlayer = %08x\n", local_player);
	fflush(fd_debug);*/

	if (local_player)
		hack_player();
}

void hook_d3d9_vtable()
{
	unsigned char pattern[] = {
		0xC7, 0x06, '?', '?', '?', '?',
		0x89, 0x86, '?', '?', '?', '?',
		0x89, 0x86, '?', '?', '?', '?'
	};
	char mask[] = "xx????xx????xx????";

	DWORD d3d9_dll = (DWORD)LoadLibrary(L"d3d9.dll");
	int i = find_pattern((unsigned char*)d3d9_dll, 0x1D3000, pattern, mask);
	DWORD *d3d9_vtable;
	memcpy(&d3d9_vtable, (void*)(d3d9_dll + i + 2), 4);

	//fprintf(fd_debug, "d3d9_vtable = %08x\n", d3d9_vtable);
	//fflush(fd_debug);

	hv_d3d9 = new HookVTable(L"D3D9", (DWORD)d3d9_vtable, 119);
	hook_engi->add_hookvtable(hv_d3d9);

	hv_d3d9->hook(L"Reset", 16, (DWORD)hookedReset);
	hv_d3d9->hook(L"Present", 17, (DWORD)hookedPresent);
	hv_d3d9->hook(L"BeginScene", 41, (DWORD)hookedBeginScene);
	hv_d3d9->hook(L"EndScene", 42, (DWORD)hookedEndScene);
	hv_d3d9->hook(L"DrawIndexedPrimitive", 82, (DWORD)hookedDrawIndexedPrimitive);
}

void hook_farcry_inline()
{
	//crygame_dll = LoadLibrary(L"CryGame.dll");
	Sleep(20000);

	HookInline *hi_firegre = new HookInline(L"CPlayer::FireGrenade", 0x330A061C, (DWORD)hookedFireGrenade);
	hook_engi->add_hookinline(hi_firegre);
	hi_firegre->hook();

	HookInline *hi_setweap = new HookInline(L"CPlayer::SetWeapon", 0x330AE390, (DWORD)hookedSetWeapon);
	hook_engi->add_hookinline(hi_setweap);
	hi_setweap->hook();
}

/* CXGame */
IEntity* CXGame::GetMyPlayer()
{
	IEntity*(__thiscall *GetMyPlayer)(void*) = (IEntity*(__thiscall*)(void*))0x330CD5A6;
	return GetMyPlayer(this);
}

void reset_addr()
{
	Base_addr = NULL;
	CXClient_addr = NULL;
	//CXEntityProcessingCmd_addr = NULL;
	CXGame_addr = NULL;
	IEntitySystem_addr = NULL;
	IRenderer_addr = NULL;

	CXGamePtr = NULL;
	local_entity = NULL;
	local_player = NULL;

	delete FarcryCheatPtr;
	FarcryCheatPtr = NULL;
	FarcryMenuPtr = NULL;
	FarcryEspPtr = NULL;
	FarcryAimbotPtr = NULL;
	FarcryTriggerbotPtr = NULL;

	init_farcry_addr();
}

void hack_player()
{
	if (local_player) {

		// Weapons
		//int weapons[] = { 10, 13, 14, 15, 16, 19, 20, 21, 22 };
		//int i;
		//for (i = 0; i < sizeof(weapons); i++) {
		//	xMakeWeaponAvailable(player, NULL, weapons[i], true);
		//}

		// Max Values
		const int MAX_HEALTH = 255;
		const int MAX_ARMOR = 100;
		const int MAX_AMMO_IN_CLIP = 999;
		const int MAX_AMMO = 999;
		const int MAX_NUMOFGRENADES = 9;
		const bool HAS_FLASHLIGHT = true;
		const bool HAS_BINOCULARS = true;
		const float MAX_ACCURACY = 0.f;
		const int MAX_LEG_HEALTH = 100;
		const int MAX_ARM_HEALTH = 100;
		const float MAX_SPEED_SCALE = 3.f;
		const float MAX_STAMINA = 100.f;
		const float MAX_RECOIL = 0.f;

		// Player Structs
		void *player_recoil = (void*)((DWORD)local_player + CPlayer_PlayerRecoil_Offset);
		void *player_stats = (void*)((DWORD)local_player + CPlayer_PlayerStats_Offset);

		// Player Stats
		int *player_health = (int*)((DWORD)player_stats + CPlayerStats_Health_Offset);
		int *player_armor = (int*)((DWORD)player_stats + CPlayerStats_Armor_Offset);
		int *player_ammo_in_clip = (int*)((DWORD)player_stats + CPlayerStats_AmmoInClip_Offset);
		int *player_ammo = (int*)((DWORD)player_stats + CPlayerStats_Ammo_Offset);
		int *player_numofgrenades = (int*)((DWORD)player_stats + CPlayerStats_NumOfGrenades_Offset);
		bool *has_flashlight = (bool*)((DWORD)player_stats + CPlayerStats_HasFlashlight_Offset);
		bool *has_binoculars = (bool*)((DWORD)player_stats + CPlayerStats_HasBinoculars_Offset);
		float *player_accuracy = (float*)((DWORD)player_stats + CPlayerStats_Accuracy_Offset);
		int *player_leg_health = (int*)((DWORD)player_stats + CPlayerStats_LegHealth_Offset);
		int *player_arm_health = (int*)((DWORD)player_stats + CPlayerStats_ArmHealth_Offset);
		float *player_speed_scale = (float*)((DWORD)player_stats + CPlayerStats_SpeedScale_Offset);
		float *player_stamina = (float*)((DWORD)player_stats + CPlayerStats_Stamina_Offset);

		// Player Recoil
		float *player_recoil_x_delta = (float*)((DWORD)player_recoil + CPlayerRecoil_XDelta_Offset);
		float *player_recoil_z_delta = (float*)((DWORD)player_recoil + CPlayerRecoil_ZDelta_Offset);
		float *player_recoil_x_up = (float*)((DWORD)player_recoil + CPlayerRecoil_XUp_Offset);
		float *player_recoil_z_up = (float*)((DWORD)player_recoil + CPlayerRecoil_ZUp_Offset);
		float *player_recoil_x = (float*)((DWORD)player_recoil + CPlayerRecoil_X_Offset);
		float *player_recoil_z = (float*)((DWORD)player_recoil + CPlayerRecoil_Z_Offset);

		while (1) {

			if (CXGamePtr->GetMyPlayer() == NULL)
				break;

			if (*player_health < MAX_HEALTH)
				*player_health = MAX_HEALTH;
			if (*player_armor < MAX_ARMOR)
				*player_armor = MAX_ARMOR;
			if (*player_ammo_in_clip < MAX_AMMO_IN_CLIP)
				*player_ammo_in_clip = MAX_AMMO_IN_CLIP;
			if (*player_ammo < MAX_AMMO)
				*player_ammo = MAX_AMMO;
			if (*player_numofgrenades < MAX_NUMOFGRENADES)
				*player_numofgrenades = MAX_NUMOFGRENADES;
			if (*has_flashlight != HAS_FLASHLIGHT)
				*has_flashlight = HAS_FLASHLIGHT;
			if (*has_binoculars != HAS_BINOCULARS)
				*has_binoculars = HAS_BINOCULARS;
			if (*player_accuracy > MAX_ACCURACY)
				*player_accuracy = MAX_ACCURACY;
			if (*player_leg_health < MAX_LEG_HEALTH)
				*player_leg_health = MAX_LEG_HEALTH;
			if (*player_arm_health < MAX_ARM_HEALTH)
				*player_arm_health = MAX_ARM_HEALTH;
			if (*player_speed_scale < MAX_SPEED_SCALE)
				*player_speed_scale = MAX_SPEED_SCALE;
			if (*player_stamina < MAX_STAMINA)
				*player_stamina = MAX_STAMINA;

			if (*player_recoil_x_delta != MAX_RECOIL)
				*player_recoil_x_delta = MAX_RECOIL;
			if (*player_recoil_z_delta != MAX_RECOIL)
				*player_recoil_z_delta = MAX_RECOIL;
			if (*player_recoil_x_up != MAX_RECOIL)
				*player_recoil_x_up = MAX_RECOIL;
			if (*player_recoil_z_up != MAX_RECOIL)
				*player_recoil_z_up = MAX_RECOIL;
			if (*player_recoil_x != MAX_RECOIL)
				*player_recoil_x = MAX_RECOIL;
			if (*player_recoil_z != MAX_RECOIL)
				*player_recoil_z = MAX_RECOIL;
		}

		reset_addr();
	}
}