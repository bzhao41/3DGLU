//
//  Polyhedron.cpp
//  AG3D2
//
//  Created by Bruce Zhao on 2/13/18.
//  Copyright © 2018 Bruce Zhao. All rights reserved.
//

#include "Polyhedron.hpp"

Polyhedron::Polyhedron() {
    buildIndexedPolyhedron();
    
    //get buffers for attributes and indices
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VIO);
    
    //put the data on the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(potentialColors),NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(potentialColors), potentialColors);
    
    //Put data on the element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) + sizeof(frameIndices),NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
                    sizeof(frameIndices), frameIndices);
    
    //set up stuff for the body of the cube
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); //make this VAO active
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //associate the VBO with the active VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);
    
    assert((program = InitShader("./vshader00_v150.glsl", "./fshader00_v150.glsl"))!=-1);
    glUseProgram(program); //make it the active one
    
    assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
    glEnableVertexAttribArray(vPosition); //enable it
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    assert((vColor = glGetAttribLocation(program, "vColor"))!=-1);
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));
    assert((mmLoc = glGetUniformLocation(program, "model_matrix")) != -1);
    
    //for the frame..
    glGenVertexArrays(1, &frameVAO);
    glBindVertexArray(frameVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VIO);
    assert((program2 = InitShader("./vshader01_v150.glsl", "./fshader01_v150.glsl")) != -1);
    glUseProgram(program2); //make it the active one
    assert((vPosition = glGetAttribLocation(program2, "vPosition")) != -1);
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    assert((vColor = glGetUniformLocation(program2, "vColor")) != -1);
    
}

Polyhedron::~Polyhedron(){
    
}


void Polyhedron::makeIndexedHexagon(GLubyte ind1, GLubyte ind2, GLubyte ind3, GLubyte ind4, GLubyte ind5, GLubyte ind6) {
    indices[index] = ind1;
    colors[index] = potentialColors[2];
    index++;
    indices[index] = ind2;
    colors[index] = colors[index - 1];
    index++;
    indices[index] = ind3;
    colors[index] = colors[index - 1];
    index++;
    indices[index] = ind3;
    colors[index] = colors[index - 1];
    index++;
    indices[index] = ind1;
    colors[index] =colors[index - 1];
    index++;
    indices[index] = ind4;
    colors[index] = colors[index - 1];
    index++;
    indices[index] = ind1;
    colors[index] = colors[index - 1];
    index++;
    indices[index] = ind5;
    colors[index] = colors[index - 1];
    index++;
    indices[index] = ind4;
    colors[index] = colors[index - 1];
    index++;
    indices[index] = ind5;
    colors[index] = colors[index - 1];
    index++;
    indices[index] = ind1;
    colors[index] = colors[index - 1];
    index++;
    indices[index] = ind6;
    colors[index] = colors[index - 1];
    index++;
}


void Polyhedron::makeIndexedQuad(GLubyte ind1, GLubyte ind2, GLubyte ind3, GLubyte ind4) {
    indices[index] = ind1;
    colors[index] = potentialColors[5];
    index++;
    indices[index] = ind2;
    colors[index] = colors[index - 1];
    index++;
    indices[index] = ind3;
    colors[index] = colors[index - 1];
    index++;
    indices[index] = ind3;
    colors[index] = colors[index - 1];
    index++;
    indices[index] = ind4;
    colors[index] = colors[index - 1];
    index++;
    indices[index] = ind1;
    colors[index] = colors[index - 1];
    index++;
}

void Polyhedron::buildIndexedPolyhedron() {
    index = 0;
    makeIndexedHexagon(5, 4, 3, 2, 1, 0);//top hexagon -> farther
    makeIndexedHexagon(6, 7, 8, 9, 10, 11);//bottem hexagon -> closer
    makeIndexedQuad(0, 6, 11, 5);  //side 1
    makeIndexedQuad(1, 7, 6, 0);  //side 2
    makeIndexedQuad(2, 8, 7, 1);  //side 3
    makeIndexedQuad(3, 9, 8, 2);  //side 4
    makeIndexedQuad(4, 10, 9, 3);  //side 5
    makeIndexedQuad(5, 11, 10, 4);  //side 6
}

void Polyhedron::draw(Camera cam, vector<Light> lights){
    glBindVertexArray(VAO);
    glUseProgram(program); //also switch to using this shader program
    glUniformMatrix4fv(mmLoc, 1, GL_TRUE,modelmatrix);
    glDrawElements(GL_TRIANGLES, (4 * 2 * 3) + (6 * 2 * 3), GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
    
    //Draw the cube frame
    glBindVertexArray(frameVAO);
    glUseProgram(program2); //also switch to using this shader program
    GLuint mm2;
    assert((mm2 = glGetUniformLocation(program2, "model_matrix")) != -1);
    glUniformMatrix4fv(mm2, 1, GL_TRUE, modelmatrix);
    glUniform4fv(vColor, 1, vec4(0, 0, 0, 1));
    glLineWidth(5.0);
    glDrawElements(GL_LINES, 36, GL_UNSIGNED_BYTE, BUFFER_OFFSET(sizeof(indices)));
}
