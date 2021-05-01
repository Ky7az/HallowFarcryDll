#include "stdafx.h"
#include "FarcryCheat.h"
using namespace std;

/* FarcryCheat */
CFarcryCheat::CFarcryCheat(Renderer *RendererPtr) : CCheat(RendererPtr)
{
	IEntitySystemPtr = NULL;
}

CFarcryCheat::~CFarcryCheat()
{
	if (menu)
		delete menu;

	for (auto &m : Modz) {
		delete m;
	}
	Modz.clear();
}

CFarcryMenu* CFarcryCheat::get_menu() const { return menu; }

void CFarcryCheat::on_setup(IEntitySystem *IEntitySystemPtr)
{
	this->IEntitySystemPtr = IEntitySystemPtr;
}

void CFarcryCheat::pre_run_modz()
{
	for (const auto &mod : Modz) {
		if (mod->check_is_on() == true)
			mod->pre_run();
	}
}

void CFarcryCheat::post_run_modz()
{
	for (const auto &mod : Modz) {
		if (mod->check_is_on() == true)
			mod->post_run();
	}
}

void CFarcryCheat::run_modz(CFarcryEntity *FarcryEntityPtr)
{
	for (const auto &mod : Modz) {
		if (mod->check_is_on() == true)
			mod->run(FarcryEntityPtr);
	}
}

CFarcryMenu* CFarcryCheat::create_menu()
{
	CFarcryMenu *men = new CFarcryMenu(RendererPtr);
	return men;
}

void CFarcryCheat::draw_menu()
{
	if (menu)
		menu->draw();
}

CFarcryModule* CFarcryCheat::create_module(unsigned short id, ModuleType type, wstring name)
{
	CFarcryModule *mod = NULL;

	switch (type) {
	case Esp:
		mod = new CFarcryEsp(id, name, RendererPtr);
		break;
	case Aimbot:
		mod = new CFarcryAimbot(id, name, RendererPtr);
		break;
	case Triggerbot:
		mod = new CFarcryTriggerbot(id, name, RendererPtr);
		break;
	default:
		throw "Invalid Module Type";
		break;
	}

	Modz.push_back(mod);
	return mod;
}

CFarcryModule* CFarcryCheat::search_module(unsigned short id)
{
	for (const auto &mod : Modz) {
		if (mod->get_id() == id)
			return mod;
	}
	return NULL;
}

void CFarcryCheat::init()
{
	menu = create_menu();
	struct HackDat {
		ModuleType type;
		wstring name;
		int hotkey;
	};
	map<unsigned short, HackDat> HackMap {
		{ 1, { Esp, L"ESP", VK_F2 } },
		{ 2, { Aimbot, L"AIMBOT", VK_F3 } },
		{ 3, { Triggerbot, L"TRIGGERBOT", VK_F4 } }
	};
	for (const auto& m : HackMap) {
		CModule *mod = create_module(m.first, m.second.type, m.second.name);
		CMenuEntry *xme = new CMenuEntry(m.second.name, m.second.hotkey, mod);
		menu->add_menuentry(xme);
	}
}

void CFarcryCheat::scan_entities()
{
	if (IEntitySystemPtr == NULL)
		return;

	pre_run_modz();

	IEntityIteratorPtr = IEntitySystemPtr->GetEntityIterator();
	IEntityIteratorPtr->MoveFirst();

	IEntity *entity = NULL;
	while ((entity = IEntityIteratorPtr->Next()) != NULL)
	{
		CFarcryEntity *FarcryEntityPtr = new CFarcryEntity(entity);
		if (FarcryEntityPtr->is_enemy_ai()) {
			FarcryEntityPtr->init();
			run_modz(FarcryEntityPtr);
		}
		delete FarcryEntityPtr;
	}

	post_run_modz();
}