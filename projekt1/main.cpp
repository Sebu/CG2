#include "global.h" 

// position und rotationswinkel fürs objekt
float	posx = 0.0f,
		posy = 0.0f,
		posz = -10.0f,
		rotx = 0.0f,
		roty = 0.0f;

gx_model	*model;
bool		visRadius = true;
bool		visNearest = true;
bool		visBoxes = true;
bool		visModel = true;

float		radius = 3.0f;
int			k = 100;
GX_VEC3		apoint(1.0f, 1.0f, 1.0f);

// listen mit radiusPunkten und dem K naechsten punkt
VertPointerList	radiusPoints;
VertPointerList	nearestPoints;

// fuer unsere maussteuerung gedrueckte taste und position merken
int			mbutton = 0;
int			sx,sy;

// wenn ein maustaste gedrueckt wird position und tastenstatus speichern
void mouse(int button, int state, int x, int y)
{
	mbutton = button;
	sx = x; sy = y;
}

// mausbewegungen bearbeiten
void mousemotion( int x, int y)
{
	if (mbutton == GLUT_RIGHT_BUTTON)
	{
		roty += (float)  (y - sy) / 2.0f;
		rotx += (float)  (x - sx) / 2.0f;
			
	}
	else if (mbutton == GLUT_MIDDLE_BUTTON)
	{
		posz += (float)  (y - sy) / 2.0f;
	}
	else if (mbutton == GLUT_LEFT_BUTTON)
	{
		posy += (float)  (y - sy) / -6.0f;
		posx += (float)  (x - sx) / 6.0f;
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
      posz -= 2.5f; 
      break;
    case 'y':  
      posz += 2.5f; 
      break;
	 case 'q':  
      roty -= 5.5f; 
      break;
    case 'w':  
      roty += 5.5f; 
      break;
	case '1':
		visRadius = !visRadius;
		break;
	case '2':
		visNearest = !visNearest;
		break;
	case '3':
		visBoxes = !visBoxes;
		break;
	case '4':
		visModel = !visModel;
		break;
	case 'k':
		{
			cout << "Bitte neuen Wert fuer k eingeben (alt " << k << " ):" << endl;
			cin >> k;
			nearestPoints.clear();
			nearestPoints = model->collectKNearest(apoint, k);
			break;
		}
	case 'r':
	{
		cout << "Bitte neuen Radius angeben (alt " << radius <<  " ):" << endl;
		cin >> radius;
		radiusPoints.clear();
		radiusPoints = model->collectInRadius(apoint, radius);
		break;
	}
	case 'p':
	{
		float x,y,z;
		cout << "Bitte x,y,z Werte angeben:" << endl;
		cin >> x >> y >> z;
		apoint.x = x;
		apoint.y = y;
		apoint.z = z;
		
		nearestPoints.clear();
		nearestPoints = model->collectKNearest(apoint, k);
		radiusPoints.clear();
		radiusPoints = model->collectInRadius(apoint, radius);
		break;
	}
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
      posy += 2.5f; 
      break;
    case GLUT_KEY_DOWN:  
      posy -= 2.5f; 
      break;
	   case GLUT_KEY_LEFT:  
      posx -= 2.5f; 
      break;
    case GLUT_KEY_RIGHT:  
      posx += 2.5f; 
      break;
    default:
      break;
  }
}

// unsere redering funktion
GLvoid display ( GLvoid )   
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
	
	glLoadIdentity();

	// Objekt drehen und bewegen
	glTranslatef(posx,posy,posz);
	glRotatef(roty, 1.0f, 0.0f, 0.0f);
	glRotatef(rotx, 0.0f, 1.0f, 0.0f);

	// kd-tree struktur anzeigen
	if(visBoxes) model->visStructure();

	// off Daten anzeigen		
	if (visModel) model->render();

	// den KNearest Punkt anzeigen
	glPointSize(4.0f);
	glColor3f( 0.0f, 1.0f, 1.0f);
	if(visNearest) render_list(nearestPoints);
	
	// die Punkte im Radius anzeigen
	if(visRadius)
	{
		glPointSize(2.0f);
		glColor3f( 1.0f, 0.0f, 0.0f);
		render_list(radiusPoints);

		// matrix retten
		glPushMatrix();
		glColor4f(5.0f,5.0f,5.0f,0.2f);
		glTranslatef(apoint.x, 
				     apoint.y, 
					 apoint.z);

		glEnable(GL_BLEND);		// alpha an
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glutSolidSphere(radius,16,16);

		glDisable(GL_BLEND);	// alpha aus
		glPopMatrix();
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
	cout << "mittlere Maustaste \t ... Zoom" << endl;
	cout << "rechte Maustaste \t ... x-y Drehen" << endl;
	cout << "Taste 1 ... Radius Punkte anzeigen" << endl;
	cout << "Taste 2 ... K Nearest Punkt anzeigen" << endl;
	cout << "Taste 3 ... KD-Baum anzeigen" << endl;
	cout << "Taste 4 ... Model anzeigen" << endl;
  	cout << "Taste r ... Radius aendern" << endl;
	cout << "Taste p ... Punkt aendern" << endl;
	cout << "Taste k ... k aendern" << endl;

	// model laden, kd-tree erzeugen und radius und k naechsten punkt(e) ermitteln
	model = new gx_model();
	if (argc < 3) {
		cout << "nicht genug Argumente" << endl;
		cout << "Aufruf mit filename max_in_node" << endl;
		cout << "nutze default Daten cow.off 20" << endl;
	
		model->load_off("cow.off");
		model->create_kdtree(20);
	} else {
		model->load_off(argv[1]);
		model->create_kdtree(atoi(argv[2]));
	}
	radiusPoints = model->collectInRadius(apoint, radius);
	nearestPoints = model->collectKNearest(apoint, k);


	//nehe.gamdev.net basic code standard init fuer glut
	glutInit            ( &argc, argv ); 
	glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE );
	glutInitWindowSize  ( 640, 480 ); 

	glutCreateWindow    ( "KDTree Test" ); 
	glutDisplayFunc     ( display );  
	
	glutReshapeFunc     ( reshape );
	glutKeyboardFunc    ( keyboard );
	glutMouseFunc		(	mouse );
	glutMotionFunc		( mousemotion );
	glutSpecialFunc		( arrow_keys );
	glutIdleFunc		( display );
	init();
	glutMainLoop        ( );   

	return 0;
}
