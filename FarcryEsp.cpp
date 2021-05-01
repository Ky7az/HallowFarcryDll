#include "stdafx.h"
#include "FarcryEsp.h"
using namespace std;

/* FarcryEsp */
CFarcryEsp::CFarcryEsp(unsigned short id, std::wstring name, Renderer *RendererPtr) : CModule(id, name), CEsp(id, name, RendererPtr), CFarcryModule(id, name)
{
	IRendererPtr = NULL;
	LocalFarcryEntityPtr = NULL;
}

CFarcryEsp::~CFarcryEsp()
{
	delete LocalFarcryEntityPtr;
}

void CFarcryEsp::on_setup(IRenderer *IRendererPtr, IEntity *LocalIEntityPtr)
{
	this->IRendererPtr = IRendererPtr;
	LocalFarcryEntityPtr = new CFarcryEntity(LocalIEntityPtr);

	screen_width = *(int*)((DWORD)IRendererPtr + IRenderer_Width_Offset);
	screen_height = *(int*)((DWORD)IRendererPtr + IRenderer_Height_Offset);
}

void CFarcryEsp::pre_run()
{
}

void CFarcryEsp::post_run()
{
}

void CFarcryEsp::run(CFarcryEntity *FarcryEntityPtr)
{
	display_entity_box(FarcryEntityPtr);
}

void CFarcryEsp::display_entity_box(CFarcryEntity *FarcryEntityPtr)
{
	Vec3D entity_position = FarcryEntityPtr->get_position();
	Vec3D local_pos = LocalFarcryEntityPtr->get_position();

	Vec3D w2s_pos;

	IRendererPtr->ProjectToScreen(entity_position.x, entity_position.y, entity_position.z, &w2s_pos.x, &w2s_pos.y, &w2s_pos.z);
	w2s_pos.x *= (screen_width / 100.00f);
	w2s_pos.y *= (screen_height / 100.00f);

	LocalFarcryEntityPtr->init();
	float dist = local_pos.calc_dist(entity_position);
	wstring repr = (boost::wformat(L"%s [%0.1fm] %dhp") % FarcryEntityPtr->get_typeW() % dist % FarcryEntityPtr->get_health()).str();

	if (dist <= dist_max && FarcryEntityPtr->get_health() > 0)
	{
		if (w2s_pos.z < 1.0f) {
			RendererPtr->draw_text(repr, w2s_pos.x - 10, w2s_pos.y + 10, Color::Purple());

		}

		pair<Vec3D, Vec3D> bbox = FarcryEntityPtr->get_bbox();
		Vec3D w2s_botleft, w2s_topright;
		IRendererPtr->ProjectToScreen(bbox.first.x, bbox.first.y, bbox.first.z, &w2s_botleft.x, &w2s_botleft.y, &w2s_botleft.z);
		IRendererPtr->ProjectToScreen(bbox.second.x, bbox.second.y, bbox.second.z, &w2s_topright.x, &w2s_topright.y, &w2s_topright.z);

		w2s_botleft.x *= (screen_width / 100.f); w2s_topright.x *= (screen_width / 100.f);
		w2s_botleft.y *= (screen_height / 100.f); w2s_topright.y *= (screen_height / 100.f);

		if ((w2s_topright.z < 1.0f) && (w2s_botleft.z < 1.0f))
		{
			if (w2s_botleft.x > w2s_topright.x)
				RendererPtr->draw_rect3(w2s_topright.x, w2s_botleft.y, w2s_botleft.x - w2s_topright.x, 1, Color::Purple());
			else
				RendererPtr->draw_rect3(w2s_botleft.x, w2s_botleft.y, w2s_topright.x - w2s_botleft.x, 1, Color::Purple());
			RendererPtr->draw_rect3(w2s_botleft.x, w2s_topright.y, 1, w2s_botleft.y - w2s_topright.y, Color::Purple());

			if (w2s_botleft.x > w2s_topright.x)
				RendererPtr->draw_rect3(w2s_topright.x, w2s_topright.y, w2s_botleft.x - w2s_topright.x, 1, Color::Purple());
			else
				RendererPtr->draw_rect3(w2s_botleft.x, w2s_topright.y, w2s_topright.x - w2s_botleft.x, 1, Color::Purple());
			RendererPtr->draw_rect3(w2s_topright.x, w2s_topright.y, 1, w2s_botleft.y - w2s_topright.y, Color::Purple());
		}
	}
}