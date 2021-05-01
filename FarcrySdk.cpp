#include "stdafx.h"
#include "FarcrySdk.h"

/* CPlayer */
/*bool CPlayer::IsVisible()
{
	typedef bool(__thiscall *IsVisible)(void*);
	IsVisible xIsVisible = (IsVisible)0x3309FFEB;
	return xIsVisible(this);
}*/

/* CXEntityProcessingCmd */
void CXEntityProcessingCmd::AddAction(unsigned int nFlags)
{
	typedef void(__thiscall *AddAction)(void*, unsigned int);
	AddAction xAddAction = (AddAction)0x330BC018;
	return xAddAction(this, nFlags);
}

/* ICryBone */
vec3d ICryBone::GetBonePosition()
{
	typedef vec3d(__thiscall* OriginalFn)(void*);
	return getvfunc<OriginalFn>(this, 7)(this);
}

/* IEntityCharacter */
ICryCharInstance* IEntityCharacter::GetCharacter(int pos)
{
	typedef ICryCharInstance*(__thiscall* OriginalFn)(void*, int);
	return getvfunc<OriginalFn>(this, 4)(this, pos);
}

/* ICryCharInstance */
ICryBone* ICryCharInstance::GetBoneByName(const char *name)
{
	typedef ICryBone*(__thiscall* OriginalFn)(void*, const char*);
	return getvfunc<OriginalFn>(this, 77)(this, name);
}

/* IEntity */
const char* IEntity::GetName()
{
	typedef const char*(__thiscall* OriginalFn)(void*);
	return getvfunc<OriginalFn>(this, 4)(this);
}

void IEntity::GetBBox(Vec3D& mins, Vec3D& maxs)
{
	typedef void(__thiscall* OriginalFn)(void*, Vec3D& mins, Vec3D& maxs);
	return getvfunc<OriginalFn>(this, 6)(this, mins, maxs);
}

void IEntity::SetAngles(const Vec3D& pos, bool bNotifyContainer, bool bUpdatePhysics, bool forceInWorld)
{
	typedef void(__thiscall* OriginalFn)(void*, const Vec3D& pos, bool bNotifyContainer, bool bUpdatePhysics, bool forceInWorld);
	return getvfunc<OriginalFn>(this, 106)(this, pos, bNotifyContainer, bUpdatePhysics, forceInWorld);
}

unsigned short IEntity::GetId()

{
	typedef unsigned short(__thiscall* OriginalFn)(void*);
	return getvfunc<OriginalFn>(this, 49)(this);
}

/* IEntityIterator */
IEntity* IEntityIterator::Next()
{
	typedef IEntity*(__thiscall* OriginalFn)(void*);
	return getvfunc<OriginalFn>(this, 3)(this);
}

void IEntityIterator::MoveFirst()
{
	typedef void(__thiscall* OriginalFn)(void*);
	return getvfunc<OriginalFn>(this, 4)(this);
}

/* IEntitySystem */
IEntityIterator* IEntitySystem::GetEntityIterator()
{
	typedef IEntityIterator*(__thiscall* OriginalFn)(void*);
	return getvfunc<OriginalFn>(this, 10)(this);
}

/* IRenderer */
void IRenderer::ProjectToScreen(float ptx, float pty, float ptz, float *sx, float *sy, float *sz)
{
	typedef void(__thiscall* OriginalFn)(void*, float, float, float, float*, float*, float*);
	return getvfunc<OriginalFn>(this, 56)(this, ptx, pty, ptz, sx, sy, sz);
}