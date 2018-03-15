//
//  Polyhedron.hpp
//  AG3D2
//
//  Created by Bruce Zhao on 2/13/18.
//  Copyright © 2018 Bruce Zhao. All rights reserved.
//

#ifndef Polyhedron_hpp
#define Polyhedron_hpp

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>
#include <math.h>

using namespace std;

class Polyhedron: public Drawable{
    
    
public:
    Polyhedron();
    ~Polyhedron();
    void draw(Camera, vector<Light>);
    
private:
    void buildIndexedPolyhedron();
    void makeIndexedQuad(GLubyte, GLubyte, GLubyte, GLubyte);
    void makeIndexedHexagon(GLubyte, GLubyte, GLubyte, GLubyte, GLubyte, GLubyte);
    GLuint vPosition;
    GLuint vColor;
    GLuint mmLoc;
    
    unsigned int index;
    //even lengths
//    vec4 vertices[12] = {
//        vec4(-0.5,0.0,0.5,1),
//        vec4(-0.25,sqrt(3)/4.,0.5,1),
//        vec4(0.25,sqrt(3)/4.,0.5,1),
//        vec4(0.5, 0,0.5,1),
//        vec4(0.25, -sqrt(3)/4.,0.5,1),
//        vec4(-0.25, -sqrt(3)/4.,0.5,1),
//
//        vec4(-0.5,0.0,-0.5,1),
//        vec4(-0.25,sqrt(3)/4.,-0.5,1),
//        vec4(.25,sqrt(3)/4.,-0.5,1),
//        vec4(0.5, 0,-0.5,1),
//        vec4(0.25, -sqrt(3)/4.,-0.5,1),
//        vec4(-0.25, -sqrt(3)/4.,-0.5,1)
//    };
    
    //squeezed but nicer numbers
    vec4 vertices[12] = {
        vec4(-0.5, 0.0, 0.5, 1),
        vec4(-0.25, 0.25, 0.5, 1),
        vec4(0.25, 0.25, 0.5, 1),
        vec4(0.5, 0, 0.5, 1),
        vec4(0.25, -0.25, 0.5, 1),
        vec4(-0.25, -0.25, 0.5, 1),
        
        vec4(-0.5, 0.0, -0.5, 1),
        vec4(-0.25, 0.25, -0.5, 1),
        vec4(0.25, 0.25, -0.5, 1),
        vec4(0.5, 0, -0.5, 1),
        vec4(0.25, -0.25, -0.5, 1),
        vec4(-0.25, -0.25, -0.5, 1)
    };
    //r g b
    vec4 potentialColors[12] = {
        vec4(0,0,0,1), //black
        vec4(1,0,0,1), //red
        vec4(0,1,0,1), //green
        vec4(0,0,1,1), //blue
        vec4(1,1,0,1), //yellow
        vec4(1,0,1,1), //purple
        vec4(0,1,1,1), //cyan
        vec4(0.2f,0.7f,0.1f,1), //something
        vec4(0.5f,0.3f,0.6f,1), //something
        vec4(0.7f,0.4f,0.2f,1), //something
        vec4(0.9f,0.9f,0.3f,1) //something
    };
//    vec4 potentialColors[12] = {
//        vec4(1,0,0,1), //black
//        vec4(1,0,0,1), //red
//        vec4(1,0,0,1), //black
//        vec4(1,0,0,1), //red
//        vec4(1,0,0,1), //black
//        vec4(1,0,0,1), //red
//        vec4(1,0,0,1),
//        vec4(1,0,0,1), //red
//        vec4(1,0,0,1),
//        vec4(1,0,0,1), //red
//        vec4(1,0,0,1), //red
//        vec4(1,0,0,1) //red
//
//    };
    
    vec4 colors[(2 * 4 * 3) + (6 * 2 * 3)];
    //GLubyte colors[(2 * 4 * 3) + (6 * 2 * 3)];
    GLubyte indices[(2 * 4 * 3) + (6 * 2 * 3)];
    
    GLubyte frameIndices[36] = {0, 1,
                                1, 2,
                                2, 3,
                                3, 4,
                                4, 5,
                                5, 0,
                                0, 6,
                                6, 7,
                                7, 1,
                                7, 8,
                                8, 2,
                                8, 9,
                                9, 3,
                                9, 10,
                                10, 4,
                                10, 11,
                                11, 5,
                                11, 6 };

};

#endif /* Polyhedron_hpp */
