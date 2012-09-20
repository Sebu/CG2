#include "global.h"

void renderQuads(int m, int n, VertList& l, VertList& nor){
    for(int y = 0;y < n - 1; y++)
	{
		glBegin(GL_QUADS);
        for(int x = 0;x < m - 1; x++)
		{
            int i = y*m + x;
 
            glNormal3fv(nor.at(i).v);
            glVertex3fv(l.at(i).v);
			
           	glNormal3fv(nor.at(i+1).v);
            glVertex3fv(l.at(i+1).v);
		    
            glNormal3fv(nor.at(i+1+m).v);
			glVertex3fv(l.at(i+1+m).v);
			
            glNormal3fv(nor.at(i+m).v);
            glVertex3fv(l.at(i+m).v);  
        }
        glEnd();
    }

}

void renderField(int m, int n, VertList& l){
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
            
            glNormal3fv( n1.v );
            glVertex3fv(l.at(i).v);
			glVertex3fv(l.at(i+1).v);
			glVertex3fv(l.at(i+1+m).v);
	
            glNormal3fv( n2.v );
            glVertex3fv(l.at(i).v);
        	glVertex3fv(l.at(i+1+m).v);
		    glVertex3fv(l.at(i+m).v); 
             
        }
        glEnd();
    }

}

void renderNormals(VertList& list, VertList& normals)
{
		glBegin(GL_LINES);
				for (int i = 0; i < static_cast<int>(list.size()); i++) {
				glVertex3f(list.at(i).x, 
							list.at(i).y, 
							list.at(i).z);
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

				glVertex3f(p.x, 
							p.y, 
							p.z);
			
		glEnd();
}

void renderList(VertList& list)
{
		glBegin(GL_POINTS);
				for (int i = 0; i < static_cast<int>(list.size()); i++) {
				glVertex3f(list.at(i).x, 
							list.at(i).y, 
							list.at(i).z);
				}
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
	glCullFace(GL_FRONT);
	//glEnable(GL_CULL_FACE);
	
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
