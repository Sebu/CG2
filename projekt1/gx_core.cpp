#include "global.h"

// Punktliste rendern
void render_list(VertPointerList& list)
{
		glBegin(GL_POINTS);
				for (int i = 0; i < static_cast<int>(list.size()); i++) {
				glVertex3f(list.at(i)->x, 
							list.at(i)->y, 
							list.at(i)->z);
				}
		glEnd();
}

// nehe.gamedev.net basic code GLInit
GLvoid init ( GLvoid )   
{
 
	glShadeModel(GL_SMOOTH);							
	//glClearDepth(1.0f);				
	//glEnable(GL_DEPTH_TEST);				
	//glDepthFunc(GL_LEQUAL);				
	glEnable ( GL_COLOR_MATERIAL );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor(.4f, 0.4f, .4f, 0.0f);
	
}




// nehe.gamedev.net basic code resphape func
GLvoid reshape ( int w, int h )  
{
	glViewport     ( 0, 0, w, h );
	glMatrixMode   ( GL_PROJECTION );  
	glLoadIdentity ( );               
	if ( h==0 )  
		gluPerspective ( 90, ( float ) w, 1.0, 5000.0 );
	else
		gluPerspective ( 90, ( float ) w / ( float ) h, 1.0, 5000.0 );
	glMatrixMode   ( GL_MODELVIEW );
	glLoadIdentity ( );  

}
