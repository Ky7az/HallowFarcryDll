#ifndef __FARCRYAIM_H__
#define __FARCRYAIM_H__

/** INCLUDES **/
#include <boost/format.hpp>
#include "FarcryEntity.h"
#include "FarcryModule.h"
#include "..\HallowCheat\Aim.h"

/** DEFINES **/

/** CLASSES **/
/* FarcryAimBase */
class CFarcryAimBase : public virtual CAimBase
{
public:
	CFarcryAimBase(Renderer *RendererPtr);
	~CFarcryAimBase();

	void on_setup(IRenderer *IRendererPtr, IEntity *LocalIEntityPtr);

protected:
	IRenderer *IRendererPtr;
	CFarcryEntity *LocalFarcryEntityPtr;
};

/* FarcryAimbot */
class CFarcryAimbot : public CFarcryAimBase, public CAimbot, public CFarcryModule
{
public:
	CFarcryAimbot(unsigned short id, std::wstring name, Renderer *RendererPtr);
	~CFarcryAimbot();

	void pre_run();
	void post_run();
	void run(CFarcryEntity *FarcryEntityPtr);
};

/* FarcryTriggerbot */
class CFarcryTriggerbot : public CFarcryAimBase, public CTriggerbot, public CFarcryModule
{
public:
	CFarcryTriggerbot(unsigned short id, std::wstring name, Renderer *RendererPtr);
	~CFarcryTriggerbot();

	void on_setup(IRenderer *IRendererPtr, CXEntityProcessingCmd *CXEntityProcessingCmdPtr, IEntity *LocalIEntityPtr);

	void pre_run();
	void post_run();
	void run(CFarcryEntity *FarcryEntityPtr);
	void action_fire();

private:
	CXEntityProcessingCmd *CXEntityProcessingCmdPtr;
};

#endif