//
//  Particles.cpp
//  AG3D2
//
//  Created by Bruce Zhao on 3/15/18.
//  Copyright © 2018 Bruce Zhao. All rights reserved.
//

#include "Particles.hpp"

void initializeParticles() {
    for (int i = 0; i < numParticles; i++) {
        particles[i].mass = 1.0;
        for (int j = 0; j < 3; j++) {
            particles[i].color[j] = (float) rand()/RAND_MAX;
            particles[i].position[j] = 2.0 * ((float) rand()/RAND_MAX) - 1.0;
            particles[i].velocity[j] =2.0 * ((float) rand()/RAND_MAX) - 1.0;
        }
        particles[i].color.w = 1.0;
        particles[i].position.w = 1.0;
        particles[i].velocity.w = 1.0;
    }
}

float last_time, present_time;
void idle(){
    float dt;
    present_time = glutGet(GLUT_ELAPSED_TIME);
    dt = 0.001*(present_time-last_time);
    for(int i=0; i<numParticles;i++){
        for(int j=0;j<3;j++){
            particles[i].position[j]+= dt*particles[i].velocity[j];
            particles[i].velocity[j]+= dt*forces(i,j)/particles[i].mass;
        }
        collision(i);
    }
    last_time = present_time;
    glutPostRedisplay();
}

float forces(int i, int j){
    if(j==1) //only affect y direction
        return -1;
    else
        return 0;
}

float coef; //how strong it bounces back
void collision(int n){
    for(int i=0;i<3; i++){
        if(particles[n].position[i] >=1.0){
            particles[n].velocity[i] *= -coef;
            particles[n].position[i] = 1.0-coef*(particles[n].position[i]-1.0);
        }
        if(particles[n].position[i]<=-1.0){
            particles[n].velocity[i] *= -coef;
            particles[n].position[i] = -1.0-coef*(particles[n].position[i]+1.0);
        }
    }
}
