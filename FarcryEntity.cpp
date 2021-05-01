#include "stdafx.h"
#include "FarcryEntity.h"
using namespace std;

/* FarcryBone */
CFarcryBone::CFarcryBone(std::string name, ICryBone *ICryBonePtr) : CBone(name)
{
	this->ICryBonePtr = ICryBonePtr;
}

CFarcryBone::~CFarcryBone()
{
}

void CFarcryBone::init()
{
	vec3d pos = ICryBonePtr->GetBonePosition();
	position = Vec3D(pos.x, pos.y, pos.z);
}

/* FarcryEntity */
CFarcryEntity::CFarcryEntity(IEntity* IEntityPtr) : CEntity() // (DWORD*)IEntityPtr
{
	this->IEntityPtr = IEntityPtr;
	this->IEntityCharacterPtr = NULL;
	this->ICryCharInstancePtr = NULL;
	this->CPlayerPtr = NULL;
}

CFarcryEntity::~CFarcryEntity()
{
	for (auto &b : Bonz) {
		delete b;
	}
	Bonz.clear();
}

Vec3D CFarcryEntity::get_angles() const
{
	return Vec3D(angles.x, angles.z, angles.y);
}

void CFarcryEntity::set_angles(const Vec3D &ang, bool smooth)
{
	IEntityPtr->SetAngles(Vec3D(ang.x, ang.z, ang.y));
}

bool CFarcryEntity::is_enemy_ai()
{
	type = (char*)(*(DWORD*)((DWORD)IEntityPtr + IEntity_ClassName_Offset));
	vector<string> wanted_types {"MercCover", "MercRear", "MercScout", "Grunt"};
	return find(wanted_types.begin(), wanted_types.end(), type) != wanted_types.end();
}

void CFarcryEntity::init()
{
	id = IEntityPtr->GetId();
	name = IEntityPtr->GetName();
	IEntityCharacterPtr = (IEntityCharacter*)((DWORD)IEntityPtr + IEntity_Character_Offset);
	ICryCharInstancePtr = IEntityCharacterPtr->GetCharacter(0);
	type = (char*)(*(DWORD*)((DWORD)IEntityPtr + IEntity_ClassName_Offset));

	CPlayerPtr = (CPlayer*)*(DWORD*)((DWORD)IEntityPtr + IEntity_Container_Offset);
	void *entity_stats = (void*)((DWORD)CPlayerPtr + CPlayer_PlayerStats_Offset);
	health = *(int*)((DWORD)entity_stats + CPlayerStats_Health_Offset);

	//is_visible = CPlayerPtr->IsVisible();

	position = *(Vec3D*)((DWORD)IEntityPtr + IEntity_Pos_Offset);
	angles = *(Vec3D*)((DWORD)IEntityPtr + IEntity_Angles_Offset);

	IEntityPtr->GetBBox(bot_left, top_right);
}

CFarcryBone* CFarcryEntity::create_bone(string name)
{
	CFarcryBone *fb;
	if ((fb = search_bone(name)) == NULL) {
		ICryBone *bone = ICryCharInstancePtr->GetBoneByName(name.c_str());
		fb = new CFarcryBone(name, bone);
		Bonz.push_back(fb);
		return fb;
	} else {
		return fb;
	}
}

CFarcryBone* CFarcryEntity::search_bone(std::string name)
{
	for (const auto &bone : Bonz) {
		if (bone->get_name() == name)
			return bone;
	}
	return NULL;
}