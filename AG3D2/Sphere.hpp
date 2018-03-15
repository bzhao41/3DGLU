//
//  Sphere.hpp
//  AG3D2
//
//  Created by Bruce Zhao on 2/22/18.
//  Copyright © 2018 Bruce Zhao. All rights reserved.
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include <stdio.h>
#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>
#include <math.h>

class Sphere:public Drawable {
    
public:
    Sphere(vec3);
    Sphere(vec3, float);
    
    void draw(Camera*,vector<Light*>);
    ~Sphere();
    void setMaterial(vec4, vec4, vec4, float);
private:
    //(4 triangular faces per tetrahedron)^(numDivisions-1)*3 vertices per triangle
    static const unsigned int numVertices = 3145728;
    float radius = 1;
    
    int index;
    vec3 origin;
    void build();
    
    //data
    vec4 vertexLocations[numVertices];
    vec3 vertexNormals[numVertices];
    vec2 vertexTextures[numVertices];
    GLuint Texture[10];
    GLuint texLoc;
    
    float sqrt2, sqrt6;
    void tetrahedron(int);
    void divideTriangle(vec4, vec4, vec4,int);
    void triangle(vec4, vec4, vec4);
    vec4 unit(vec4);
    

    void assignGouradVertices();
};
#endif /* Sphere_hpp */
