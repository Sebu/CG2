#include "global.h"


class gx_model
{
private:
	gx_kdtree	*kdtree;


public:
	VertList	verts;
	long		vertcount;
	gx_aabb		*box;
	gx_model();
	void load_off(const char* filename);
	void render();
	void visStructure();
	void createKdtree(int n);
	VertPointerList collectInRadius(GX_VEC3& p, float radius);
	VertPointerList collectKNearest(GX_VEC3& p, int k);
};
