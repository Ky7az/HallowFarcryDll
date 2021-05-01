#include "stdafx.h"
#include "FarcryAim.h"
using namespace std;

/* FarcryAimBase */
CFarcryAimBase::CFarcryAimBase(Renderer *RendererPtr) : CAimBase(RendererPtr)
{
	IRendererPtr = NULL;
	LocalFarcryEntityPtr = NULL;
}

CFarcryAimBase::~CFarcryAimBase()
{
	delete LocalFarcryEntityPtr;
}

void CFarcryAimBase::on_setup(IRenderer *IRendererPtr, IEntity *LocalIEntityPtr)
{
	this->IRendererPtr = IRendererPtr;
	LocalFarcryEntityPtr = new CFarcryEntity(LocalIEntityPtr);

	screen_width = *(int*)((DWORD)IRendererPtr + IRenderer_Width_Offset);
	screen_height = *(int*)((DWORD)IRendererPtr + IRenderer_Height_Offset);
	center_pos = { screen_width * 0.5f, screen_height * 0.5f };
}

/* FarcryAimbot */
CFarcryAimbot::CFarcryAimbot(unsigned short id, std::wstring name, Renderer *RendererPtr) : CAimBase(RendererPtr), CModule(id, name), CAimbot(id, name, RendererPtr), CFarcryAimBase(RendererPtr), CFarcryModule(id, name)
{
}

CFarcryAimbot::~CFarcryAimbot()
{
}

void CFarcryAimbot::pre_run()
{
	center_pos = { screen_width * 0.5f, screen_height * 0.5f };

	aim_pos = { center_pos.x, center_pos.y };
	low_dist = fov;
	aim_ang = Vec3D();
	low_fov = fov;
}

void CFarcryAimbot::post_run()
{
	if (GetAsyncKeyState(VK_CONTROL) != 0) {
		//if (aim_pos.z < 1.0f)
		//	aim_at_position(aim_pos.x, aim_pos.y);
		if (aim_ang.x != 0.0f && aim_ang.y != 0.0f)
			LocalFarcryEntityPtr->set_angles(aim_ang);
	}
}

/*
void CFarcryAimbot::run(CFarcryEntity *FarcryEntityPtr)
{
	Vec3D *entity_pos = FarcryEntityPtr->get_position();
	LocalFarcryEntityPtr->init();
	float dist = LocalFarcryEntityPtr->get_position()->calc_dist(*entity_pos);

	if (dist <= dist_max && FarcryEntityPtr->get_health() > 0) {
		CFarcryBone *FarcryBoneHeadPtr = FarcryEntityPtr->create_bone("Bip01 Head");
		FarcryBoneHeadPtr->init();

		Vec3D *head = FarcryBoneHeadPtr->get_position();
		*head += *entity_pos;

		Vec3D w2s_head;
		IRendererPtr->ProjectToScreen(head->x, head->y, head->z, &w2s_head.x, &w2s_head.y, &w2s_head.z);
		w2s_head.x *= (screen_width / 100.f);
		w2s_head.y *= (screen_height / 100.f);

		Vec2D w2s_headd = { w2s_head.x, w2s_head.y };
		float dist_headd = center_pos.calc_dist(w2s_headd);

		//if (FarcryEntityPtr->get_is_visible() == true)
		//{
		if (dist_headd < low_dist)
		{
			low_dist = dist_headd;
			aim_pos = w2s_head;
		}
		//}
	}
}
*/

void CFarcryAimbot::run(CFarcryEntity *FarcryEntityPtr)
{
	LocalFarcryEntityPtr->init();
	Vec3D local_pos = LocalFarcryEntityPtr->get_position();
	Vec3D local_ang = LocalFarcryEntityPtr->get_angles();

	CFarcryBone *LocalFarcryBoneHeadPtr = LocalFarcryEntityPtr->create_bone("Bip01 Head");
	LocalFarcryBoneHeadPtr->init();
	Vec3D local_head = LocalFarcryBoneHeadPtr->get_position();
	local_head += local_pos;

	Vec3D entity_pos = FarcryEntityPtr->get_position();
	Vec3D entity_ang = FarcryEntityPtr->get_angles();
	float dist = local_pos.calc_dist(entity_pos);

	if (dist <= dist_max && FarcryEntityPtr->get_health() > 0) {

		CFarcryBone *FarcryBoneHeadPtr = FarcryEntityPtr->create_bone("Bip01 Head");
		FarcryBoneHeadPtr->init();
		Vec3D target_head = FarcryBoneHeadPtr->get_position();
		target_head += entity_pos;

		Vec3D ang;
		calc_angles(local_head, target_head, ang);
		float angdiff = diff_angles(local_ang, ang);

		if (angdiff < low_fov) {
			low_fov = angdiff;
			aim_ang = ang;
		}
	}
}

/* FarcryTriggerbot */
CFarcryTriggerbot::CFarcryTriggerbot(unsigned short id, std::wstring name, Renderer *RendererPtr) : CAimBase(RendererPtr), CModule(id, name), CTriggerbot(id, name, RendererPtr), CFarcryAimBase(RendererPtr), CFarcryModule(id, name)
{
}

CFarcryTriggerbot::~CFarcryTriggerbot()
{
}

void CFarcryTriggerbot::on_setup(IRenderer *IRendererPtr, CXEntityProcessingCmd *CXEntityProcessingCmdPtr, IEntity *LocalIEntityPtr)
{
	CFarcryAimBase::on_setup(IRendererPtr, LocalIEntityPtr);
	this->CXEntityProcessingCmdPtr = CXEntityProcessingCmdPtr;
}

void CFarcryTriggerbot::pre_run()
{
}

void CFarcryTriggerbot::post_run()
{
}

void CFarcryTriggerbot::run(CFarcryEntity *FarcryEntityPtr)
{
	Vec3D entity_pos = FarcryEntityPtr->get_position();
	Vec3D local_pos = LocalFarcryEntityPtr->get_position();
	LocalFarcryEntityPtr->init();
	float dist = local_pos.calc_dist(entity_pos);

	if (dist <= dist_max && FarcryEntityPtr->get_health() > 0) {
		CFarcryBone *FarcryBoneHeadPtr = FarcryEntityPtr->create_bone("Bip01 Head");
		FarcryBoneHeadPtr->init();

		Vec3D head = FarcryBoneHeadPtr->get_position();
		Vec3D entity_pos = FarcryEntityPtr->get_position();
		head += entity_pos;

		Vec3D w2s_head;
		IRendererPtr->ProjectToScreen(head.x, head.y, head.z, &w2s_head.x, &w2s_head.y, &w2s_head.z);
		w2s_head.x *= (screen_width / 100.f);
		w2s_head.y *= (screen_height / 100.f);

		Vec2D w2s_headd = { w2s_head.x, w2s_head.y };
		float dist = center_pos.calc_dist(w2s_headd);

		//if (FarcryEntityPtr->get_is_visible() == true)
		//{
		if (fabs(dist) <= fov)
			action_fire();
		//}
	}
}

void CFarcryTriggerbot::action_fire()
{
	CXEntityProcessingCmdPtr->AddAction(ACTION_FIRE);
}