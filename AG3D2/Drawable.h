#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__
#include "Angel.h"
#include "Camera.h"
#include "Light.h"
#include <cassert>
#include <vector>

using namespace std;

class Drawable {
protected:
	GLuint program;
    GLuint program2;
    GLuint frameVAO;
	GLuint VAO;
	GLuint VBO;
    GLuint VIO;
    GLuint ColorV;
	mat4 modelmatrix;
    
    GLuint vPosition;
    GLuint nPosition;
    GLuint vTexture;
    
    GLuint mmLoc,vmLoc,pmLoc; //locations of uniforms
    GLuint matDiffuse, matSpecular, matAmbient, matAlpha;
    //Materials
    vec4 diffuse, specular, ambient;
    float shine;

public:
	virtual void draw(Camera*, vector<Light*>)=0;
	void setModelMatrix(mat4 mm) { modelmatrix = mm; }
    mat4 get_model_matrix() { return modelmatrix; }
	static unsigned char* ppmRead(char* filename, int* width, int* height);
	virtual ~Drawable()=0;
};

#endif
