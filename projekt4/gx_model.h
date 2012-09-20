#include "global.h"

extern GLvoid display ( GLvoid );

class gx_model
{
private:
	gx_kdtree              *kdtree;

public:
	VertList                verts;
	std::vector<GX_HEDGE*>  edges;
	std::vector<GX_FACE*>   faces;
       
	VertPointerList        templ;         //pointerliste auf verts

	long                   vertcount;
	int                    edgecount;
	gx_aabb                *box;
	
	double                 edist;
	double                 espring;
	double                 erep;
	double                 crep;
	int                    k;
	double                 qual;
	
	double  energy() {return edist + erep + espring; };
	
	gx_model();
	void load_off(const char* filename, int r);
	void load_data(const char* filename, int r, gx_model &m);
	void render();
	void visStructure();
	void createKdtree(int n);
	void genExtraPoints();
	VertPointerList collectInRadius(GX_VEC3& p, float radius);
	VertPointerList collectKNearest(GX_VEC3& p, int k);
	
	VertList rayMarch();
	VertList sphereTrace();
};
