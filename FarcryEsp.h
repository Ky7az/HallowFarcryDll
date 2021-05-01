#ifndef __FARCRYESP_H__
#define __FARCRYESP_H__

/** INCLUDES **/
#include <sstream>
#include <boost/format.hpp>
#include "FarcryModule.h"
#include "FarcryEntity.h"
#include "..\HallowCheat\Esp.h"
#include "..\HallowDirectX\Vector.h"

/** DEFINES **/

/** CLASSES **/
/* FarcryEsp */
class CFarcryEsp : public CEsp, public CFarcryModule
{
public:
	CFarcryEsp(unsigned short id, std::wstring name, Renderer *RendererPtr);
	~CFarcryEsp();

	void on_setup(IRenderer *IRendererPtr, IEntity *LocalIEntityPtr);

	void pre_run();
	void post_run();
	void run(CFarcryEntity *FarcryEntityPtr);

	void display_entity_box(CFarcryEntity *FarcryEntityPtr);

private:
	IRenderer *IRendererPtr;
	CFarcryEntity *LocalFarcryEntityPtr;

	//std::vector<FarcryEntity*> FarcryEntitiesPtr;
};

#endif