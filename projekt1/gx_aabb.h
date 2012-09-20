#include "global.h"

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
const float MAX_SIZE = 100000.0f;

class gx_aabb
{

public:
	GX_VEC3	vmax,vmin;

	gx_aabb();
	gx_aabb(VertList& points, const int count);
	gx_aabb(GX_VEC3 vmax, GX_VEC3 vmin);
	float distance(GX_VEC3& point);
	GX_VEC3 center();
	bool point_in(GX_VEC3 *point);
	int long_side();
	void render();
	void render_solid();
};
