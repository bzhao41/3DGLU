//
//  Material.hpp
//  AG3D2
//
//  Created by Bruce Zhao on 3/2/18.
//  Copyright © 2018 Bruce Zhao. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp

#include <stdio.h>
#include "Angel.h"

class Material {
    public:
        Material();
        Material(vec4, vec4, vec4, GLfloat);
        ~Material();
    vec4 getAmbiance() { return ambient; }
    vec4 getDiffuse() { return diffuse; }
    vec4 getSpeculat() { return specular; }
    GLfloat getShine() { return shininess; }
    protected:
        vec4 ambient;
        vec4 diffuse;
        vec4 specular;
        GLfloat shininess;
};
#endif /* Material_hpp */
