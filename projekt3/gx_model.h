#include "global.h"

extern GLvoid display ( GLvoid );
class gx_model
{
private:
	gx_kdtree	*kdtree;


public:
	VertList	verts;
	VertPointerList templ;

	long		vertcount;
	gx_aabb		*box;
	gx_model();
	void load_off(const char* filename, int r);
	void render();
	void visStructure();
	void createKdtree(int n);
	void genExtraPoints();
	VertPointerList collectInRadius(GX_VEC3& p, float radius);
	VertPointerList collectKNearest(GX_VEC3& p, int k);
	
	VertList rayMarch();
	VertList sphereTrace();
};
