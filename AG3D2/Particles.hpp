//
//  Particles.hpp
//  AG3D2
//
//  Created by Bruce Zhao on 3/15/18.
//  Copyright © 2018 Bruce Zhao. All rights reserved.
//

#ifndef Particles_hpp
#define Particles_hpp

#include <stdio.h>
#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>
#include <math.h>

struct particle{
    vec4 color;
    vec4 position;
    vec4 velocity;
    float mass;
};

const int numParticles = 1000;
particle particles[numParticles];
vec4 particlePoints[numParticles];
vec4 particleColors[numParticles];

void initializeParticles();
void updateParticles();
void drawParticles();
float applyForces(int, int);
void testCollision(int);
void collision(int n);
float forces(int i, int j);

#endif /* Particles_hpp */
