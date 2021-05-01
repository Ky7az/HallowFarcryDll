#include "stdafx.h"
#include "FarcryMenu.h"

/* FarcryMenu */
CFarcryMenu::CFarcryMenu(Renderer *RendererPtr) : CMenu(RendererPtr)
{
	IRendererPtr = NULL;
}

CFarcryMenu::~CFarcryMenu()
{
}

void CFarcryMenu::on_setup(IRenderer *IRendererPtr)
{
	this->IRendererPtr = IRendererPtr;
}