
#ifndef __GX_KDTREE_H__
#define __GX_KDTREE_H__

#include "global.h"




class gx_kdtree
{
private:
	bool		leaf;
	gx_kdtree	*children;
	VertList	points;
	

public:
	static int		max_in_node;
	static float	maxdist;
	gx_aabb			*box;
	gx_kdtree();
	void create(VertList& pointlist);
	float distance(GX_VEC3& point);
	VertPointerList collectInRadius(GX_VEC3& p, float radius, VertPointerList vlist);
	VertPointerList collectKNearest(GX_VEC3& p, int k, VertPointerList vlist);
	void render();
	void visStructure();
	void render_leaf();
	
};



#endif
