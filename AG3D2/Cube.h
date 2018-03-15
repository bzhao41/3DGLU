#ifndef __CUBE_H__
#define __CUBE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Cube: public Drawable{

public:
	Cube(vec3);
	~Cube();
	void draw(Camera*, vector<Light*>);
    void setMaterial(vec4, vec4, vec4, float);
    int getTexture() { return activeTexture; }
    void setTexture(int i) { activeTexture = i; }

private:
    void init();
    void buildIndexedCube();
    void makeIndexedQuad(GLubyte, GLubyte, GLubyte, GLubyte);

	unsigned int index;
    int activeTexture = 0;
    GLuint Texture[10];
    GLuint texLoc;

    
	vec4 vertices[8] = { vec4(-0.5,-0.5,0.5,1),vec4(-0.5,0.5,0.5,1),vec4(0.5,0.5,0.5,1),vec4(0.5,-0.5,0.5,1),
		vec4(-0.5,-0.5,-0.5,1),vec4(-0.5,0.5,-0.5,1),vec4(0.5,0.5,-0.5,1),vec4(0.5,-0.5,-0.5,1) };

    vec4 vertexLocations[6 * 2 * 3];
    vec3 vertexNormals[6 * 2 * 3];
    vec2 vertexTextureLocations[6 * 2 * 3];
};
#endif
