#include "global.h"


typedef struct GX_VEC3
{

public:
	union {
		struct {
			GLfloat x,y,z;
		};
		GLfloat v[3];
	};
	
	float distance;

	GX_VEC3() {};
	GX_VEC3( GLfloat _x, GLfloat _y, GLfloat _z );

	

}	GX_VEC3, *PGX_VEC3;

float dist(GX_VEC3& a, GX_VEC3& b);
bool compare_dist(const GX_VEC3& a, const GX_VEC3& b);
bool compare_dist_pointer(const PGX_VEC3& a, const PGX_VEC3& b);
typedef GX_VEC3 Point;
typedef GX_VEC3 Vector;
typedef std::vector<GX_VEC3>  VertList;
typedef std::vector<PGX_VEC3> VertPointerList;
