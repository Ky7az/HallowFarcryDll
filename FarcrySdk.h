#ifndef __FARCRY_SDK_H__
#define __FARCRY_SDK_H__

/** INCLUDES **/
#include "../HallowDirectX/Vector.h"

/** DEFINES **/
inline void**& getvtable(void* inst, size_t offset = 0)
{
	return *reinterpret_cast<void***>((size_t)inst + offset);
}
inline const void** getvtable(const void* inst, size_t offset = 0)
{
	return *reinterpret_cast<const void***>((size_t)inst + offset);
}
template<typename Fn>
inline Fn getvfunc(const void* inst, size_t index, size_t offset = 0)
{
	return reinterpret_cast<Fn>(getvtable(inst, offset)[index]);
}

/** CLASSES **/
/* CPlayer */
class CPlayer
{
//public:
	//bool IsVisible();
};

/* CXEntityProcessingCmd */
class CXEntityProcessingCmd
{
public:
	void AddAction(unsigned int nFlags);
;};

/* ICharacter */
class ICharacter
{
};

/* ICryBone */
class ICryBone
{
public:
	vec3d GetBonePosition(); // 7
};

/* ICryCharInstance */
class ICryCharInstance
{
public:
	ICryBone* GetBoneByName(const char *name); // 77
};

/* IEntity */
class IEntity
{
public:
	const char* GetName(); // 4
	void GetBBox(Vec3D& mins, Vec3D& maxs); // 6
	unsigned short GetId(); // 49
	void SetAngles(const Vec3D& pos, bool bNotifyContainer = true, bool bUpdatePhysics = true, bool forceInWorld = false); // 106
};

/* IEntityCharacter */
class IEntityCharacter
{
public:
	ICryCharInstance* GetCharacter(int pos); // 4
};

/* IEntityIterator */
class IEntityIterator
{
public:
	IEntity* Next(); // 3
	void MoveFirst(); // 4
};

/* IEntitySystem */
class IEntitySystem
{
public:
	IEntityIterator* GetEntityIterator(); // 10
};

/* IRenderer */
class IRenderer
{
public:
	void ProjectToScreen(float ptx, float pty, float ptz, float *sx, float *sy, float *sz); // 56
};

#endif