#include "global.h"

float gx_kdtree::maxdist;
int gx_kdtree::max_in_node;

// k naechsten punkt finden
VertPointerList gx_kdtree::collectKNearest(GX_VEC3& p, int k, VertPointerList vlist)
{
	// wenns wir uns nicht in einem blatt befinden
	// dann einfach schauen welche box naeher an unserem punkt ist
	// und zuerst in diese schauen
	if (!leaf){
		float ld = children[0].distance(p);
		float rd = children[1].distance(p); 
		if(rd < ld)
		{
			if (rd <= gx_kdtree::maxdist ) vlist = children[1].collectKNearest(p, k, vlist);
			if (ld <= gx_kdtree::maxdist ) vlist = children[0].collectKNearest(p, k, vlist);
		} else {
			if (ld <= gx_kdtree::maxdist ) vlist = children[0].collectKNearest(p, k, vlist);
			if (rd <= gx_kdtree::maxdist ) vlist = children[1].collectKNearest(p, k, vlist);
		}

	}
	else {
		for (int i = 0; i < static_cast<int>(points.size()); i++)
		{
			float d = points[i].distance = dist(p, points[i]);

			// die ersten k-1 in eine liste packen
			if ( static_cast<int>(vlist.size()) < k-1)
				vlist.push_back(&points[i]);

			// das k-te element in die liste packen diese 
			// sortieren und die groesste entfernung zu  p  merken 
			else if( static_cast<int>(vlist.size()) == k-1) 
			{
				vlist.push_back(&points[i]);
				sort(vlist.begin(),vlist.end(), compareDistPointer );
				gx_kdtree::maxdist = vlist.back()->distance;

			}
			// punkt gefunden der naeher an p ist als der k-te punkt ?
			// 
			else if( d < gx_kdtree::maxdist )
			{
                /*vlist.insert(
					std::upper_bound( vlist.begin(),
									  vlist.end(),
									  &points[i],
									  compareDistPointer ),
					&points[i] );

*/

					
				vlist.push_back(&points[i]);
				sort(vlist.begin(),vlist.end(), compareDistPointer );
				vlist.pop_back();
				gx_kdtree::maxdist = vlist.back()->distance;
			}

		}	
	}
	return vlist;
}
	
// alle punkte in einem radius finden
VertPointerList gx_kdtree::collectInRadius(GX_VEC3& p, float radius, VertPointerList vlist)
{
    //cout << "test" << endl;
	// wenn punkt zuweit von box entfernt dann einfach vlist unveraendert zurueck
	if (distance(p) >= radius) return vlist;

	if (!leaf){
		
		// wenn kinderbox im radius liegt dann im kind punkte in vlist sammeln
		if(children[0].distance(p) < radius)
			vlist = children[0].collectInRadius(p, radius, vlist);
		if(children[1].distance(p) < radius)
			vlist = children[1].collectInRadius(p, radius, vlist);
	}
	else {
		// wenn punkt im radius ist diesen in vlist sammeln
		for (int i = 0; i < static_cast<int>(points.size()); i++) {
			if (dist(p, points.at(i)) < radius) {
				vlist.push_back(&points.at(i));
			}
		}
	}
	return vlist;
}

// konstruktor
gx_kdtree::gx_kdtree()
{
	leaf = true;
	children = NULL;
	box = NULL;

}


// Abstand der der bbox zu einem punkt bestimmen
float gx_kdtree::distance(GX_VEC3& point)
{
	return box->distance(point);
}

void gx_kdtree::addPoints(VertList& pointlist)
{
    cout << "Fuege Punkte hinzu" <<endl;
    for (long j = 0; j<static_cast<int>(pointlist.size()); j++){
        Point p = pointlist.back();
        pointlist.pop_back();
        addPoint(p);
           
    }
}

bool gx_kdtree::addPoint(Point& p)
{

    bool status = false;
    if(!leaf)
    {
      	float ld = children[0].distance(p);
		float rd = children[1].distance(p); 
		if(rd < ld)
		{
			status = children[1].addPoint(p);
			if(status == false) status = children[0].addPoint(p);
		} else {
			status = children[0].addPoint(p);
			if(status == false) status = children[1].addPoint(p);
		}  
    }
    else if( box->pointIn(&p) ) { points.push_back(p); status=true; }
    return status;
}

void gx_kdtree::create(VertList& pointlist)
{

	points = pointlist;
	double avg = 0.0f;

   	//laengste achse bestimmen
	int axis = box->longSide();
	float l = box->vmax.v[axis] - box->vmin.v[axis];
	

	if (static_cast<int>(points.size()) > max_in_node) // teilen wenn min n punkte und box > 1.0f
	{

       // distanz zum rand der box berechnen (fuer median berechnung)
	   for (long j = 0; j<static_cast<int>(points.size()); j++)
 			  points.at(j).distance = (points.at(j).v[axis]  - box->vmin.v[axis]);	

	    // median berechnen (nach distanz sortieren und punkt in der mitte auswaehlen)
	    sort(points.begin(), points.end(), compareDist );

	    int m = static_cast<int>(points.size())/2; // mitte

	    avg = points[m].distance;
	    
        VertList rlist;
	    VertList llist;

	    for (long j = static_cast<int>(points.size())-1; j>=0; j--){
		    if (j<m) llist.push_back(points.back());
		    else rlist.push_back(points.back());
		    points.pop_back();		
        }
        points.clear();
		leaf = false;

		// wenn kein blatt dann 2 neue kinder erzeugen
		children = new gx_kdtree[2];
		children[0].box = new gx_aabb(box->vmax, box->vmin);	//	links
		children[1].box = new gx_aabb(box->vmax, box->vmin);	//	rechts

		// median zur teilung verwenden
		children[0].box->vmax.v[axis] -= l-avg;
		children[1].box->vmin.v[axis] += avg;

		// kinder "erzeugen"/fuellen
		children[0].create(llist);
		children[1].create(rlist);


		llist.clear();
		rlist.clear();
		return;
	} 


}

void gx_kdtree::visStructure()
{	

	if (leaf)
	{
		// irgendeine farbe berechnen
		glColor4f( box->vmin.x*5.2f, box->vmin.z*6.4f , box->vmin.y*3.6f, .1f );
	
		//box zeichnen
		if(leaf && box)	box->renderSolid();
		glColor4f( .62f, 1.0f , .6f, .28f );
		if(leaf && box)	box->render();
	}
	//kinder rendern
	else if(!leaf && children) 
	{
		children[0].visStructure();
		children[1].visStructure();
	}


}

//kd-tree rendern
void gx_kdtree::render()
{
   	glPointSize(2.0f);

	if(leaf) { renderLeaf(); return; }
	if(!leaf && children) 
	{
		children[0].render();
		children[1].render();
	}
}


// blatt rendern
void gx_kdtree::renderLeaf(){

	glBegin(GL_POINTS);
			glColor3f(1.0f,1.0f,1.0f);
			for (int i = 0; i<static_cast<int>(points.size());i++){
    		   glNormal3fv(points.at(i).n);
               glVertex3dv(points.at(i).v);
            }
	glEnd();

}
