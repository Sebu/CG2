#include "global.h" 

void gx_model::genExtraPoints()
{
    static float fmin=0.0f, fmax= 0.0f;
        
    VertList newpoints;
    GX_VEC3 *v1=NULL, *v2 =NULL;
    GX_VEC3 *tp = NULL;
    float e, ebasis = .01f * box->diaLength(); 
    
    cout << "Generiere extra Punkte" << endl;
    for (long i = 0; i< static_cast<int>(templ.size()) ;i++){
        GX_VEC3 *v1 = NULL, *v2 = NULL;
        Point *p = templ.at(i);
        e = ebasis;
        
        // Punkte aussen
        while (p != tp){
            if(v1!=NULL) {delete v1; v1=NULL;}
            v1 = new GX_VEC3(p->x+(p->nx*e),p->y+(p->ny*e),p->z+(p->nz*e),p->nx,p->ny,p->nz);
            VertPointerList t = collectKNearest(*v1, 1);
            tp = t.front();
            e /= 2.0f;
            if(tp->x == p->x || tp->y == p->y || tp->z == p->z) break;
           // cout << e << endl;
        }
           
        v1->fi = e; 
        kdtree->addPoint(*v1); 
        //newpoints.push_back(*v1);
        
        // Punkte innen
        e = -ebasis;
        while (p != tp){
            if(v2!=NULL) {delete v2; v2=NULL;}
            v2 = new GX_VEC3(p->x+(p->nx*e),p->y+(p->ny*e),p->z+(p->nz*e),p->nx,p->ny,p->nz);
            VertPointerList t = collectKNearest(*v2, 1);
            tp = t.front();
            e /= 2.0f;
            if(tp->x == p->x || tp->y == p->y || tp->z == p->z) break;
        }
        v2->fi = e;
        kdtree->addPoint(*v2); 
        //newpoints.push_back(*v2);
        //cout << i << endl;
        
        if (v2->fi < fmin) fmin = v2->fi;
        if (v1->fi > fmax) fmax = v1->fi;
        //cout << "min: " << fmin << "\t\t max: " << fmax << endl;
    }
    //if(kdtree) kdtree->addPoints(newpoints);   
}



gx_model::gx_model()
{
	kdtree = NULL;
}




// .off daten laden und im vector verts ablegen
void gx_model::load_off(const char* filename, int r)
{
	fstream fs;
	char	id[5];
	int		edges;
	int		faces;
	GX_VEC3	vert;
    GX_VEC3	normal;
    
	cout << "LADE MODELDATEN (bei vielen Punkten kann dies etwas dauern)..." << endl;

	fs.open(filename, fstream::in);

	fs >> id;
	fs >> vertcount >> faces >> edges;

	cout << "Anzahl der Punkte " << vertcount << endl;

	for(long i = 0; i<vertcount; i++)
	{
		fs >> vert.x >> vert.y >> vert.z >> normal.x >> normal.y >> normal.z;
		
        float fac = (float)r;
		GX_VEC3 *v = new GX_VEC3(vert.x,vert.y,vert.z,fac*normal.x,fac*normal.y,fac*normal.z);
			
        v->fi = 0.0f;

        verts.push_back(*v);

        templ.push_back(v);

	}
	
	box = new gx_aabb( verts,vertcount );

	fs.close();
}

// kd-tree aus modeldaten erzeugen
void gx_model::createKdtree(int n)
{
	cout << "GENERIERE KD-BAUM (bei vielen Punkten kann dies etwas dauern)... ";
	kdtree = new gx_kdtree();
	gx_kdtree::max_in_node = n;
	kdtree->box = new gx_aabb(box->vmax, box->vmin);
	kdtree->create(verts);
	cout << "OK" << endl;
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
			for (long i = 0; i< static_cast<int>(verts.size());i++){
    		   glNormal3fv(verts.at(i).n);
               glVertex3dv(verts.at(i).v);
            }
		glEnd();
	}
}

VertList gx_model::rayMarch()
{
    Point a,b;
    GLdouble	model[16],
				proj[16];
	GLint		view[4];
	Point      *x = NULL;
	float fi1, fi2;
    float scale = box->diaLength();
	GX_VEC3 z;
	GX_VEC3 start, end;
	
    VertList r;
	glGetDoublev(GL_MODELVIEW_MATRIX,model); 
	glGetDoublev(GL_PROJECTION_MATRIX,proj); 
	glGetIntegerv(GL_VIEWPORT,view); 


    cout << "RAYMARCHING ... ";

    // um bild in datei zu schreiben
    unsigned char *image;
    image = (unsigned char*)malloc(640*480*3);
    memset(image, 0, 640*480*3);


    for (int yi=0; yi<480; yi++)
	{
    
        for (int xi=0; xi<640; xi++)
	    {

            gluUnProject( xi, yi, 0.0f , model , proj , view , &a.x, &a.y, &a.z );
	        gluUnProject( xi, yi, 1.0f , model , proj , view , &b.x, &b.y, &b.z );
	        
	        Vector v = (b-a);
	        v.normalize();
	        //render_ray(a,v); //funzt
            VertList l = box->rayIntersect(a,v);
            
            if (l.size()==2) {
                if ( dist(a,l.front()) < dist(a,l.back()) )
                    {start = l.front(); end = l.back();}
                else
                    {start = l.back(); end = l.front();}
                }
            if (l.size()==1) {start = a; end = l.back();}
            
            if(!l.empty()){
                
                
                x = genPointConst(start.x,start.y,start.z, *this);  
                fi1 = x->fi;

                VertPointerList d = collectKNearest(start, 1);
                float dis = dist( start,*d.front());
                z = start+v*dis;
                while ( true )
                {

                    x = genPointConst(z.x,z.y, z.z, *this);
                    fi2 = x->fi;
                    if (fi1*fi2 < 0.0f){z = z + v*fi2; x->fi = 1.0f - dist(z,a)/scale; break;}
                    if (!box->pointIn(&z)){x->fi = 0.0f; break;}
                    fi1 = fi2;
                    z = z + v*(20.0f);

                }

                image[(xi+yi*640)*3] = (char)(x->fi*255.0f);
                image[(xi+yi*640)*3+1] = (char)(x->fi*255.0f);
                image[(xi+yi*640)*3+2] = (char)(x->fi*255.0f);
                r.push_back(*x);
                //free(x);
            }
        }
        cout << "*";
    }
    writeScreen("testray.bmp", 640, 480, image);
    free(image);
    cout << "OK" << endl;
    return r;

}

VertList gx_model::sphereTrace()
{
    Point a,b;
    GLdouble	model[16],
				proj[16];
	GLint		view[4];
	Point      *x = NULL;
	float fi1, fi2;
    float scale = box->diaLength();
	GX_VEC3 z;
	GX_VEC3 start, end;
	
    VertList r;
	glGetDoublev(GL_MODELVIEW_MATRIX,model); 
	glGetDoublev(GL_PROJECTION_MATRIX,proj); 
	glGetIntegerv(GL_VIEWPORT,view);
    
    
    cout << "SPHERETRACING ... ";
     
    // um bild in datei zu schreiben
    unsigned char *image;
    image = (unsigned char*)malloc(640*480*3);
    memset(image, 0, 640*480*3);

    
    for (int yi=0; yi<480; yi++)
	{
    
        for (int xi=0; xi<640; xi++)
	    {

            gluUnProject( xi, yi, 0.0f , model , proj , view , &a.x, &a.y, &a.z );
	        gluUnProject( xi, yi, 1.0f , model , proj , view , &b.x, &b.y, &b.z );
	        
	        Vector v = (b-a);
	        v.normalize();
	        //render_ray(a,v); //funzt
            VertList l = box->rayIntersect(a,v);
            
            if (l.size()==2) {
                if ( dist(a,l.front()) < dist(a,l.back()) )
                    {start = l.front(); end = l.back();}
                else
                    {start = l.back(); end = l.front();}
                }
            if (l.size()==1) {start = a; end = l.back();}
            
            if(!l.empty()){
              
                x = genPointConst(start.x,start.y,start.z, *this);  
                fi1 = x->fi;

                VertPointerList d = collectKNearest(start, 1);
                float dis = dist( start,*d.front());
                z = start+v*dis;
                while ( true )
                {

                    x = genPointConst(z.x,z.y, z.z, *this);
                    fi2 = x->fi;
                    if (fabs(fi1) < 0.003f) {x->fi = 1.0f - dist(z,a)/scale; break;}
                    if (!box->pointIn(&z)){x->fi = 0.0f; break;}
                    fi1 = fi2;
                    z = z + v*fi2;

                }
                image[(xi+yi*640)*3] = (char)(x->fi*255.0f);
                image[(xi+yi*640)*3+1] = (char)(x->fi*255.0f);
                image[(xi+yi*640)*3+2] = (char)(x->fi*255.0f);
                r.push_back(*x);
            }
        }
        cout << "*";
    }
    writeScreen("testsphere.bmp", 640, 480, image);
    free(image);
    cout << "OK" << endl;
    return r;

}

