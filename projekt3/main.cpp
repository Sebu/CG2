#include "global.h" 



GLfloat LightAmbient[]= { 0.0f, 0.0f, 0.0f, 1.0f }; 
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };	

// position und rotationswinkel fürs objekt
float	posx = -.5f,
		posy = -.5f,
		posz = -500.0f,
		rotx = 0.0f,
		roty = 0.0f,
		rotlx = 0.0f,
		rotly = 0.0f,
		poslz = 130.0f;
		
gx_model	*model;
bool		visTrisFineConst = false;
bool		visTrisConst = false;
bool		visTrisFineLinear = false;
bool		visTrisLinear = false;
bool		visTrisFineSquare = false;
bool		visTrisSquare = false;

bool		visMLSConst = false;
bool		visMLSLinear = false;
bool		visMLSSquare = false;

bool		visNormals = false;
bool		visStruktur = false;
bool		visModel = true;

int         n = 10;
int         m = 10;
int			k = 10;

GX_VEC3		apoint(0.0f, 0.0f, 150.0f);

// 
VertList        pointsConst;
VertList        pointsLinear;
VertList        pointsSquare;

VertList        trisFineConst;
VertList        trisConst;
VertList        trisFineLinear;
VertList        trisLinear;
VertList        trisFineSquare;
VertList        trisSquare;
VertList        pic;

// fuer unsere maussteuerung gedrueckte taste und position merken
int			mbutton = 0;
int			sx,sy;

void processMenuEvents(int option) {

    switch (option) {
	case 1:
		visTrisFineConst = !visTrisFineConst;
        break;        
	case 2:
		visTrisConst = !visTrisConst;
		break;
	case 3:
		visMLSConst = !visMLSConst;
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
    case 8:
		visTrisFineLinear = !visTrisFineLinear;
        break;        
	case 9:
		visTrisLinear = !visTrisLinear;
		break;
	case 10:
		visTrisFineSquare = !visTrisFineSquare;
        break;        
	case 11:
		visTrisSquare = !visTrisSquare;
		break;
    case 12:
		visMLSLinear = !visMLSLinear;
        break; 
    case 13:
		visMLSSquare = !visMLSSquare;
        break; 
    case 14:
		model->rayMarch();
        break;
    case 15:
		model->sphereTrace();
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
	 case 'q':  
      roty -= 5.5f; 
      break;
    case 'w':  
      roty += 5.5f; 
      break;
    case 's':  
      save_tris("testa.off", trisFineConst); 
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
        rotly += 1.5f; 
      break;
    case GLUT_KEY_DOWN:  
        rotly -= 1.5f; 
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
    glutAddMenuEntry("feine Flaeche anzeigen (konstant)",1);
    glutAddMenuEntry("Flaeche anzeigen (konstant)",2);
    
    glutAddMenuEntry("feine Flaeche anzeigen (linear)",8);
    glutAddMenuEntry("Flaeche anzeigen (linear)",9);
    
    glutAddMenuEntry("feine Flaeche anzeigen (quadrat.)",10);
    glutAddMenuEntry("Flaeche anzeigen (quadrat.)",11);
    
    glutAddMenuEntry("Funktionswerte anzeigen (konstant)",3);  
    glutAddMenuEntry("Funktionswerte anzeigen (linear)",12);
    glutAddMenuEntry("Funktionswerte anzeigen (quadrat.)",13); 
    
    glutAddMenuEntry("Datenpunkte anzeigen",4);
    //glutAddMenuEntry("Normalen anzeigen",5);
    glutAddMenuEntry("Positionsreset",6);
    glutAddMenuEntry("KD-Tree anzeigen",7);
    
    glutAddMenuEntry("Ray Marching starten", 14);
    glutAddMenuEntry("Sphere Tracing starten", 15);
    
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
  

    // funktionswerte anzeigen
    glColor3f( 1.0f, 1.0f, 1.0f);
    glPointSize(5.0f);
    if(visMLSConst) renderValues(pointsConst);
    if(visMLSLinear) renderValues(pointsLinear);    
    if(visMLSSquare) renderValues(pointsSquare);

    
    glEnable(GL_LIGHTING);
    
    // model anzeigen
    glColor3f( 1.0f, 1.0f, 1.0f);
    if(visTrisFineConst) renderTris(trisFineConst);
    if(visTrisConst) renderTris(trisConst);
    if(visTrisFineLinear) renderTris(trisFineLinear);
    if(visTrisLinear) renderTris(trisLinear);
    if(visTrisFineSquare) renderTris(trisFineSquare);
    if(visTrisSquare) renderTris(trisSquare);
    
    // off Daten anzeigen
    glPointSize(2.0f);		
	if (visModel) model->render();

    glDisable(GL_LIGHTING);


    // ray marching bild anzeigen
    glPointSize(1.0f);
    renderList(pic);
    
    //KD-tree anzeigen
    if(visStruktur)
    {
        glDisable(GL_DEPTH_TEST);
        model->visStructure();
        glEnable(GL_DEPTH_TEST);
    }


    //konstant fit berechnen
    if(trisConst.empty() && (visTrisConst || visTrisFineConst || visMLSConst )){
        pointsConst = genPoints(2*n, 2*m, 2*k, *model, genPointConst );
        trisFineConst = genTrisFine(2*n, 2*m, 2*k, pointsConst );
        trisConst = genTris(2*n, 2*m, 2*k, pointsConst );
    }
        
    //linear fit berechnen
    if(trisLinear.empty() && (visTrisLinear || visTrisFineLinear || visMLSLinear )){
        pointsLinear = genPoints(2*n, 2*m, 2*k, *model, genPointLinear);
        trisFineLinear = genTrisFine(2*n, 2*m, 2*k, pointsLinear );
        trisLinear = genTris(2*n, 2*m, 2*k, pointsLinear );
    }

    //square fit berechnen
    if(trisSquare.empty() && (visTrisSquare || visTrisFineSquare || visMLSSquare )){
        pointsSquare = genPoints(2*n, 2*m, 2*k, *model, genPointSquare);
        trisFineSquare = genTrisFine(2*n, 2*m, 2*k, pointsSquare);
        trisSquare = genTris(2*n, 2*m, 2*k, pointsSquare);
    }

    //ray marching berechnen
    //if(pic.empty())
      //  pic = model->rayMarch(); 

    
    
    
    
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
	if (argc < 6) {
		cout << "nicht genug Argumente" << endl;
		cout << "Aufruf mit: filename N M K namelenausrichtung (1 normal -1 nach innen)" << endl;
    	cout << "Default: cat.off 10 10 10 1" << endl;
		model->load_off("cat.off", 1);
	} else {
		model->load_off(argv[1], atoi(argv[5]));
		n = atoi(argv[2]);
		m = atoi(argv[3]);
		k = atoi(argv[4]);
	}
	
	// bbox etwas vergrößern
    model->box->sizeUp(0.1f);
    
    model->createKdtree(20);
    model->genExtraPoints();


    // konstant

    //pointsConst = genPoints(2*n, 2*m, 2*k, *model, genPointConst);
    //trisFineConst = genTrisFine(2*n, 2*m, 2*k, pointsConst );
    //trisConst = genTris(2*n, 2*m, 2*k, pointsConst );

    

 
    
	//nehe.gamdev.net basic code standard init fuer glut
	glutInit            ( &argc, argv ); 
	glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE );
	glutInitWindowSize  ( 640, 480 ); 

	glutCreateWindow    ( "Blatt3 Approximation und Polygonisierung von Impliziten Flächen" ); 
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
