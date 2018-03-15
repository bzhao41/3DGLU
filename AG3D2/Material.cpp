//
//  Material.cpp
//  AG3D2
//
//  Created by Bruce Zhao on 3/2/18.
//  Copyright © 2018 Bruce Zhao. All rights reserved.
//

#include "Material.hpp"

Material::Material() {
    ambient = vec4(0, 0, 0, 1.0);
    diffuse = vec4(0.5, 0.5, 0.5, 1.0);
    specular = vec4(1.0, 1.0, 1.0, 1.0);
    shininess = 100.0;
}

Material::Material(vec4 a, vec4 d, vec4 s, GLfloat shine) {
    ambient = a;
    diffuse = d;
    specular = s;
    shininess = shine;
}

Material::~Material() {
    
}
