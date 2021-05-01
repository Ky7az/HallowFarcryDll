#ifndef __FARCRYMODULE_H__
#define __FARCRYMODULE_H__

/** INCLUDES **/
#include "../HallowCheat/Module.h"
#include "FarcryEntity.h"

/** DEFINES **/

/** CLASSES **/
/* FarcryModule */
class CFarcryModule : public virtual CModule
{
public:
	CFarcryModule(unsigned short id, std::wstring name);
	virtual ~CFarcryModule();

	virtual void pre_run() = 0;
	virtual void post_run() = 0;
	virtual void run(CFarcryEntity *FarcryEntityPtr) = 0;
};

#endif
