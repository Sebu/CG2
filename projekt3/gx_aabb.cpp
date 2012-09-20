#include "global.h" 

float gx_aabb::diaLength()
{
    return (vmax - vmin).length();    
}

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
	
	for (int i = 0; i<3; i++)
	{
		disttemp = fabs(point.v[i]-vmin.v[i]);
		if(disttemp<=dist) dist = disttemp;
	}
	for (int i = 0; i<3; i++)
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
int gx_aabb::longSide()
{
	int result = 0;
	float side  = 0.0f;

	for (int i = 0; i<3; i++){
		if ( side < vmax.v[i] - vmin.v[i])
		{
			side = vmax.v[i] - vmin.v[i];
			result = i;
		}
	}
	return result;
}

// testen ob ein punkt p in der bbox liegt
bool gx_aabb::pointIn(GX_VEC3 *point)
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
void gx_aabb::renderSolid()
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

void gx_aabb::sizeUp(float factor)
{
	GX_VEC3 size = vmax-vmin;
	for (int i = 0; i<3; i++)
	{
	   vmin.v[i] -= size.v[i]*factor;	
	   vmax.v[i] += size.v[i]*factor;
    }	
}

VertList gx_aabb::rayIntersect(Point& a, Vector& v)
{
	int n = 0;
	VertList r;

	//delta werte
	float d, dx, dy, dz;

	//länge des richtungsvektors v
	float li = 1/v.length();

	//winkel für die 3 achsen mit v
	float win_v_x = v.x * li; //Vector(1.0f,0.0f,0.0f).dot(v*(li));
	float win_v_y = v.y * li;//Vector(0.0f,1.0f,0.0f).dot(v*(li));
	float win_v_z = v.z * li; //Vector(0.0f,0.0f,1.0f).dot(v*(li));

	//oben
	if( (a.y>=vmax.y && v.y<0.0f) || (a.y<=vmax.y && v.y>0.0f) )
	{
		// v auf die ebene projezieren
        d = win_v_y/(vmax.y-a.y);
        // mit hilfe von d und den winkeln die position des schnittpunktes berechnen
		dx = win_v_x / d;
		dz = win_v_z / d;
		// testen ob der gefundene punkt auch im 4eck liegt
		if(vmin.x<=a.x+dx && vmax.x>=a.x+dx && vmin.z<=a.z+dz && vmax.z>=a.z+dz)
			{r.push_back(Point(a.x+dx, vmax.y, a.z+dz)); n++;}
	}

	//unten
	if( (a.y>=vmin.y && v.y<0.0f) || (a.y<=vmin.y && v.y>0.0f) )
	{
		d = win_v_y/(vmin.y-a.y);
		dx = win_v_x / d;
		dz = win_v_z / d;
		if(vmin.x<=a.x+dx && vmax.x>=a.x+dx && vmin.z<=a.z+dz && vmax.z>=a.z+dz )
			{r.push_back(Point(a.x+dx, vmin.y, a.z+dz));n++;}
		if (n==2) return r;
	}

	//rechts
	if( (a.x>=vmax.x && v.x<0.0f) || (a.x<=vmax.x && v.x>0.0f) )
	{
		d = win_v_x/(vmax.x-a.x);
		dy = win_v_y / d;
		dz = win_v_z / d;
		if(vmin.y<=a.y+dy && vmax.y>=a.y+dy && vmin.z<=a.z+dz && vmax.z>=a.z+dz )
			{r.push_back(Point(vmax.x, a.y+dy, a.z+dz));n++;}
		if (n==2) return r;
	}

	//links
	if( (a.x>=vmin.x && v.x<0.0f) || (a.x<=vmin.x && v.x>0.0f) )
	{
		d = win_v_x/(vmin.x-a.x);
		dy = win_v_y / d;
		dz = win_v_z / d;
		if(vmin.y<=a.y+dy && vmax.y>=a.y+dy && vmin.z<=a.z+dz && vmax.z>=a.z+dz )
			{r.push_back(Point(vmin.x, a.y+dy, a.z+dz));n++;}
		if (n==2) return r;
	}


	//vorne
	if  ( (a.z>=vmax.z && v.z<0.0f) || (a.z<=vmax.z && v.z>0.0f) )
	{
		d = win_v_z/(vmax.z-a.z);
		dy = win_v_y / d;
		dx = win_v_x / d;
		if(vmin.y<=a.y+dy && vmax.y>=a.y+dy && vmin.x<=a.x+dx && vmax.x>=a.x+dx)
			{r.push_back(Point(a.x+dx, a.y+dy, vmax.z));n++;}
		if (n==2) return r;
	}

	//hinten
	if ( (a.z>=vmin.z && v.z<0.0f) || (a.z<=vmin.z && v.z>0.0f) )
	{
		d = win_v_z/(vmin.z-a.z);
		dy = win_v_y / d;
		dx = win_v_x / d;
		if(vmin.y<=a.y+dy && vmax.y>=a.y+dy && vmin.x<=a.x+dx && vmax.x>=a.x+dx)
			{r.push_back(Point(a.x+dx, a.y+dy, vmin.z));n++;}
		if (n==2) return r;
	}
	return r;
}
