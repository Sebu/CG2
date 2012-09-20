#include "global.h"

#define PI 3.14159

class gx_model;
typedef struct GX_HEDGE;
typedef struct GX_FACE;



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
	
    list<GX_HEDGE*> es;
    GX_HEDGE*  he;      // eine kante
    bool        b;      // randknoten?
    float      fi;      // funktionswert des punktes
	float      distance;

	GX_VEC3() {b=false; he=NULL; x = 0.0f; y = 0.0f; z = 0.0f;  nx = 0.0f; ny = 0.0f; nz = 0.0f; };
	GX_VEC3( GLdouble _x, GLdouble _y, GLdouble _z );
	GX_VEC3( GLfloat _x, GLfloat _y, GLfloat _z ,GLfloat _nx, GLfloat _ny, GLfloat _nz );
	GX_VEC3 operator* (const GX_VEC3& b) const;
    GX_VEC3 operator- (const GX_VEC3& b) const;
    GX_VEC3 operator+ (const GX_VEC3& b) const;
    GX_VEC3 operator* (const float b) const;

    void normalize();
    float length();
    
}   GX_VEC3, *PGX_VEC3;


typedef struct GX_HEDGE
{
    GX_VEC3*    sv;     // start vertex
    float       temp;
    GX_HEDGE*   opp;    // gegenseite
    GX_HEDGE*   prev;   // vorgaengerkante (CW)  
    GX_HEDGE*   next;   // nachfolgekante (CW) 
    GX_FACE*    f;
    bool        b;      // randkante?
    bool        removed;
    bool        spring;
    int         type;    
    GX_HEDGE(GX_VEC3* _sv) {type = 5; sv = _sv;
                            b = false; removed = false; opp = NULL; prev = NULL; next = NULL;};
    GX_HEDGE() {type = 8; b = false; removed = false; opp = NULL; prev = NULL;  sv = NULL; next = NULL; };
} GX_HEDGE;


typedef struct GX_FACE
{
    GX_HEDGE* e;        // eine kante
    
    bool    removed;
    
    GX_FACE(GX_HEDGE* _e) {e = _e; removed = false; };
    GX_FACE() {};
} GX_FACE;

typedef GX_VEC3 Point;
typedef GX_VEC3 Vector;
typedef std::vector<GX_VEC3>  VertList;
typedef std::vector<PGX_VEC3> VertPointerList;

double dihedralAngle(GX_HEDGE &e, GX_VEC3 &v);
double vertAngles(GX_VEC3 &v);
double egdeAngles(GX_HEDGE &e);
double angle(GX_VEC3& _a, GX_VEC3& _b);

void optimize(gx_model &m1, gx_model &m2);

bool swapable(GX_HEDGE* e, gx_model &m1, gx_model &m2);
bool swapEdge(GX_HEDGE* e, gx_model &m1, gx_model &m2);

bool collapseEdge(GX_HEDGE* e, gx_model &m1, gx_model &m2);
bool collapseable(GX_HEDGE* e, gx_model &m1, gx_model &m2);

bool splitable(GX_HEDGE* e, gx_model &m1, gx_model &m2);
bool splitEdge(GX_HEDGE* e, gx_model &m1, gx_model &m2);



VertList genNormals(VertList tU, VertList tV);

float dist(GX_VEC3& a, GX_VEC3& b);



VertList genTrisFine(int n, int m, int k, VertList& points);
VertList genTris(int n, int m, int k, VertList& points);
VertList polygonise(VertList& grid, double isolevel,VertList& verts);
GX_VEC3 vertexInterp(double isolevel, GX_VEC3& p1,GX_VEC3& p2);
bool compareDist(const GX_VEC3& a, const GX_VEC3& b);
bool compareDistPointer(const PGX_VEC3& a, const PGX_VEC3& b);

Point *genPointConst(float posx, float posy ,float posz, gx_model& o);


VertList genPoints(int n, int m, int k, gx_model& o,
                    Point*(*genPoint)(float posx, float posy ,float posz, gx_model& o) );

