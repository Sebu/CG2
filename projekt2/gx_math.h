#include "global.h"



class gx_model;

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
	GX_VEC3 operator* (const GX_VEC3& b) const;
    GX_VEC3 operator- (const GX_VEC3& b) const;
    GX_VEC3 operator+ (const GX_VEC3& b) const;
    GX_VEC3 operator* (const float b) const;
    void normalize();
    
}   GX_VEC3, *PGX_VEC3;




typedef GX_VEC3 Point;
typedef GX_VEC3 Vector;
typedef std::vector<GX_VEC3>  VertList;
typedef std::vector<PGX_VEC3> VertPointerList;

VertList genNormals(VertList tU, VertList tV);

GX_VEC3 berstein(int n, float t, VertList& l, GX_VEC3 *tangent);
VertList bezierV(int km, int kn, int m, int n,VertList& l, VertList *tangent);
VertList bezierU(int km, int kn, int m, int n,VertList& l, VertList *tangent);
float dist(GX_VEC3& a, GX_VEC3& b);
bool compareDist(const GX_VEC3& a, const GX_VEC3& b);
bool compareDistPointer(const PGX_VEC3& a, const PGX_VEC3& b);
VertList genPoints(int m, int n, gx_model& o, VertList *nor);

