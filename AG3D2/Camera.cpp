#include "Camera.h"
#include "mat.h"
//TODO
Camera::Camera(int width, int height) {
    roll = 0.1;
    pitch = 0.1;
    yaw = 0.1;
    
    GLfloat fov = 65;
    GLfloat nearp = 0.1;
    GLfloat farp = 100.0;
    projection_matrix = Perspective(fov, GLfloat(width) / height, nearp, farp);
    
    position = vec4(0, 0, 0, 1);
    forward = vec4(0, 0, 100, 1);
    originalF = forward;
    up = vec4(0, 1, 0, 1);
    view_matrix = LookAt(position, forward, up);
    moveable = true;
}

void Camera::positionCamera(vec4 ei, vec4 ni, vec4 vi) {
    position = ei;
    forward = ni;
    up = vi;
    view_matrix = LookAt(position, position + forward, up);
}


Camera::Camera(GLfloat width, GLfloat height, GLfloat fov, int near, int far, vec4 pup, vec4 pforward, vec4 eye, bool move) {
    roll = 0.0;
    pitch = 0.0;
    yaw = 0.0;
    
    position = eye;
    up = pup;
    forward = pforward;
    originalF = pforward;
    moveable = move;
    projection_matrix = Perspective(65, width / height, near, far);
    view_matrix = LookAt(position, position + forward , up);
}

Camera::~Camera() {};

void Camera::setDimensions(GLfloat nWidth, GLfloat nHeight) {
    width = nWidth;
    height = nHeight;
}

void Camera::update(int keypress) {
    switch (keypress) {
        case GLUT_KEY_UP:
            position += forward * speed;
            break;
        case GLUT_KEY_RIGHT:
            strafe = normalize(cross(forward, up));
            strafe.w = 0;
            position += strafe * speed;
            break;
        case GLUT_KEY_DOWN:
            position -= forward * speed;
            break;
        case GLUT_KEY_LEFT:
            strafe = normalize(cross(forward, up));
            strafe.w = 0;
            position -= strafe * speed;
            break;
        case 'c':
            yaw -= 1;
            break;
        case 'C':
            yaw += 1;
            break;
        case 'x':
            pitch -= 1;
            break;
        case 'X':
            pitch += 1;
            break;
        case 'z':
            roll -= 1;
            break;
        case 'Z':
            roll += 1;
            break;
        default:
            break;
    }
    mat4 rotation = RotateX(pitch) * RotateY(yaw) * RotateZ(roll);
    forward = normalize(rotation * originalF);
    view_matrix = LookAt(position, forward + position, normalize(rotation * up));
}





