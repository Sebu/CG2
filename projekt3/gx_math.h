#include "global.h"



class gx_model;

typedef struct GX_VEC3
{

public:
	union {
		struct {
			GLdouble x,y,z;
		};
		GLdouble v[3];
	};
	
    union {
		struct {
			GLfloat nx,ny,nz;
		};
		GLfloat n[3];
	};
	

    long num;
    float fi; // funktionswert des punktes
	float distance;

	GX_VEC3() {};
	GX_VEC3( GLdouble _x, GLdouble _y, GLdouble _z );
	GX_VEC3( GLfloat _x, GLfloat _y, GLfloat _z ,GLfloat _nx, GLfloat _ny, GLfloat _nz );
	GX_VEC3 operator* (const GX_VEC3& b) const;
    GX_VEC3 operator- (const GX_VEC3& b) const;
    GX_VEC3 operator+ (const GX_VEC3& b) const;
    bool operator== (const GX_VEC3& b) const;
    GX_VEC3 operator* (const float b) const;
    void normalize();
    float length();
    
}   GX_VEC3, *PGX_VEC3;


typedef struct GX_FACE
{
    int v1, v2, v3;
	GX_FACE() {};
        
} GX_FACE;



typedef GX_VEC3 Point;
typedef GX_VEC3 Vector;
typedef std::vector<GX_VEC3>  VertList;
typedef std::vector<PGX_VEC3> VertPointerList;

VertList genNormals(VertList tU, VertList tV);
void reduceData(VertList t, VertList &v, std::vector<GX_FACE> &f);

float dist(GX_VEC3& a, GX_VEC3& b);
VertList genTrisFine(int n, int m, int k, VertList& points);
VertList genTris(int n, int m, int k, VertList& points);
VertList polygonise(VertList& grid, double isolevel,VertList& verts);
GX_VEC3 vertexInterp(double isolevel, GX_VEC3& p1,GX_VEC3& p2);
bool compareDist(const GX_VEC3& a, const GX_VEC3& b);
bool compareDistPointer(const PGX_VEC3& a, const PGX_VEC3& b);

Point *genPointConst(float posx, float posy ,float posz, gx_model& o);
Point *genPointLinear(float posx, float posy ,float posz, gx_model& o);
Point *genPointSquare(float posx, float posy ,float posz, gx_model& o);

VertList genPoints(int n, int m, int k, gx_model& o,
                    Point*(*genPoint)(float posx, float posy ,float posz, gx_model& o) );

