#include "global.h"

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


// berstein formel
GX_VEC3 berstein(int n, float t, VertList& l, GX_VEC3 *tangent){
    GX_VEC3 b[n+1][n+1];

    // mit den stuetzpunkten initialisieren
    for (int i = 0; i < n+1; i++)
        b[i][0] = l.at(i);  
    
    // ausrechnen
    for (int ji = 1; ji <= n; ji++)
        for (int i = 0; i <= (n-ji); i++)
        {
            b[i][ji] = b[i][ji - 1]*(1.0f - t)  + b[i + 1][ji - 1]*t;
        }
    
    // tangente mitnehmen    
    if (tangent != NULL) *tangent = (b[1][n-1]-b[0][n-1]) * (float)n;
    // p zurueck geben
    return b[0][n];
}

// bezier tensor in V richtung
VertList bezierV(int km, int kn,int m, int n, VertList& l, VertList *tangent){
    VertList r;
    VertList t;
    
    float stepx = 1.0f/(float)(km-1);
    float stepy = 1.0f/(float)(kn-1);
    
    float posy = 0.0f;
    for(int yiter=0; yiter<(kn); yiter++){ // y
        float posx = 0.0f;
        for(int xiter=0; xiter<(km); xiter++){   // x
            
            GX_VEC3 tan;
            for (int j=0;j<n;j++)
            {
                VertList t2;
                for(int i=0;i<m;i++)
                    t2.push_back(l.at(j*m+i));   
                r.push_back( berstein(m-1,posx, t2, NULL) );
                t2.clear();
            }
            
            t.push_back( berstein(n-1,posy, r, &tan) );
            tangent->push_back( tan );
            r.clear();
            
            posx += stepx;
        }
        posy += stepy;
    }

    //cout << t.size();
    return t;
    
}

// bezier tensor in U richtung
VertList bezierU(int km, int kn,int m, int n,VertList& l, VertList *tangent){
    VertList r;
    VertList t;
    
    float stepx = 1.0f/(float)(km-1);
    float stepy = 1.0f/(float)(kn-1);
    
    float posy = 0.0f;
    for(int yiter=0; yiter<(kn); yiter++){ // y
        float posx = 0.0f;
        for(int xiter=0; xiter<(km); xiter++){   // x
            
            GX_VEC3 tan;
            for (int j=0;j<m;j++)
            {
                VertList t2;
                for(int i=0;i<n;i++)
                    t2.push_back(l.at(j+i*m));   
                r.push_back( berstein(n-1,posx, t2, NULL) );
                t2.clear();
            }
            
            t.push_back( berstein(m-1,posy, r, &tan) );
            tangent->push_back( tan );
            r.clear();
            
            posx += stepx;
        }
        posy += stepy;
    }

    //cout << t.size();
    return t;
    
}


VertList genPoints(int m, int n, gx_model& o, VertList *nor){
   
    // Schrittweite berechnen 
    float stepx = (o.box->vmax.x - o.box->vmin.x) / (float)(m-1);
    float stepy = (o.box->vmax.y - o.box->vmin.y) / (float)(n-1);
    VertList l;
    
    float posy = o.box->vmin.y;
    for(int yiter=0; yiter<n; yiter++){ // y
        float posx = o.box->vmin.x;
        for(int xiter=0; xiter<m; xiter++){   // x
            
            
            ColumnVector b(6);
            b(1) = 1.0f;      b(2) = posx;      b(3) = posy;
            b(4) = posx*posx; b(5) = posx*posy; b(6) = posy*posy;
            
            Point *xp = new Point(posx,posy,.0f);  // z Wert spielt keine Rolle

            VertPointerList plist;
            plist = o.collectKNearest(*xp, 9);
            //plist = o.collectInRadius(*xp, 0.01f);

            Matrix part1(6,6); part1 = 0.0;
            Matrix part2(6,1); part2 = 0.0;
            
          
            for(int i=0; i < static_cast<int>(plist.size()); i++){
                float x = plist.at(i)->x;
                float y = plist.at(i)->y;
                float fi = plist.at(i)->z;

                ColumnVector bi(6);
                bi(1) = 1.0f;   bi(2) = x;      bi(3) = y;
                bi(4) = x*x;    bi(5) = x*y;    bi(6) = y*y;
                
                float d = dist(*xp,*plist.at(i));
                float wend = (1.0f - d)*(1.0f - d)*(1.0f - d)*(1.0f - d)*(4*d+1);
                
                part1 += wend * bi * bi.t();
                
                part2 += wend * bi * fi;
            }
            Matrix c = (part1.i() * part2);
            //  f(x) = b^t(x) * c(x) 
            Matrix f = b.t() * c;
            
            
            xp->z = f(1,1);
            l.push_back(*xp);
            
            // normale berechnen
            if(nor != NULL){
                Vector tU;
                Vector tV;
                ColumnVector bu(6);
                bu(1) = 0.0f;      bu(2) = 1.0f; bu(3) = 0.0f;
                bu(4) = posx*2.0f; bu(5) = posy; bu(6) = 0.0f;
                tU.x = 1.0f;
                tU.y = 0.0f;
                Matrix u = bu.t() * c;
                tU.z = u(1,1);
                ColumnVector bv(6);
                bv(1) = 0.0f; bv(2) = 0.0f; bv(3) = 1.0f;
                bv(4) = 0.0f; bv(5) = posx; bv(6) = 2.0f*posy;
                tV.x = 0.0f;
                tV.y = 1.0f;
                Matrix v = bu.t() * c;
                tV.z = v(1,1);
                Point normal = tU * tV;
                normal.normalize();
                nor->push_back( normal ); 
            }
            posx += stepx;
        }
        posy += stepy;
    }
    //cout << l.size();
    return l;                 
}

         
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


