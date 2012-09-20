#include "global.h"

double dihedralAngle(GX_HEDGE &e, GX_VEC3 &v)
{
        GX_VEC3 v1 =  *(e.opp->next->next->sv ) - v;   
                  
        GX_VEC3 v2 =  *(e.opp->sv) - v;
        GX_VEC3 v3 =  *(e.prev->sv ) - v; 

        v1.normalize(); v2.normalize(); v3.normalize();

        GX_VEC3 r1 = v1*v2;
        GX_VEC3 r2 = v2*v3;
        r1.normalize(); r2.normalize();
       
        return angle(r1,r2);


}

double vertAngles(GX_VEC3 &v)
{
    double r = 0.0f;

  
    std::list<GX_HEDGE*>::const_iterator iter;
    for (iter=v.es.begin(); iter != v.es.end(); iter++)
    {    
         double neu = dihedralAngle( *(*iter) , v);
        // cout << neu << endl;
         if(neu > r) r = neu; 
    }

    return r;
    
}

double egdeAngles(GX_HEDGE &e)
{
    GX_VEC3 vm = *(e.opp->sv) - *e.sv;
    GX_VEC3 vl = *(e.prev->sv) - *e.sv;
    GX_VEC3 vr = *(e.opp->prev->sv) - *e.sv;
    
    float r1 = angle(vm,vl);
    float r2 = angle(vm,vr);
    //cout << r << endl;
    if (r1 > r2) return r1;
    else return r2; 
    
}

double angle(GX_VEC3& _a, GX_VEC3& _b)
{
    GX_VEC3 a = _a;
    GX_VEC3 b = _b;
    a.normalize(); b.normalize();
    return (acos(a.x*b.x + a.y*b.y + a.z*b.z)*180.0f)/PI;
}

bool splitable(GX_HEDGE* e, gx_model &m1, gx_model &m2)
{
   
    if ( e->b ) return false;

    GX_HEDGE    *o = e->opp;
    GX_HEDGE    *en = e->next;
    GX_HEDGE    *ep = e->prev;
    GX_HEDGE    *on = o->next;
    GX_HEDGE    *op = o->prev;
    
    // mittelpunkt berechnen
    GX_VEC3 a = *(e->next->sv);
    GX_VEC3 b = *(o->next->sv);
    
// EXPORT IN FUNKTION - BEGIN

    // 1 neuen knoten erzeugen
    GX_VEC3 *c = new GX_VEC3();
    *c = a*0.5f + b*0.5f;
    
    double eps = 0.01f;

    Point *xp = genPointConst(c->x, c->y, c->z, m2);
    double err = xp->fi*xp->fi;
    

    if (err < m1.qual) return false;
 
    Point *xx = genPointConst(c->x+eps, c->y, c->z, m2);
    Point *xy = genPointConst(c->x, c->y+eps, c->z, m2);
    Point *xz = genPointConst(c->x, c->y, c->z+eps, m2);
    xp->nx = (xx->fi-xp->fi)/eps;
    xp->ny = (xy->fi-xp->fi)/eps;
    xp->nz = (xz->fi-xp->fi)/eps;
   
    float len = sqrt(xp->nx*xp->nx+xp->ny*xp->ny+xp->nz*xp->nz);
    if (len == 0.0f) return false;
    
    xp->nx /= len; xp->ny /= len; xp->nz /= len;

    c->x += -xp->nx*xp->fi;
    c->y += -xp->ny*xp->fi;
    c->z += -xp->nz*xp->fi;
    c->nx = xp->nx;    c->ny = xp->ny; c->nz = xp->nz;            

// EXPORT IN FUNKTION - END
   
    // 6 neue kanten erzeugen
    GX_HEDGE *e1 = new GX_HEDGE(c);
    GX_HEDGE *e2 = new GX_HEDGE(c); 
    GX_HEDGE *e3 = new GX_HEDGE(c); 

    GX_HEDGE *e4 = new GX_HEDGE(e->next->sv);  
    GX_HEDGE *e5 = new GX_HEDGE(e->prev->sv);   
    GX_HEDGE *e6 = new GX_HEDGE(o->prev->sv);   
    
    GX_HEDGE *e7 = new GX_HEDGE(c); 
    GX_HEDGE *e8 = new GX_HEDGE(e->sv); 
    
    // sv kantenlisten updaten
    c->es.push_back(e7);
    c->es.push_back(e1);
    c->es.push_back(e2);
    c->es.push_back(e3);
    
    e1->opp = e4;
    e2->opp = e5;
    e3->opp = e6;
    e4->opp = e1;
    e5->opp = e2;
    e6->opp = e3;
    e7->opp = e8;
    e8->opp = e7;
    
        
    // umlenken
    e1->next = e->next;
    e1->prev = e5;
    e2->next = e->prev;
    e2->prev = e;
    e3->next = o->prev;
    e3->prev = e4;
    e4->next = e3;
    e4->prev = o->prev;
    e5->next = e1;
    e5->prev = e->next;
    e6->next = o;
    e6->prev = o->next;
    e8->next = e2;
    e8->prev = e->prev;
    e7->next = o->next;
    e7->prev = e6;
    
    if ( vertAngles(*c) > 10.f )  return false;

    return true; 
}

bool splitEdge(GX_HEDGE* e, gx_model &m1, gx_model &m2)
{


    GX_HEDGE    *o = e->opp;
    GX_HEDGE    *en = e->next;
    GX_HEDGE    *ep = e->prev;
    GX_HEDGE    *on = o->next;
    GX_HEDGE    *op = o->prev;
    
    // mittelpunkt berechnen
    GX_VEC3 a = *(e->next->sv);
    GX_VEC3 b = *(o->next->sv);
    
    // 1 neuen knoten erzeugen
    GX_VEC3 *c = new GX_VEC3(   a.x*0.5f + b.x*0.5f,
                                a.y*0.5f + b.y*0.5f,
                                a.z*0.5f + b.z*0.5f,
                                0.0f,0.0f,0.0f);
    c->b = false;
            
    double eps = 0.01f;
    Point *xp = genPointConst(c->x, c->y, c->z, m2);
    
    cout << "spliting" << endl;
    
    Point *xx = genPointConst(c->x+eps, c->y, c->z, m2);
    Point *xy = genPointConst(c->x, c->y+eps, c->z, m2);
    Point *xz = genPointConst(c->x, c->y, c->z+eps, m2);
    xp->nx = (xx->fi-xp->fi)/eps;
    xp->ny = (xy->fi-xp->fi)/eps;
    xp->nz = (xz->fi-xp->fi)/eps;
    
    float len = sqrt(xp->nx*xp->nx +xp->ny*xp->ny +xp->nz*xp->nz);
    xp->nx /= len; xp->ny /= len; xp->nz /= len;
    
    c->x += -xp->nx*xp->fi;
    c->y += -xp->ny*xp->fi;
    c->z += -xp->nz*xp->fi;
    c->nx = xp->nx;    c->ny = xp->ny; c->nz = xp->nz;  
    c->fi = xp->fi*xp->fi;
    m1.erep += m1.crep;          
    //alt
    c->he = o;
    
    m1.verts.push_back(*c);
    m1.templ.push_back(c);
        
    // 6 neue kanten erzeugen
    GX_HEDGE *e1 = new GX_HEDGE(c);
    e1->type = 11;
    GX_HEDGE *e2 = new GX_HEDGE(c); 
    e2->type = 12;
    e2->temp = b.x;
    GX_HEDGE *e3 = new GX_HEDGE(c); 
    e3->type = 13;
    GX_HEDGE *e4 = new GX_HEDGE(e->next->sv);  
    e4->type = 14;
    GX_HEDGE *e5 = new GX_HEDGE(e->prev->sv); 
    e5->type = 15;
    GX_HEDGE *e6 = new GX_HEDGE(o->prev->sv);
    e6->type = 16;
    
    // sv kantenlisten updaten
    c->es.push_back(o);
    c->es.push_back(e1);
    c->es.push_back(e2);
    c->es.push_back(e3);
    ep->sv->es.push_back(e5);
    op->sv->es.push_back(e6);
    o->sv->es.push_back(e4);
    o->sv->es.remove(o);
    
    // c startpunkt ändern
    o->sv = c;
    o->type = 20;
    o->temp = len;
    //alt
    en->sv->he = e4;
    e->sv->he = e;
    ep->sv->he = ep;
    op->sv->he = op;

    e1->opp = e4;
    e2->opp = e5;
    e3->opp = e6;
    e4->opp = e1;
    e5->opp = e2;
    e6->opp = e3;
    
    // umlenken
    e1->next = e->next;
    e1->prev = e5;
    e2->next = e->prev;
    e2->prev = e;
    e3->next = o->prev;
    e3->prev = e4;
    e4->next = e3;
    e4->prev = o->prev;
    e5->next = e1;
    e5->prev = e->next;
    e6->next = o;
    e6->prev = o->next;
    e->next = e2;
    o->prev = e6;
    
    en->next = e5;
    en->prev = e1;
    ep->prev = e2;
    on->next = e6;
    op->next = e4;
    op->prev = e3;
    
    
    // 2 neue faces erzeugen
    GX_FACE *f1 = new GX_FACE(e1);
    GX_FACE *f2 = new GX_FACE(e4);
    GX_FACE *fe = e->f;
    GX_FACE *fo = o->f;
    
    e->f->e = e;
    o->f->e = o;

    en->f = f1;
    op->f = f2;
    
    e1->f = f1;
    e2->f = fe;
    e3->f = f2;
    e4->f = f2;
    e5->f = f1;
    e6->f = fo;
    
    m1.edges.push_back(e1); m1.edges.push_back(e2); m1.edges.push_back(e3);
    m1.edges.push_back(e4); m1.edges.push_back(e5); m1.edges.push_back(e6); 
    m1.faces.push_back(f1); m1.faces.push_back(f2);    
    
    return true;
      
}
bool collapseable(GX_HEDGE* e, gx_model &m1, gx_model &m2)
{
    //cout << "collapseable" << endl;
    std::list<GX_HEDGE*>::const_iterator iter;
    GX_HEDGE    *o = e->opp;
    
    // legaler schritt?    
    if ( ( e->b ) || ( o->b ) ||  (e->removed)  ||
     (o->removed) ||  e->sv->b ||  o->sv->b  ) return false;
    
    // neuen punkt berechnen
    GX_VEC3 a = *(o->sv);
    GX_VEC3 b = *(e->sv);
    GX_VEC3 c = a*0.5f + b*0.5f;


    // fehler ?
    if (fabs((a-b).length()) < 0.0000001f ) return false;
    
    // energy berechnen naja :)
    if(m1.k==0)
    {
        double err = genPointConst(c.x,c.y,c.z,m2)->fi;
        if( (-a.fi -b.fi +err*err -m1.crep) > 0.0f ) return false;
    }
                    
    // muss genau hier stehen warum weiss nur der mock
    c.b = false;
    
    // alle umliegenden kanten in unseren testpunkt packen
    for (iter=e->sv->es.begin(); iter != e->sv->es.end(); iter++)
             c.es.push_back((*iter));
    for (iter=o->sv->es.begin(); iter != o->sv->es.end(); iter++)
             c.es.push_back((*iter));

    // die toten kanten wieder raus
    c.es.remove(e);
    c.es.remove(o);
    c.es.remove(e->next);
    c.es.remove(o->next);
    
    if (c.es.size()>6) return false;
    // 70grad ist schon recht spit 50 geht im moment recht gut
    if ( vertAngles(c) > 60.f )  return false;
    
    return true;  
}

bool collapseEdge(GX_HEDGE* e, gx_model &m1, gx_model &m2)
{
    GX_HEDGE    *o = e->opp;
    
    GX_VEC3 a = *(o->sv);
    GX_VEC3 b = *(e->sv);
    
    std::list<GX_HEDGE*>::const_iterator iter;
    o->sv->x = a.x*0.5f + b.x*0.5f;
    o->sv->y = a.y*0.5f + b.y*0.5f;
    o->sv->z = a.z*0.5f + b.z*0.5f;
            
   //cout << "collapsing" << endl;
    
    if(m1.k==0)
    {
        double err = genPointConst(o->sv->x,o->sv->y,o->sv->z,m2)->fi;
        //m1.edist += -a.fi-b.fi+err*err;
        e->next->sv->fi = err*err;
    }    
    m1.erep -= m1.crep;
                
    e->removed = true;
    o->removed = true;
    e->prev->removed = true;
    e->next->removed = true;
    o->prev->removed = true;
    o->next->removed = true;

    e->sv->es.remove(e);
    e->sv->es.remove(o->next);
    o->sv->es.remove(o);
    o->sv->es.remove(e->next);   
    e->prev->sv->es.remove(e->prev);
    o->prev->sv->es.remove(o->prev);
    
    
    e->next->opp->opp = e->prev->opp;
    e->prev->opp->opp = e->next->opp;

    o->next->opp->opp = o->prev->opp;
    o->prev->opp->opp = o->next->opp;
    
    
    e->f->removed = true;
    o->f->removed = true;
    
    
    for (iter=e->sv->es.begin(); iter != e->sv->es.end(); iter++)
             o->sv->es.push_back((*iter));
             
    for (iter=e->sv->es.begin(); iter != e->sv->es.end(); iter++)
    {
             (*iter)->sv = o->sv;
             (*iter)->type = 40;
             (*iter)->temp = o->sv->x;
             
    }
        
    e->sv->es.clear();
         
    //e->next->sv->he = o->prev->opp;
    
    return true;
    
}

bool swapable(GX_HEDGE* e, gx_model &m1, gx_model &m2)
{
    //cout << "swapable" << endl;
    GX_HEDGE    *o = e->opp,
                *op = o->prev,
                *on = o->next,
                *ep = e->prev,
                *en = e->next;
                
    if ( e->b || e->removed || (egdeAngles(*e) > 50.0f ) || (egdeAngles(*o) > 50.0f ) ) 
    { return false; } 
  

    
    float l = (*(e->prev->sv) - *(o->prev->sv)).length();

    
/// <NARFFFF> 
    // neuen punkt berechnen
    GX_VEC3 a1 = *(o->sv);
    GX_VEC3 b1 = *(e->sv);
    GX_VEC3 c1 = a1*0.5f + b1*0.5f;
    GX_VEC3 a2 = *(o->prev->sv);
    GX_VEC3 b2 = *(e->prev->sv);
    GX_VEC3 c2 = a2*0.5f + b2*0.5f;
    // energy berechnen jaja :)
    
  //  cout << a2.x << " " << a2.y << " " << a2.z << endl;
  //  cout << b2.x << " " << b2.y << " " << b2.z << endl;
  //  cout << e->prev->type << "  " << e->prev->temp << endl;    

    //mittelpunktgewichte vergleichen  
    double err1 = genPointConst(c1.x,c1.y,c1.z,m2)->fi;
    double err2 = genPointConst(c2.x,c2.y,c2.z,m2)->fi;
    

  
    if (fabs(err1)<fabs(err2)) return false;

  
/// </NARFFFF>

    return true;
}

bool swapEdge(GX_HEDGE* e, gx_model &m1, gx_model &m2)
{
    
    GX_HEDGE    *o = e->opp,
                *op = o->prev,
                *on = o->next,
                *ep = e->prev,
                *en = e->next;

    // cout << "swapping" << endl;    
    
    // faces und verts anpassen
    e->f->e = e;
    o->f->e = o;
    
    // aus sv liste entfernen
    e->sv->es.remove(e);
    o->sv->es.remove(o);
    
    // alt    
    //e->sv->he = on; 
    //o->sv->he = en;
    
    // neues start vertex setzten
    e->sv = e->prev->sv;
    e->type = 2;
    e->sv->es.push_back(e);
    o->sv = o->prev->sv;
    o->type = 2;
    o->sv->es.push_back(o);
        
    // e selber umlenken
    e->next = op;
    e->prev = en;
    
    //o umlenken
    o->next = ep;
    o->prev = on;
    
    //e-next + o-next umlenken
    en->next = e;
    en->prev = op;
    on->next = o;
    on->prev = ep;
    
    //e-prev + o-prev umlenken
    ep->next = on;
    ep->prev = o;
    ep->f = o->f;
    op->next = en;
    op->prev = e;
    op->f = e->f;
    
    return true;
    
};

void optimize(gx_model &m1, gx_model &m2)
{
    std::list<GX_HEDGE*>::const_iterator iter;
    std::queue<GX_HEDGE*> list;
    for(int i=0; i < static_cast<int>(m1.edges.size()); i++ )
        list.push( m1.edges[i] );
        
    int index = 0;
    while (!list.empty())
    {
        bool done = false;
        
        GX_HEDGE *e = list.front();
        list.pop();
        
        if (e->removed) continue;
        

        

        // EDGE COLLAPSE
       
        GX_HEDGE *o = e->opp;
        GX_VEC3  *s = e->opp->sv;
        
        
        if( collapseable(e, m1, m2) )
        {
                collapseEdge(e, m1, m2);
                e->type = 1;
                for (iter=s->es.begin(); iter != s->es.end(); iter++)
                {
                    list.push( (*iter) );
                    list.push( (*iter)->next );
                }
                    
                done = true;
        }
        
       
        if( !done && swapable(e, m1, m2)   )  
        { 
   
            swapEdge(e, m1, m2);
            if(dihedralAngle(*e, *(e->sv) ) < 5.0f) done = true; 
            if(done){
                 list.push( e );
                 list.push( e->next );
                 list.push( e->prev );
                 list.push( e->opp->next );
                 list.push( e->opp->prev );
            }
            //swap back
            if(!done) swapEdge(e, m1, m2);
            
            e->type = 2;
        }
        
         
       
        if( m1.k==0 && !done && splitable(e, m1, m2) )  
        { 

            //for (iter=s->es.begin(); iter != s->es.end(); iter++)
             //   list.push( (*iter) );   
            splitEdge(e, m1, m2);
            e->type = 3;
                
             done = true;
        }   
        
        
        
        if (m1.k>0)
        {
            if (m1.crep*(float)m1.k >= m1.erep) break;
            if(done) list.push(e);
            
        }
        //if (e->b || e->opp->b ) cout << "bound " << index++ << endl;

    }
    GX_VEC3 *a = new GX_VEC3(1.0f,.0f,.0f);
    GX_VEC3 *b = new GX_VEC3(1.0f,.0f,.0f);
    
    cout << angle( *a,*b )<< endl; 

       

};


Point *genPointConst(float posx, float posy ,float posz, gx_model& o)
{
    static float fmin=0.0f, fmax= 0.0f;
    VertPointerList plist;
             
    Point *xp = new Point(posx,posy,posz); 
                    
    float h = 0.0f;
    plist = o.collectKNearest(*xp, 4);
    h = dist(*xp,*plist.back());

                
    double part1 = 0.0f; double part2 = 0.0f;

    
    for(int i=0; i < static_cast<int>(plist.size()); i++){
        float x = plist.at(i)->x;
        float y = plist.at(i)->y;
        float z = plist.at(i)->z;
        float fi = plist.at(i)->fi;
               
        float d = dist(*xp,*plist.at(i));
        double wend = 0.0f;
                    
        d = d/h;
        wend = (1.0f - d)*(1.0f - d)*(1.0f - d)*(1.0f - d)*(4.0f*d+1.0f);
      
        part1 += wend;
        part2 += wend * fi;

    }
                
    //  f(x) = b^t(x) * c(x)
    xp->fi = (1.0f/part1) * part2;
    if (xp->fi < fmin) fmin = xp->fi;
    if (xp->fi > fmax) fmax = xp->fi;
    // cout << "min: " << fmin << "\t\t max: " << fmax << endl;
    
    return xp;
}

VertList genPoints(int n, int m, int k, gx_model& o,
                    Point*(*genPoint)(float posx, float posy ,float posz, gx_model& o) )
{
       
    VertList l;
       
    // Schrittweite berechnen 
    float stepx = (o.box->vmax.x - o.box->vmin.x) / (float)(n-1);
    float stepy = (o.box->vmax.y - o.box->vmin.y) / (float)(m-1);
    float stepz = (o.box->vmax.z - o.box->vmin.z) / (float)(k-1);

    
    float posz = o.box->vmin.z;
    for(int ziter=0; ziter<k; ziter++){ // z
        float posy = o.box->vmin.y;
        for(int yiter=0; yiter<m; yiter++){ // y
            float posx = o.box->vmin.x;
            for(int xiter=0; xiter<n; xiter++){   // x
            
                float e = 0.001f;
                Point *xp = (genPoint)(posx, posy, posz, o);
                Point *xx = (genPoint)(posx+e, posy, posz, o);
                Point *xy = (genPoint)(posx, posy+e, posz, o);
                Point *xz = (genPoint)(posx, posy, posz+e, o);
                xp->nx = (xx->fi-xp->fi)/e;
                xp->ny = (xy->fi-xp->fi)/e;
                xp->nz = (xz->fi-xp->fi)/e;
                
                l.push_back(*xp);
         
                posx += stepx;
                cout << "*";
            }
            posy += stepy;
        }
        posz += stepz;
    }
    return l;                 
}

VertList genTrisFine(int n, int m, int k, VertList& points)
{
    VertList l;
    for(long z=0; z<k-1; z++){
        for(long y=0; y<m-1; y++){
            for(long x=0; x<n-1; x++){
                VertList g;

                

                g.push_back(points.at(x   +(y+1)*n +  z*n*m) );
                g.push_back(points.at(x+1 +(y+1)*n +  z*n*m) );
                g.push_back(points.at(x+1 + y*n +     z*n*m) );
                g.push_back(points.at(x +   y*n +     z*n*m) );

                g.push_back(points.at(x   +(y+1)*n +  (z+1)*n*m) );
                g.push_back(points.at(x+1 +(y+1)*n +  (z+1)*n*m) );      
                g.push_back(points.at(x+1 + y*n +     (z+1)*n*m) );
                g.push_back(points.at(x +   y*n +     (z+1)*n*m) );


                               
                l = polygonise(g,.0f,l);  
                g.clear(); 
            }
        }
    }
    return l;
}

VertList genTris(int n, int m, int k, VertList& points)
{
    VertList l;
    for(long z=0; z<k-2; z+=2){
        for(long y=0; y<m-2; y+=2){
            for(long x=0; x<n-2; x+=2){
                VertList g;

                

                g.push_back(points.at(x   +(y+2)*n +  z*n*m) );
                g.push_back(points.at(x+2 +(y+2)*n +  z*n*m) );
                g.push_back(points.at(x+2 + y*n +     z*n*m) );
                g.push_back(points.at(x +   y*n +     z*n*m) );

                g.push_back(points.at(x   +(y+2)*n +  (z+2)*n*m) );
                g.push_back(points.at(x+2 +(y+2)*n +  (z+2)*n*m) );      
                g.push_back(points.at(x+2 + y*n +     (z+2)*n*m) );
                g.push_back(points.at(x +   y*n +     (z+2)*n*m) );


                               
                l = polygonise(g,.0f,l);  
                g.clear(); 
            }
        }
    }
    return l;
}

VertList polygonise(VertList& grid, double isolevel, VertList& verts)
{
   long i,ntriang;
   long cubeindex;
   GX_VEC3 vertlist[12];

int edgeTable[256]={
0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0   };
int triTable[256][16] =
{{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};

   /*
      Determine the index into the edge table which
      tells us which vertices are inside of the surface
   */
   cubeindex = 0;
   if (grid.at(0).fi < isolevel) cubeindex |= 1;
   if (grid.at(1).fi < isolevel) cubeindex |= 2;
   if (grid.at(2).fi < isolevel) cubeindex |= 4;
   if (grid.at(3).fi < isolevel) cubeindex |= 8;
   if (grid.at(4).fi < isolevel) cubeindex |= 16;
   if (grid.at(5).fi < isolevel) cubeindex |= 32;
   if (grid.at(6).fi < isolevel) cubeindex |= 64;
   if (grid.at(7).fi < isolevel) cubeindex |= 128;

   /* Cube is entirely in/out of the surface */
   if (edgeTable[cubeindex] == 0)
      return verts;

   /* Find the vertices where the surface intersects the cube */
   if (edgeTable[cubeindex] & 1)
      vertlist[0] = vertexInterp(isolevel,grid.at(0),grid.at(1));
   if (edgeTable[cubeindex] & 2)
      vertlist[1] = 
         vertexInterp(isolevel,grid.at(1), grid.at(2) );
   if (edgeTable[cubeindex] & 4)
      vertlist[2] = 
         vertexInterp(isolevel,grid.at(2), grid.at(3) );
   if (edgeTable[cubeindex] & 8)
      vertlist[3] = 
         vertexInterp(isolevel,grid.at(3), grid.at(0) );
   if (edgeTable[cubeindex] & 16)
      vertlist[4] = 
         vertexInterp(isolevel,grid.at(4), grid.at(5) );
   if (edgeTable[cubeindex] & 32)
      vertlist[5] = 
         vertexInterp(isolevel,grid.at(5), grid.at(6) );
   if (edgeTable[cubeindex] & 64)
      vertlist[6] = 
         vertexInterp(isolevel,grid.at(6),grid.at(7) );
   if (edgeTable[cubeindex] & 128)
      vertlist[7] = 
         vertexInterp(isolevel,grid.at(7),grid.at(4) );
   if (edgeTable[cubeindex] & 256)
      vertlist[8] = 
         vertexInterp(isolevel,grid.at(0),grid.at(4) );
   if (edgeTable[cubeindex] & 512)
      vertlist[9] = 
         vertexInterp(isolevel,grid.at(1),grid.at(5) );
   if (edgeTable[cubeindex] & 1024)
      vertlist[10] = 
         vertexInterp(isolevel,grid.at(2),grid.at(6) );
   if (edgeTable[cubeindex] & 2048)
      vertlist[11] = 
         vertexInterp(isolevel,grid.at(3),grid.at(7) );

   /* Create the triangle */
   for (i=0;triTable[cubeindex][i]!=-1;i+=3) {
      GX_VEC3 v1 = vertlist[triTable[cubeindex][i  ]];
      GX_VEC3 v2 = vertlist[triTable[cubeindex][i+1]];
      GX_VEC3 v3 = vertlist[triTable[cubeindex][i+2]];
      verts.push_back(v1);verts.push_back(v2);verts.push_back(v3);
   }
   //vertlist.clear();
   return verts;
}

// vertex und normal interpolation
GX_VEC3 vertexInterp(double isolevel, GX_VEC3& p1,GX_VEC3& p2)
{
   double mu;
   GX_VEC3 p;

   if (fabs(isolevel-p1.fi) < 0.00001)
      return p1;
   if (fabs(isolevel-p2.fi) < 0.00001)
      return p2;
   if (fabs(p1.fi-p2.fi) < 0.00001)
      return p1;
   mu = (isolevel - p1.fi) / (p2.fi - p1.fi);

   p.x = p1.x + mu * (p2.x - p1.x);
   p.y = p1.y + mu * (p2.y - p1.y);
   p.z = p1.z + mu * (p2.z - p1.z);
   p.nx = p1.nx + mu * (p2.nx - p1.nx);
   p.ny = p1.ny + mu * (p2.ny - p1.ny);
   p.nz = p1.nz + mu * (p2.nz - p1.nz);
   
   return p;
}

// vector normalisieren
void GX_VEC3::normalize(){

    float len = sqrt(x*x+y*y+z*z); 
    x /= len;
    y /= len;
    z /= len;
}

// aus 2 listen mit tangeten in u und v richtung normalen berechnen
VertList genNormals(VertList tU, VertList tV){
    VertList n;
    for(int i=0; i < static_cast<int>(tU.size()); i++)
    {
        GX_VEC3 v = tU.at(i)*tV.at(i);
        v.normalize();
        n.push_back(v);
    }
    return n;
}
         
// "konstruktoren"
GX_VEC3::GX_VEC3( GLdouble _x, GLdouble _y, GLdouble _z )
{                                                                                            
    x = _x;
    y = _y;
    z = _z;
}
GX_VEC3::GX_VEC3( GLfloat _x, GLfloat _y, GLfloat _z ,GLfloat _nx, GLfloat _ny, GLfloat _nz )
{
    x = _x;
    y = _y;
    z = _z;
    nx = _nx;
    ny = _ny;
    nz = _nz; 
}

// distance wert von 2 punkten berechnen
float dist(GX_VEC3& a, GX_VEC3& b) {
	double xd = a.x - b.x;
	double yd = a.y - b.y;
	double zd = a.z - b.z;
	return sqrt( xd*xd + yd*yd + zd*zd );
}

// distance wert von 2 punkten vergleichen
bool compareDist(const GX_VEC3& a, const GX_VEC3& b) {
    return a.distance < b.distance;
}


// distance wert von 2 punkten vergleichen
bool compareDistPointer(const PGX_VEC3& a, const PGX_VEC3& b) {
    return a->distance < b->distance;
}

GX_VEC3 GX_VEC3::operator* (const GX_VEC3& b) const
{
    return GX_VEC3(y*b.z-z*b.y, z*b.x-x*b.z , x*b.y-y*b.x);
}


GX_VEC3 GX_VEC3::operator- (const GX_VEC3& b) const
{
    return GX_VEC3(x-b.x, y-b.y , z-b.z);
}

GX_VEC3 GX_VEC3::operator+ (const GX_VEC3& b) const
{
    return GX_VEC3(x+b.x, y+b.y , z+b.z);
}

GX_VEC3 GX_VEC3::operator* (const float b) const
{
    return GX_VEC3(x*b, y*b , z*b);
}

float GX_VEC3::length()
{
    return sqrt(x*x+y*y+z*z);
}


