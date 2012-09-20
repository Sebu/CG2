#include "global.h"

void renderQuads(int m, int n, VertList& l, VertList& nor){
    for(int y = 0;y < n - 1; y++)
	{
		glBegin(GL_QUADS);
        for(int x = 0;x < m - 1; x++)
		{
            int i = y*m + x;
 
            glNormal3dv(nor.at(i).v);
            glVertex3dv(l.at(i).v);
			
           	glNormal3dv(nor.at(i+1).v);
            glVertex3dv(l.at(i+1).v);
		    
            glNormal3dv(nor.at(i+1+m).v);
			glVertex3dv(l.at(i+1+m).v);
			
            glNormal3dv(nor.at(i+m).v);
            glVertex3dv(l.at(i+m).v);  
        }
        glEnd();
    }

}

void renderField(int m, int n, VertList& l){
    if (l.empty()) return;
    
    for(int y = 0;y < n - 1; y++)
	{
		glBegin(GL_TRIANGLES);
        for(int x = 0;x < m - 1; x++)
		{
            int i = y*m + x;
           
            GX_VEC3 n1 = (l.at(i+1)-l.at(i)) * (l.at(i+1+m)-l.at(i));
            GX_VEC3 n2 = (l.at(i+1+m)-l.at(i)) * (l.at(i+m)-l.at(i));
            n1.normalize();
            n2.normalize();
            
            glColor3f(l.at(i).fi,l.at(i).fi,l.at(i).fi);
            glNormal3dv( n1.v );
            glVertex3dv(l.at(i).v);
			glVertex3dv(l.at(i+1).v);
			glVertex3dv(l.at(i+1+m).v);
	
            glNormal3dv( n2.v );
            glVertex3dv(l.at(i).v);
        	glVertex3dv(l.at(i+1+m).v);
		    glVertex3dv(l.at(i+m).v); 
             
        }
        glEnd();
    }

}

void renderNormals(VertList& list, VertList& normals)
{
		glBegin(GL_LINES);
				for (int i = 0; i < static_cast<int>(list.size()); i++) {
				glVertex3dv(list.at(i).v);
				glVertex3f(list.at(i).x+normals.at(i).x, 
							list.at(i).y+normals.at(i).y, 
							list.at(i).z+normals.at(i).z);
				}
		glEnd();
}

// Punktliste rendern
void renderPlist(VertPointerList& list)
{
		glBegin(GL_POINTS);
				for (int i = 0; i < static_cast<int>(list.size()); i++) {
				glVertex3f(list.at(i)->x, 
							list.at(i)->y, 
							list.at(i)->z);
				}
		glEnd();
}

void renderPoint(GX_VEC3 p)
{
		glBegin(GL_POINTS);
    
                
				glVertex3f(p.x,	p.y, p.z);
			
		glEnd();
}

void renderList(VertList& list)
{
		glBegin(GL_POINTS);
				for (int i = 0; i < static_cast<int>(list.size()); i++) {
                    glColor3f(list.at(i).fi*.001f, list.at(i).fi*.001f,list.at(i).fi*.001f);
				    glNormal3fv(list.at(i).n);
                    glVertex3dv(list.at(i).v);
				}
		glEnd();

}

void renderValues(VertList& list)
{
		glBegin(GL_POINTS);
				for (int i = 0; i < static_cast<int>(list.size()); i++) {
				    float fi = list.at(i).fi;
				    glNormal3fv(list.at(i).n);
                    if (fi>0.0f) glColor3f(0.0f, fi*.07f+0.1f,0.0f);
                    if (fi<=0.0f) glColor3f(fi*.5f, 1.0f,1.0f);
                    glVertex3dv(list.at(i).v);
				}
		glEnd();
}

void renderEdges(std::vector<GX_HEDGE*> list, int n)
{
				for (int i = 0; i < static_cast<int>(list.size()); i++) {
				if (!list[i]->removed){
                        glLineWidth(1.0f);
                        glColor3f( 1.0f, 1.0f, 1.0f);
                        if (list[i]->b) glColor3f( 0.0f, 1.0f, 1.0f);
                        if (i == n)  {
                            glLineWidth(4.0f);
                            
                                GX_HEDGE    *s =  list[i]->opp->next;
                                GX_HEDGE    *n = s;
                                
                                std::list<GX_HEDGE*>::const_iterator iter;
 
                                for (iter=list[i]->sv->es.begin(); iter != list[i]->sv->es.end(); iter++)
                                {
                                    glColor3f( 1.0f, 1.0f, 0.0f);
                                    glBegin(GL_LINES);
                                        glVertex3dv((*iter)->sv->v);
					                    glVertex3dv((*iter)->opp->sv->v);
					                glEnd();
                                }
                                for (iter=list[i]->opp->sv->es.begin(); iter != list[i]->opp->sv->es.end(); iter++)
                                {
                                    glColor3f( 1.0f, 1.0f, 0.0f);
                                    glBegin(GL_LINES);
                                        glVertex3dv((*iter)->sv->v);
					                    glVertex3dv((*iter)->opp->sv->v);
					                glEnd();
                                }
                                /*do 
                                {   
                                    
                                    glColor3f( 1.0f, 1.0f, 0.0f);
                                    glBegin(GL_LINES);
                                        glVertex3dv(n->sv->v);
					                    glVertex3dv(n->opp->sv->v);
					                glEnd();
                                    if (n->opp == NULL) cout << "wutz" << endl;
                                    n = n->opp->next;
                                } while( n->sv == s->sv && n != s );
                                */
                                glColor3f( 1.0f, 0.0f, 0.0f);
                        }

                        glBegin(GL_LINES);
                            glVertex3dv(list[i]->sv->v);
					        glVertex3dv(list[i]->opp->sv->v);
					    glEnd();
                    }        
				}

}

void renderFaces(std::vector<GX_FACE*> list)
{
		glBegin(GL_TRIANGLES);
				for (int i = 0; i < static_cast<int>(list.size()); i++) {
				if (!list[i]->removed){
                        glNormal3fv(list[i]->e->prev->sv->n);	
                        glVertex3dv(list[i]->e->prev->sv->v);
					    glNormal3fv(list[i]->e->sv->n);
                        glVertex3dv(list[i]->e->sv->v);
                        glNormal3fv(list[i]->e->next->sv->n);
					    glVertex3dv(list[i]->e->next->sv->v);
                    }        
				}
		glEnd();
}

void renderTris(VertList& list)
{
		glBegin(GL_TRIANGLES);
				for (int i = 0; i < static_cast<int>(list.size()); i +=3) {
                    //GX_VEC3 n =  (list.at(i+1)-list.at(i)) * (list.at(i+2)-list.at(i));
                    //n.normalize();
                    //glNormal3dv( n.v );
                    glNormal3fv(list.at(i).n);
                    glVertex3dv(list.at(i).v);
                    glNormal3fv(list.at(i+1).n);
					glVertex3dv(list.at(i+1).v);
					glNormal3fv(list.at(i+2).n);
					glVertex3dv(list.at(i+2).v);
                            
				}
		glEnd();
}

void render_ray(Point& a,Vector& v)
{

	float x2 = a.x + v.x*1000.0f;
	float y2 = a.y + v.y*1000.0f;
	float z2 = a.z + v.z*1000.0f;

	glLineWidth(1.0f);
	glBegin(GL_LINES);
		glColor3f( 1.0f, 1.0f, 1.0f);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(x2, y2, z2);
	glEnd();
}

// nehe.gamedev.net basic code GLInit
GLvoid init ( GLvoid )   
{
 
	glShadeModel(GL_SMOOTH);							
	glClearDepth(1.0f);				
	glEnable(GL_DEPTH_TEST);				
	glDepthFunc(GL_LEQUAL);				
	glEnable ( GL_COLOR_MATERIAL );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor(.4f, 0.4f, .4f, 0.0f);
	glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
	
}




// nehe.gamedev.net basic code resphape func
GLvoid reshape ( int w, int h )  
{
	glViewport     ( 0, 0, w, h );
	glMatrixMode   ( GL_PROJECTION );  
	glLoadIdentity ( );               
	if ( h==0 )  
		gluPerspective ( 90, ( float ) w, 0.1f, 5000.0f );
	else
		gluPerspective ( 90, ( float ) w / ( float ) h, 0.1f, 5000.0f );
	glMatrixMode   ( GL_MODELVIEW );
	glLoadIdentity ( );  

}
