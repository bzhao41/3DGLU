#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Angel.h"

class Camera {
//TODO
public:
//    Camera(GLfloat width, GLfloat height, GLfloat fov, int near, int far, vec4 up, vec4 forward, vec4 position, bool moving);
    Camera(GLfloat width, GLfloat height, GLfloat fov, int near, int far, vec4 eye, vec4 at, vec4 up, bool moving);
    Camera(int, int);
    ~Camera();

    mat4 getProjection() { return projection_matrix; }
    mat4 getView() { return view_matrix; }
    
    void update(int);
    void setDimensions(GLfloat, GLfloat);
    void positionCamera(vec4, vec4, vec4);
    
    vec4 getUp() { return up; }
    bool canMove() { return moveable; }
    vec4 getPosition() { return position; }
    vec4 getForward() { return forward; }
    
    void rollC(int);
    void pitchC(int);
    void yawC(int);
    
private:
    GLfloat width;
    GLfloat height;
    GLfloat speed = .3;
    
    GLfloat roll;
    GLfloat pitch;
    GLfloat yaw;
    
    GLfloat dX = 0;
    GLfloat dY = 0;
    GLfloat dZ = 0;
    
    GLint fov;
    
    vec4 eye;
    vec4 at;
    vec4 up;
    
    vec4 position;
    vec4 forward;
    vec4 originalF;
    vec4 strafe;
    bool moveable;
    
    mat4 projection_matrix;
    mat4 view_matrix;
};

#endif
