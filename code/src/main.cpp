#include <cmath>
#include <iostream>

#include "camera.h"
#include "extra.h"
#include "Vector3f.h"

using std::cout;
using std::endl;

namespace {
    const int INIT_WINDOW_X = 60;
    const int INIT_WINDOW_Y = 60;
    const int INIT_WINDOW_W = 600;
    const int INIT_WINDOW_H = 600;

    // Camera
    Camera camera;

    // UI Variables
    bool g_mousePressed = false;
    
    // Header declarations for globals

    void drawSystem() {
	glShadeModel(GL_SMOOTH);
	glutWireSphere(1, 20, 15);
    }

    void drawScene(void) {
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Light color
	GLfloat Lt0diff[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat Lt0pos[] = {3.0, 3.0, 5.0, 1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
	glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

	glLoadMatrixf(camera.viewMatrix());

	// Draw actual system
	drawSystem();

	cout << "Drawing..." << endl;
	
	// Draw axes while rotatin
	if(g_mousePressed ) {
	    cout << "Draw Mouse pressed" << endl;
	    glPushMatrix();
	    Vector3f eye = camera.GetCenter();
	    glTranslatef( eye[0], eye[1], eye[2] );
	    
	    // Save current state of OpenGL
	    glPushAttrib(GL_ALL_ATTRIB_BITS);
	    
	    // This is to draw the axes when the mouse button is down
	    glDisable(GL_LIGHTING);
	    glLineWidth(3);
	    glPushMatrix();
	    glScaled(5.0,5.0,5.0);
	    glBegin(GL_LINES);

	    // x, y, z axs
	    glColor4f(1,0.5,0.5,1); glVertex3f(0,0,0); glVertex3f(1,0,0);
	    glColor4f(0.5,1,0.5,1); glVertex3f(0,0,0); glVertex3f(0,1,0);
	    glColor4f(0.5,0.5,1,1); glVertex3f(0,0,0); glVertex3f(0,0,1);

	    // negative axes
	    glColor4f(0.5,0.5,0.5,1);
	    glVertex3f(0,0,0); glVertex3f(-1,0,0);
	    glVertex3f(0,0,0); glVertex3f(0,-1,0);
	    glVertex3f(0,0,0); glVertex3f(0,0,-1);

	    // End LINES
	    glEnd();
	    glPopMatrix();
	    
	    glPopAttrib();
	    glPopMatrix();
	}	
	
	// Dump image to scren
	glutSwapBuffers();
    }

    void initCamera(void) {
	camera.SetDimensions(INIT_WINDOW_W, INIT_WINDOW_H);
	camera.SetDistance(10);
	camera.SetCenter(Vector3f::ZERO);
    }

    void mouseFunc(int button, int state, int x, int y) {
	cout << "Mouse func" << endl;
	if (state == GLUT_DOWN) {
		g_mousePressed = true;
            
		switch (button) {
		case GLUT_LEFT_BUTTON:
		    camera.MouseClick(Camera::LEFT, x, y);
		    break;
		case GLUT_MIDDLE_BUTTON:
		    camera.MouseClick(Camera::MIDDLE, x, y);
		    break;
		case GLUT_RIGHT_BUTTON:
		    camera.MouseClick(Camera::RIGHT, x,y);
		default:
		    break;
		}                       
	} else {
	    camera.MouseRelease(x,y);
	    g_mousePressed = false;
	}
	glutPostRedisplay();
    }
    
    // Called when mouse is moved while button pressed.
    void motionFunc(int x, int y) {
	cout << "Motion func" << endl;
	camera.MouseDrag(x,y);        
	
	glutPostRedisplay();
    }

    // Reshape function
    void reshapeFunc(int w, int h) {
	camera.SetDimensions(w,h);

	camera.SetViewport(0,0,w,h);
	camera.ApplyViewport();

	// Set up a perspective view, with square aspect ratio
	glMatrixMode(GL_PROJECTION);

	camera.SetPerspective(50);
	glLoadMatrixf( camera.projectionMatrix() );
    }

    // Initialize OpenGL's rendering modes
    void initRendering() {
	glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
	glEnable(GL_LIGHTING);     // Enable lighting calculations
	glEnable(GL_LIGHT0);       // Turn on light #0.

	glEnable(GL_NORMALIZE);

	// Setup polygon drawing
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Clear to black
	glClearColor(0,0,0,1);
    }

    void timerFunc(int t) {
	//	cloth->addT(t);
	//	stepSystem();

	//	glutPostRedisplay();

	glutTimerFunc(t, &timerFunc, t);
    }
}

int main(int argc, char* argv[]) {
    cout << "Starting main..." << endl;
    glutInit(&argc, argv);

    // Double buffer for animation
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );    

    glutInitWindowPosition(INIT_WINDOW_X, INIT_WINDOW_Y);
    glutInitWindowSize(INIT_WINDOW_W, INIT_WINDOW_H);

    initCamera();

    glutCreateWindow("Assignment 4");

    initRendering();

    // Moue resizing
    glutReshapeFunc(reshapeFunc);    

    // Interactivity
    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);

    // Display callback
    glutDisplayFunc(drawScene);

    // Timer function
    glutTimerFunc(20, timerFunc, 20);    
    
    glutMainLoop();

    return 0;
}
