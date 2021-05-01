#ifndef __FARCRYCHEAT_H__
#define __FARCRYCHEAT_H__

/** INCLUDES **/
#include <string>
#include <map>
#include "FarcryMenu.h"
#include "FarcryEsp.h"
#include "FarcryAim.h"
#include "..\HallowCheat\Cheat.h"

/** DEFINES **/

/** CLASSES **/
/* FarcryCheat */
class CFarcryCheat : public CCheat
{
public:
	CFarcryCheat(Renderer *RendererPtr);
	~CFarcryCheat();

	void on_setup(IEntitySystem *IEntitySystemPtr);

	void pre_run_modz();
	void post_run_modz();
	void run_modz(CFarcryEntity *FarcryEntityPtr);

	CFarcryMenu* create_menu();
	void draw_menu();

	CFarcryModule* create_module(unsigned short id, ModuleType type, std::wstring name);
	CFarcryModule* search_module(unsigned short id);

	void init();
	void scan_entities();

	CFarcryMenu* get_menu() const;

private:
	CFarcryMenu *menu;
	IEntitySystem *IEntitySystemPtr;
	IEntityIterator *IEntityIteratorPtr;
	std::vector<CFarcryModule*> Modz;
};

#endif