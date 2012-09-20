#include "global.h" 


gx_aabb::gx_aabb()
{
	vmax = vmin = GX_VEC3(0.0f,0.0f,0.0f);
}

// aabb aus einer punktliste aufbauen 
// maximaler und minimaler punkt definieren die dimensionen
gx_aabb::gx_aabb(VertList& points, const int count)
{
	vmax = GX_VEC3(-MAX_SIZE,-MAX_SIZE,-MAX_SIZE);
	vmin = GX_VEC3(MAX_SIZE,MAX_SIZE,MAX_SIZE);

	for (int i=0; i<static_cast<int>(points.size()); i++)
	{
		if (points.at(i).x<vmin.x) vmin.x = points.at(i).x;
		if (points.at(i).x>vmax.x) vmax.x = points.at(i).x;

		if (points.at(i).y<vmin.y) vmin.y = points.at(i).y;
		if (points.at(i).y>vmax.y) vmax.y = points.at(i).y;

		if (points.at(i).z<vmin.z) vmin.z = points.at(i).z;
		if (points.at(i).z>vmax.z) vmax.z = points.at(i).z;
	}
}


gx_aabb::gx_aabb(GX_VEC3 _vmax, GX_VEC3 _vmin)
{
	vmax = _vmax;
	vmin = _vmin;

}

// abstand eines punktes zu einer bbox bestimmen
// untere abschaetzung wie in der VL vorgeschlagen
float gx_aabb::distance(GX_VEC3& point)
{
	float dist = fabs(point.x-vmin.x),
		disttemp = 0.0f;
	
	for (int i = 0; i<2; i++)
	{
		disttemp = fabs(point.v[i]-vmin.v[i]);
		if(disttemp<=dist) dist = disttemp;
	}
	for (int i = 0; i<2; i++)
	{
		disttemp = fabs(point.v[i]-vmax.v[i]);
		if(disttemp<=dist) dist = disttemp;
	}
	return dist;
}

// mittelpunkt der bbox ermitteln
GX_VEC3 gx_aabb::center()
{
	GX_VEC3 center;
	center.x = vmax.x - vmin.x;
	center.y = vmax.y - vmin.y;
	center.z = vmax.z - vmin.z;
	return center;
}

// laengste seite einer bbox ermitteln
int gx_aabb::long_side()
{
	int result = 0;
	float side  = 0.0f;

	for (int i = 0; i<2; i++){
		if ( side < vmax.v[i] - vmin.v[i])
		{
			side = vmax.v[i] - vmin.v[i];
			result = i;
		}
	}
	return result;
}

// testen ob ein punkt p in der bbox liegt
bool gx_aabb::point_in(GX_VEC3 *point)
{
	if ( point->x <= vmax.x && point->x >= vmin.x &&
		 point->y <= vmax.y && point->y >= vmin.y &&
		 point->z <= vmax.z && point->z >= vmin.z
		)
		return true;
	return false;
}

// bbox als wireframe visualisieren
void gx_aabb::render()
{
	glBegin(GL_LINES);
		glVertex3f(vmin.x,vmin.y,vmin.z);
		glVertex3f(vmin.x,vmin.y,vmax.z);
		glVertex3f(vmax.x,vmin.y,vmin.z);
		glVertex3f(vmax.x,vmin.y,vmax.z);
		glVertex3f(vmax.x,vmax.y,vmin.z);
		glVertex3f(vmax.x,vmax.y,vmax.z);
		glVertex3f(vmin.x,vmax.y,vmin.z);
		glVertex3f(vmin.x,vmax.y,vmax.z);

		glVertex3f(vmin.x,vmin.y,vmin.z);
		glVertex3f(vmax.x,vmin.y,vmin.z);
		glVertex3f(vmin.x,vmax.y,vmin.z);
		glVertex3f(vmax.x,vmax.y,vmin.z);
		glVertex3f(vmin.x,vmin.y,vmin.z);
		glVertex3f(vmin.x,vmax.y,vmin.z);
		glVertex3f(vmax.x,vmin.y,vmin.z);
		glVertex3f(vmax.x,vmax.y,vmin.z);

		glVertex3f(vmin.x,vmin.y,vmax.z);
		glVertex3f(vmax.x,vmin.y,vmax.z);
		glVertex3f(vmin.x,vmax.y,vmax.z);
		glVertex3f(vmax.x,vmax.y,vmax.z);
		glVertex3f(vmin.x,vmin.y,vmax.z);
		glVertex3f(vmin.x,vmax.y,vmax.z);
		glVertex3f(vmax.x,vmin.y,vmax.z);
		glVertex3f(vmax.x,vmax.y,vmax.z);

	glEnd();
}

// bbox als soliden koerper visualisieren
void gx_aabb::render_solid()
{

	glBegin(GL_QUADS);
		glVertex3f(vmin.x,vmin.y,vmin.z);
		glVertex3f(vmin.x,vmax.y,vmin.z);
		glVertex3f(vmax.x,vmax.y,vmin.z);
		glVertex3f(vmax.x,vmin.y,vmin.z);

		glVertex3f(vmin.x,vmin.y,vmax.z);
		glVertex3f(vmin.x,vmax.y,vmax.z);
		glVertex3f(vmax.x,vmax.y,vmax.z);
		glVertex3f(vmax.x,vmin.y,vmax.z);

		glVertex3f(vmin.x,vmin.y,vmin.z);
		glVertex3f(vmin.x,vmin.y,vmax.z);
		glVertex3f(vmax.x,vmin.y,vmax.z);
		glVertex3f(vmax.x,vmin.y,vmin.z);
		
		glVertex3f(vmin.x,vmax.y,vmin.z);
		glVertex3f(vmin.x,vmax.y,vmax.z);
		glVertex3f(vmax.x,vmax.y,vmax.z);
		glVertex3f(vmax.x,vmax.y,vmin.z);

		glVertex3f(vmin.x,vmin.y,vmin.z);
		glVertex3f(vmin.x,vmin.y,vmax.z);
		glVertex3f(vmin.x,vmax.y,vmax.z);
		glVertex3f(vmin.x,vmax.y,vmin.z);

		glVertex3f(vmax.x,vmin.y,vmin.z);
		glVertex3f(vmax.x,vmin.y,vmax.z);
		glVertex3f(vmax.x,vmax.y,vmax.z);
		glVertex3f(vmax.x,vmax.y,vmin.z);

	glEnd();

}
