#ifndef __FARCRYENTITY_H__
#define __FARCRYENTITY_H__

/** INCLUDES **/
#include "FarcrySdk.h"
#include "..\HallowCheat\Entity.h"
#include "..\HallowFarcry\FarcryDef.h"

/** DEFINES **/

/** CLASSES **/

/* FarcryBone */
class CFarcryBone : public CBone
{
public:
	CFarcryBone(std::string name, ICryBone *ICryBonePtr);
	~CFarcryBone();

	void init();

private:
	ICryBone *ICryBonePtr;
};

/* FarcryEntity */
class CFarcryEntity : public CEntity
{
public:
	CFarcryEntity(IEntity* IEntityPtr);
	~CFarcryEntity();

	Vec3D get_angles() const;
	void set_angles(const Vec3D &ang, bool smooth = false);

	//void pre_X();
	//void post_X();

	void init();
	bool is_enemy_ai();

	CFarcryBone* create_bone(std::string name);
	CFarcryBone* search_bone(std::string name);

private:
	IEntity *IEntityPtr;
	IEntityCharacter *IEntityCharacterPtr;
	ICryCharInstance *ICryCharInstancePtr;
	CPlayer *CPlayerPtr;

	std::vector<CFarcryBone*> Bonz;
};

#endif