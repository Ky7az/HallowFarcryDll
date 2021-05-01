#ifndef __FARCRYMENU_H__
#define __FARCRYMENU_H__

/** INCLUDES **/
#include "FarcryEsp.h"
#include "..\HallowCheat\Menu.h"

/** DEFINES **/

/** CLASSES **/
/* FarcryMenu */
class CFarcryMenu : public CMenu
{
public:
	CFarcryMenu(Renderer *RendererPtr);
	~CFarcryMenu();

	void on_setup(IRenderer *IRendererPtr);

private:
	IRenderer *IRendererPtr;
};

#endif