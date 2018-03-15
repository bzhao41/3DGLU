#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "Angel.h"

class Light {
	//TODO
    
    public:
    Light(vec4 p, vec4 a, vec4 s, vec4 d) : position(p),
    ambient(a), specular(s), diffuse(d) {}
    
    vec4 getPosition(){return position;}
    vec4 getAmbient(){return ambient;}
    vec4 getDiffuse(){return diffuse;}
    vec4 getSpecular(){return specular;}
    int getTheta(){return theta;}
    int isOn() {return onOff;}
    
    void setPosition(vec4 p) { position = p; }
    void setAmbient(vec4 a) { ambient = a; }
    void setDiffuse(vec4 d) { diffuse = d; }
    void setSpecular(vec4 s) { specular = s; }
    void incrementTheta(int x){
        theta = (theta + x) % 360;
    }
    
    void turnOn() {onOff = 1;}
    void turnOff() {onOff = 0;}
    
    
    protected:
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    int theta = 1;
    int onOff = 0;
    
    
};


#endif
