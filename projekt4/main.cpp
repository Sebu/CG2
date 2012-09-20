#include "global.h" 



GLfloat LightAmbient[]= { 0.0f, 0.0f, 0.0f, 1.0f }; 
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };	

// position und rotationswinkel fürs objekt
float	posx = -.5f,
		posy = -.5f,
		posz = -10.0f,
		rotx = 0.0f,
		roty = 0.0f,
		rotlx = 0.0f,
		rotly = 0.0f,
		poslz = 130.0f;
		
gx_model	*model;
gx_model	*model2;
gx_model	*modelNormal;

bool		visFaces = false;
bool		visEdges = true;

bool		visFacesOpt = false;
bool		visEdgesOpt = true;

bool		visNormals = false;
bool		visStruktur = false;
bool		visModel = true;

int         n = 10;
int         m = 10;
int			k = 10;
int         number= 229;

GX_VEC3		apoint(0.0f, 0.0f, 400.0f);

// 


// fuer unsere maussteuerung gedrueckte taste und position merken
int			mbutton = 0;
int			sx,sy;

void processMenuEvents(int option) {

    switch (option) {
	case 1:
		optimize(*model2, *model);
        break; 
    case 2:
		visEdges = !visEdges;
        break;          
    case 3:
		visFaces = !visFaces;
        break;
	case 4:
        visModel = !visModel;
		break;
	case 5:
        visNormals = !visNormals;
		break;
	case 6:
        posx = -.5f;
		posy = -.5f;
		posz = -2.0f;
		rotx = 0.0f;
		roty = 0.0f;
        break;
	case 7:
        visStruktur = !visStruktur;
		break;
    case 12:
		visEdgesOpt = !visEdgesOpt;
        break;          
    case 13:
		visFacesOpt = !visFacesOpt;
        break; 
    }
}

// mausbewegungen bearbeiten
void mousemotion( int x, int y)
{
    switch (mbutton) {
	case GLUT_LEFT_BUTTON:
		posy += (float)  (y - sy) / -2.0f;
        posx += (float)  (x - sx) / 2.0f;
		break;
	case GLUT_MIDDLE_BUTTON:
		posz += (float)  (y - sy) / 2.0f;
        break;
	case 10:
        roty += (float)  (y - sy) / 2.0f;
        rotx += (float)  (x - sx) / 2.0f;
    	break;
	case 20:
        rotly += (float)  (y - sy) / -2.0f;
        rotlx += (float)  (x - sx) / -2.0f;
    	break;
	default: // linke taste + ctl
		break;
    }
	sx = x; sy = y;
}

// naja der name sagt es schon tasten abfragen
// "normale tasten"
GLvoid keyboard ( unsigned char key, int x, int y ) 
{
  switch ( key ) {
    case 27:        
      exit(0);
      break;       
	case 'a':  
      apoint.z -= 6.2f; 
      break;
    case 'y':  
      apoint.z += 6.2f; 
      break;
	 case 'o':  
      optimize(*model2, *model);
      cout << "qual: " << model2->erep/model2->crep << endl;
      break;
    case 's':  
      swapEdge(model2->edges[number], *model2, *model);
      break;
    case 'c':  
      collapseEdge(model2->edges[number], *model2, *model );
      cout << number << endl;
      break;
    case 'd':  
      splitEdge(model2->edges[number], *model2, *model );
      break;      
    case 'w':  
      roty += 5.5f; 
      break;
      
    default: 
		break;

  }
}

// naja der name sagt es schon tasten abfragen
// diesmal aber die pfeiltasten als sondertasten
void arrow_keys ( int a_keys, int x, int y )
{
    float t;
  switch ( a_keys ) {
    case GLUT_KEY_UP: 
        number++;
      break;
    case GLUT_KEY_DOWN:  
        number--;
      break;
	   case GLUT_KEY_LEFT:  
        rotlx += 1.5f; 
      break;
    case GLUT_KEY_RIGHT:  
        rotlx -= 1.5f; 
      break;
    default:
      break;
  }
}

// www.lighthouse3d.com demo code modifiziert
void createGLUTMenus() {

    int menu;

    // create the menu and
    // tell glut that "processMenuEvents" will 
    // handle the events
    menu = glutCreateMenu(processMenuEvents);

    //add entries to our menu
    glutAddMenuEntry("optimieren",1);
    glutAddMenuEntry("Kanten anzeigen",2);
    glutAddMenuEntry("Flaechen anzeigen",3);   
    glutAddMenuEntry("Kanten anzeigen (opt. Mesh)",12);
    glutAddMenuEntry("Flaechen anzeigen (opt. Mesh)",13); 
     
    glutAddMenuEntry("Datenpunkte anzeigen",4);
    //glutAddMenuEntry("Normalen anzeigen",5);
    glutAddMenuEntry("Positionsreset",6);
    glutAddMenuEntry("KD-Tree anzeigen",7);
    
    // attach the menu to the right button
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}



// wenn ein maustaste gedrueckt wird position und tastenstatus speichern
void mouse(int button, int state, int x, int y)
{
	mbutton = button;
	sx = x; sy = y;
	int mod = glutGetModifiers();
    if (mod == GLUT_ACTIVE_CTRL && mbutton == GLUT_LEFT_BUTTON)
        mbutton = 10;
    if (mod == GLUT_ACTIVE_ALT && mbutton == GLUT_LEFT_BUTTON)
        mbutton = 20;
}

// unsere redering funktion
GLvoid display ( GLvoid )   
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
	
	glLoadIdentity();


    


    

   	// Objekt drehen und bewegen
	glTranslatef(posx,posy,posz);
    glRotatef(rotx, 0.0f, 1.0f, 0.0f);
	glRotatef(roty, 1.0f, 0.0f, 0.0f);
	glRotatef(225.0f, 0.0f, 0.0f, 1.0f);
   

    // licht setzen und anzeigen
    glPushMatrix();
    glRotatef(rotly, 1.0f, 0.0f, 0.0f);
    glRotatef(rotlx, 0.0f, 1.0f, 0.0f);
    float l[3]; 
    l[0] = (float)apoint.x; 
    l[1] = (float)apoint.y; 
    l[2] = (float)apoint.z;
    
    // lichtpunkt anzeigen
    glColor3f( 1.0f, 1.0f, 0.0f);
    glPointSize(6.0f);
    renderPoint(apoint);
    glLightfv(GL_LIGHT0, GL_POSITION, l);
    glPopMatrix();
    
    // bbox der datenpunkte anzeigen
    glColor3f( 0.0f, 1.0f, 0.0f);
    model->box->render();
  

  
    glEnable(GL_LIGHTING);
    
    // model anzeigen
    glColor3f( 1.0f, 1.0f, 1.0f);

    
    // off Daten anzeigen
    glPointSize(2.0f);		
	if (visModel) model->render();


    if(visFacesOpt) renderFaces(model2->faces);
    if(visFaces) renderFaces(modelNormal->faces);   
    
    glDisable(GL_LIGHTING);

    glColor3f( 1.0f, 1.0f, 1.0f);
    
    if(visEdgesOpt) renderEdges(model2->edges, number);
    if(visEdges) renderEdges(modelNormal->edges, number);  
    
    //KD-tree anzeigen
    if(visStruktur)
    {
        glDisable(GL_DEPTH_TEST);
        model->visStructure();
        glEnable(GL_DEPTH_TEST);
    }
    
    
    
	// daten raus "spülen" ;) und back/front buffer swappen
	glFlush();
	glutSwapBuffers ( );
	
	

}

// nehe.gamedev.net basic code MAIN
int main ( int argc, char** argv )  
{

	// ein paar infos fuer die konsole
	cout << "linke Maustaste \t ... x-y Bewegung" << endl;
	cout << "linke Maustaste + Strg \t ... x-y Drehen" << endl;
	cout << "linke Maustaste + Alt \t ... x-y Licht Drehen" << endl;
    cout << "mittlere Maustaste \t ... Zoom" << endl;
	cout << "rechte Maustaste  \t ... Menu" << endl;



	// model laden, kd-tree erzeugen und radius und k naechsten punkt(e) ermitteln
	model = new gx_model();
	model2 = new gx_model();
	modelNormal = new gx_model();
	
	if (argc < 6) {
		cout << "nicht genug Parameter" << endl;
		model->load_off("cat.off", 1);
	} else {

		model->load_off(argv[1], atoi(argv[5]));

	}
	
	
	// bbox etwas vergrößern
    model->box->sizeUp(0.15f);
    model->createKdtree(20);
    model->genExtraPoints();
    model2->crep = atof(argv[2]);
    model2->qual = atof(argv[3]);
    model2->k = atoi(argv[4]);
	model2->load_data("testa.off", atoi(argv[5]), *model);
	modelNormal->load_data("testa.off", atoi(argv[5]), *model);
    cout << model2->crep << " " << model2->qual << " " << model2->k << endl;



   
	//nehe.gamdev.net basic code standard init fuer glut
	glutInit            ( &argc, argv ); 
	glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE );
	glutInitWindowSize  ( 640, 480 ); 

	glutCreateWindow    ( "Blatt4 Dreiecksnetze" ); 
	glutDisplayFunc     ( display );  
	
	glutReshapeFunc     ( reshape );
	glutKeyboardFunc    ( keyboard );
	glutMouseFunc		( mouse );
	glutMotionFunc		( mousemotion );
	glutSpecialFunc		( arrow_keys );
	glutIdleFunc		( display );
	createGLUTMenus();
	init();
	
	// lichteinstellungen
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);	
	glEnable(GL_LIGHT0);	
	glEnable(GL_NORMALIZE);
	glColorMaterial ( GL_FRONT_AND_BACK , GL_AMBIENT_AND_DIFFUSE ) ;	
    glEnable(GL_LIGHTING);
    

    
	glutMainLoop        ( );   

	return 0;
}
