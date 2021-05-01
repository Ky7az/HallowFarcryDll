#include "stdafx.h"
#include "FarcryModule.h"

/* FarcryModule */
CFarcryModule::CFarcryModule(unsigned short id, std::wstring name) : CModule(id, name)
{
}

CFarcryModule::~CFarcryModule()
{
	is_on = false;
}
