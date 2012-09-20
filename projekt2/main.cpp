#include "global.h" 



GLfloat LightAmbient[]= { 0.0f, 0.0f, 0.0f, 1.0f }; 
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };	

// position und rotationswinkel fürs objekt
float	posx = -.5f,
		posy = -.5f,
		posz = -2.0f,
		rotx = 0.0f,
		roty = 0.0f;

gx_model	*model;
bool		visTensor = false;
bool		visWLS = false;
bool		visMLS = false;
bool		visNormals = false;
bool		visStruktur = false;
bool		visModel = true;

int			km = 20;
int			kn = 20;
int         n = 10;
int         m = 10;
GX_VEC3		apoint(1.0f, 1.0f, 1.0f);

// listen mit radiusPunkten und dem K naechsten punkt
VertPointerList	radiusPoints;
VertPointerList	nearestPoints;
VertList	    test;
VertList        tang;
VertList	    normals;
VertList        tensor;
VertList        tensor2;

// fuer unsere maussteuerung gedrueckte taste und position merken
int			mbutton = 0;
int			sx,sy;

void processMenuEvents(int option) {

    switch (option) {
	case 1:
		visWLS = !visWLS;
        break;        
	case 2:
		visTensor = !visTensor;
		break;
	case 3:
		visMLS = !visMLS;
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

    }
}

// mausbewegungen bearbeiten
void mousemotion( int x, int y)
{
    switch (mbutton) {
	case GLUT_LEFT_BUTTON:
		posy += (float)  (y - sy) / -6.0f;
        posx += (float)  (x - sx) / 6.0f;
		break;
	case GLUT_MIDDLE_BUTTON:
		posz += (float)  (y - sy) / 2.0f;
        break;
	case 10:
        roty += (float)  (y - sy) / 2.0f;
        rotx += (float)  (x - sx) / 2.0f;
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
      apoint.z -= .2f; 
      break;
    case 'y':  
      apoint.z += .2f; 
      break;
	 case 'q':  
      roty -= 5.5f; 
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
  switch ( a_keys ) {
    case GLUT_KEY_UP:  
      apoint.y += .2f; 
      break;
    case GLUT_KEY_DOWN:  
      apoint.y -= .2f; 
      break;
	   case GLUT_KEY_LEFT:  
      apoint.x -= .2f; 
      break;
    case GLUT_KEY_RIGHT:  
      apoint.x += .2f; 
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
    glutAddMenuEntry("Kontrollpunkte anzeigen",1);
    glutAddMenuEntry("Tensorflaeche anzeigen (Bezier)",2);
    glutAddMenuEntry("Tensorflaeche anzeigen (MLS)",3);   
    glutAddMenuEntry("Datenpunkte anzeigen",4);
    glutAddMenuEntry("Normalen anzeigen",5);
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
}

// unsere redering funktion
GLvoid display ( GLvoid )   
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
	
	glLoadIdentity();

	// Objekt drehen und bewegen
	glTranslatef(posx,posy,posz);
    glScalef(1.0f,1.0f,1.0f);
	glRotatef(roty, 1.0f, 0.0f, 0.0f);
	glRotatef(rotx, 0.0f, 1.0f, 0.0f);
	glRotatef(225.0f, 0.0f, 0.0f, 1.0f);
    
    

    glLightfv(GL_LIGHT0, GL_POSITION, apoint.v);
    
    // bbox der datenpunkte anzeigen
    glColor3f( 0.0f, 1.0f, 0.0f);
    model->box->render();
    
    // tensor und stuetzpunkte anzeigen
    glColor3f( 1.0f, 1.0f, 1.0f);
    glEnable(GL_LIGHTING);
    if(visTensor) renderQuads(km, kn, tensor, normals);
    if(visMLS) renderQuads(km, kn, tensor2, tang);
    if(visWLS) renderField(m, n, test);
    glDisable(GL_LIGHTING);

    // normalen anzeigen
    if(visTensor && visNormals) renderNormals(tensor, normals);
    if(visMLS && visNormals) renderNormals(tensor2, tang);

    //KD-tree anzeigen
    if(visStruktur)
    {
        glDisable(GL_DEPTH_TEST);
        model->visStructure();
        glEnable(GL_DEPTH_TEST);
    }
    
    // lichtpunkt anzeigen
    glColor3f( 1.0f, 1.0f, 0.0f);
    glPointSize(6.0f);
    renderPoint(apoint);
    
	// off Daten anzeigen
    glPointSize(2.0f);		
	if (visModel) model->render();


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
    cout << "mittlere Maustaste \t ... Zoom" << endl;
	cout << "rechte Maustaste  \t ... Menu" << endl;



	// model laden, kd-tree erzeugen und radius und k naechsten punkt(e) ermitteln
	model = new gx_model();
	if (argc < 6) {
		cout << "nicht genug Argumente" << endl;
		cout << "Aufruf mit: filename m n km kn" << endl;
    	cout << "Default: franke5.off 10 10 20 20" << endl;
		model->load_off("franke5.off");
	} else {
		model->load_off(argv[1]);
		m = atoi(argv[2]);
		n = atoi(argv[3]);
		km = atoi(argv[4]);
		kn = atoi(argv[5]);
	}
	model->createKdtree(1);

    // stuetzpunkte und tensorflaeche + normalen berechnen
    VertList	    tU;
    VertList	    tV;
    
    // kontrollpunkte aus punktdaten erzeugen
    cout << "GENERIERE STUETZPUNKTE ... ";
    test = genPoints(m, n, *model, NULL);
    cout << "OK" << endl;

    // MLS tensorflaeche und normalen erzeugen
    cout << "GENERIERE MLS TENSORFLAECHE ... ";
    tensor2 = genPoints(km, kn, *model, &tang);
    cout << "OK" << endl;
         
    // bezier tensorflaeche und normalen erzeugen
    cout << "GENERIERE BEZIER TENSORFLAECHE ... ";
    tensor = bezierU(km, kn,m, n, test, &tU);
    bezierV(km, kn,m, n, test, &tV);
    normals = genNormals(tU,tV);
    tU.clear();
    tV.clear();
    cout << "OK" << endl;
    
	//nehe.gamdev.net basic code standard init fuer glut
	glutInit            ( &argc, argv ); 
	glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE );
	glutInitWindowSize  ( 640, 480 ); 

	glutCreateWindow    ( "Blatt2 Approximation von Hoehenfeldern" ); 
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
	//glLightf (GL_LIGHT0, GL_SPOT_CUTOFF, 15.f); 
	glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE ) ;	
    glEnable(GL_LIGHTING);
    
	glutMainLoop        ( );   

	return 0;
}
