//
//  Plane.hpp
//  AG3D2
//
//  Created by Bruce Zhao on 3/3/18.
//  Copyright © 2018 Bruce Zhao. All rights reserved.
//

#ifndef Plane_hpp
#define Plane_hpp

#include <stdio.h>
#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>
#include <math.h>

class Plane:public Drawable {
    public:
    Plane();
    ~Plane();
    void draw(Camera*, vector<Light*>);
    void build();
    void setMaterial(vec4, vec4, vec4, float);
    
    private:
    
    vec4 vPlane[6] {
        vec4(-12,-1,12,1),
        vec4(12,-1,12,1),
        vec4(-12,-1,-12,1),
        vec4(12,-1,12,1),
        vec4(12,-1,-12,1),
        vec4(-12,-1,-12,1)
    };

    vec3 nPlane[6];
    vec2 tPlane[6] {
        vec2(0,0),
        vec2(1,0),
        vec2(1,1),
        vec2(0,0),
        vec2(1,1),
        vec2(0,1)
    };
    
    void assignGouraudVertices();
    
    GLuint Texture;
    GLuint texLoc;
};
#endif /* Plane_hpp */
