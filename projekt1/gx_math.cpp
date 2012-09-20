#include "global.h"

// "konstruktor"
GX_VEC3::GX_VEC3( GLfloat _x, GLfloat _y, GLfloat _z )
{
    x = _x;
    y = _y;
    z = _z;
}

// distance wert von 2 punkten berechnen
float dist(GX_VEC3& a, GX_VEC3& b) {
	float xd = a.x - b.x;
	float yd = a.y - b.y;
	float zd = 0; // a.z - b.z;
	return sqrt( xd*xd + yd*yd + zd*zd );
}

// distance wert von 2 punkten vergleichen
bool compare_dist(const GX_VEC3& a, const GX_VEC3& b) {
    return a.distance < b.distance;
}


// distance wert von 2 punkten vergleichen
bool compare_dist_pointer(const PGX_VEC3& a, const PGX_VEC3& b) {
    return a->distance < b->distance;
}
