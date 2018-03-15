//
// A GLSL "Hello World"
// Display a blue square
//

#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Camera.h"  //for camera objects (for use in future assignments)
#include "Light.h"    //for lights (for use in future assignments)
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Cube.h"
#include "SkyBox.hpp"

//Forward declarations
void init(void);
void display(void);
void keyboard(unsigned char, int, int);
void resize(int width, int height);
void close(void);
void arrowKeys(int, int, int);
void rotateSun(int);
//void mouseClick(int, int, int, int);

//Objects
Sphere* mSphere;
Cube* mCube;
Plane* plane;
Camera* cam;
Camera* skyCam;
Light* sun;
Light* flashlight;
SkyBox* sky;

vector<Light*> lights;
vector<Drawable*>drawables;

GLuint windowID=0;
int width = 512;
int height = 512;

bool alternateCamera = false;
float pi = 3.14159265358979323846;
float fov = 65.0;
int near = 1;
int far = 100;


//particles
//struct particle{
//    vec4 color;
//    vec4 position;
//    vec4 velocity;
//    float mass;
//};
//
//const int numParticles = 1000;
//particle particles[numParticles];
//vec4 particlePoints[numParticles];
//vec4 particleColors[numParticles];
//
//void initializeParticles();
//void updateParticles();
//void drawParticles();
//float applyForces(int, int);
//void testCollision(int);
//void collision(int n);
//float forces(int i, int j);

//end

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
    //initialize GLUT
    glutInit(&argc, argv);
#ifdef __APPLE__
    glutInitDisplayMode(GLUT_DEPTH | GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_SINGLE);
#else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
    glutInitWindowSize(width, height);
    
    windowID = glutCreateWindow("3D Sample");
    skyCam = new Camera(width, height);
    skyCam->positionCamera(vec4(0, 0, 0, 1), normalize(vec4(0, 0, 2, 1) - vec4(0, 0, 0, 1)), vec4(0, 1, 0, 0));
    cam = new Camera(width, height, fov, near, far, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, -3, 1 }, true);
    sun = new Light({ 0, 1, 0, 0 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 });
    flashlight = new Light(cam->getPosition(), { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 });
    sun->turnOn();
    flashlight->turnOff();
    lights.push_back(sun);
    lights.push_back(flashlight);
    
    //print out info about our system
    fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
    fprintf(stdout, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    //initialize glew on non-apple systems
#ifndef __APPLE__
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
    
    init(); //initalize objects
    
    //set up the callback functions
    glutDisplayFunc(display);
    glutWMCloseFunc(close);
    glutKeyboardFunc(keyboard);  //What to do if a keyboard event is detected
    glutSpecialFunc(arrowKeys);
    glutReshapeFunc(resize);
    
    
    //start the main event listening loop
    glutMainLoop();
    return 0;
}

// Initialization
void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    sky = new SkyBox();
    
    mSphere = new Sphere(vec3(1, 0, -.5));
    mSphere->setMaterial(vec4(1.0, 0.2, 0.2, 1.0), vec4(1.0, 1.0, 1.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), 50.0f);
    plane = new Plane();
    plane->setMaterial(vec4(0.0, 1.0, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), 50.0f);
    mCube = new Cube(vec3(-.75, 0, 0));
    mCube->setMaterial(vec4(1.0, 0.2, 0.2, 1.0), vec4(1.0, 1.0, 1.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), 50.0f);
    
    drawables.push_back(plane);
    drawables.push_back(mCube);
    drawables.push_back(mSphere);
    
    glutTimerFunc(50, rotateSun, 0);
    
    
}
//rotates the sun by 1 degree
void rotateSun(int x) {
    int t = 1;
    vec4 currentPos = sun->getPosition();
    sun->incrementTheta(t);
    float s = sin(t * pi/180);
    float c = cos(t * pi/180);
    float xp = currentPos.x * c - currentPos.y * s;
    float yp = currentPos.y * c + currentPos.x * s;
    sun->setPosition(vec4(xp, yp, currentPos.z, currentPos.w));
    if (sun->getTheta() > 130 && sun->getTheta() < 240 ) {
        sun->turnOff();
    } else {
        sun->turnOn();
    }
    glutPostRedisplay();
    glutTimerFunc(50, rotateSun, 0);
}

//----------------------------------------------------------------------------
//Display callback
void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    sky->draw(skyCam, lights);
    for (unsigned int i = 0; i < drawables.size(); i++)
        drawables[i]->draw(cam, lights);
    glutSwapBuffers();
}

//----------------------------------------------------------------------------
//Window resize callback
void resize(int w, int h) {
    //glViewport(0, 0, (GLsizei)w, (GLsizei)h);  //make the viewport the entire window
    glutReshapeWindow( width, height);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 't':
            mCube->setTexture(0);
            break;
        case 'T':
            mCube->setTexture(1);
            break;
        case ' ':
            if (flashlight->isOn() == 1) {
                cout << "off" << endl;
                flashlight->turnOff();
            } else {
                flashlight->turnOn();
                flashlight->setPosition(cam->getPosition());
                cout << "on" << endl;
            }
            glutPostRedisplay();
            break;
        case 033:  // Escape key
        case 'q': case 'Q':
            close();
            break;
    }
    if (cam->canMove()) {
        switch (key) {
            case 'C':
            case 'c':
            case 'Z':
            case 'z':
            case 'X':
            case 'x':
                cam->update(key);
                skyCam->update(key);
                flashlight->setPosition(cam->getPosition());
                break;
        }
        glutPostRedisplay();
    }
}

void close(){
    delete(sky);
    for (unsigned int i = 0; i < drawables.size(); i++)
        delete(drawables[i]);
    
    if(windowID>0)
        glutDestroyWindow(windowID);
    
    exit(EXIT_SUCCESS);
}

void arrowKeys(int key, int x, int y) {
    if(cam->canMove()) {
        switch(key) {
            case GLUT_KEY_UP:
            case GLUT_KEY_RIGHT:
            case GLUT_KEY_DOWN:
            case GLUT_KEY_LEFT:
                cam->update(key);
                flashlight->setPosition(cam->getPosition());
                break;
        }
        glutPostRedisplay();
    }
}

//void initializeParticles() {
//    for (int i = 0; i < numParticles; i++) {
//        particles[i].mass = 1.0;
//        for (int j = 0; j < 3; j++) {
//            particles[i].color[j] = (float) rand()/RAND_MAX;
//            particles[i].position[j] = 2.0 * ((float) rand()/RAND_MAX) - 1.0;
//            particles[i].velocity[j] =2.0 * ((float) rand()/RAND_MAX) - 1.0;
//        }
//        particles[i].color.w = 1.0;
//        particles[i].position.w = 1.0;
//        particles[i].velocity.w = 1.0;
//    }
//}
//
//float last_time, present_time;
//void idle(){
//    float dt;
//    present_time = glutGet(GLUT_ELAPSED_TIME);
//    dt = 0.001*(present_time-last_time);
//    for(int i=0; i<numParticles;i++){
//        for(int j=0;j<3;j++){
//            particles[i].position[j]+= dt*particles[i].velocity[j];
//            particles[i].velocity[j]+= dt*forces(i,j)/particles[i].mass;
//        }
//        collision(i);
//    }
//    last_time = present_time;
//    glutPostRedisplay();
//}
//
//float forces(int i, int j){
//    if(j==1) //only affect y direction
//        return -1;
//    else
//        return 0;
//}
//
//float coef; //how strong it bounces back
//void collision(int n){
//    for(int i=0;i<3; i++){
//        if(particles[n].position[i] >=1.0){
//            particles[n].velocity[i] *= -coef;
//            particles[n].position[i] = 1.0-coef*(particles[n].position[i]-1.0);
//        }
//        if(particles[n].position[i]<=-1.0){
//            particles[n].velocity[i] *= -coef;
//            particles[n].position[i] = -1.0-coef*(particles[n].position[i]+1.0);
//        }
//    }
//}

