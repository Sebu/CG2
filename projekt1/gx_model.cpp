#include "global.h" 

gx_model::gx_model()
{
	kdtree = NULL;
}

// .off daten laden und im vector verts ablegen
void gx_model::load_off(const char* filename)
{
	fstream fs;
	char	id[4];
	int		edges;
	int		faces;
	GX_VEC3	vert;

	cout << "LADE MODELDATEN (bei vielen Punkten kann dies etwas dauern)..." << endl;

	fs.open(filename, fstream::in);

	fs >> id;
	fs >> vertcount >> faces >> edges;

	cout << "Anzahl der Punkte " << vertcount << endl;

	for(long i = 0; i<vertcount; i++)
	{
		fs >> vert.x >> vert.y >> vert.z;
		
		GX_VEC3 *v = new GX_VEC3(vert.x,vert.y,vert.z);
		verts.push_back(*v);
	}
	
	box = new gx_aabb(verts,vertcount);

	fs.close();
}

// kd-tree aus modeldaten erzeugen
void gx_model::create_kdtree(int n)
{
	cout << "GENERIERE KD-BAUM (bei vielen Punkten kann dies etwas dauern)..." << endl;
	kdtree = new gx_kdtree();
	gx_kdtree::max_in_node = n;
	kdtree->box = new gx_aabb(box->vmax, box->vmin);
	kdtree->create(verts);
}

// k naechsten punkt ermitteln
VertPointerList gx_model::collectKNearest(GX_VEC3& p, int k)
{
	VertPointerList vlist;
	if (vertcount < k) {
		cout << "k ist zu gross (alter wert wird weiter verwendet)" << endl;
		return vlist;
	}
	
	gx_kdtree::maxdist = MAX_SIZE;
	vlist = kdtree->collectKNearest(p, k, vlist); 
	
	VertPointerList out;

	if( !vlist.empty() )
			cout << "k naechster: " << vlist[k-1]->x << " " << vlist[k-1]->y << " " << vlist[k-1]->z << endl;
	return vlist;
}

// punkte im radius eins punktes suchen
VertPointerList gx_model::collectInRadius(GX_VEC3& p, float radius)
{
	VertPointerList vlist;
	return kdtree->collectInRadius(p,radius, vlist); 
}

// kd-tree struktur visualisieren
void gx_model::visStructure()
{
	// alpha blending an
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (kdtree) kdtree->visStructure();
	else box->render();

	// alpha blending aus
	glDisable(GL_BLEND);
}

// model rendern
void gx_model::render()
{
	// wenn eine kd struktur vorhanden ist diese rendern
	if (kdtree) kdtree->render();
	// sonst einfach alle punkte durchlaufen
	else {
		glPointSize(5.0f);
		glBegin(GL_POINTS);
			glColor3f( 0.0f, 1.0f, 0.0f);
			for (long i = 0; i< static_cast<int>(verts.size());i++)
				glVertex3f(verts.at(i).x, verts.at(i).y ,verts.at(i).z);
		glEnd();
	}
}
