//
//  texture.h
//  Coursework2
//
//  Created by Hailey on 2021/11/22.
//

#pragma once

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class Texture
{
    public:
    int width, height;
    GLubyte* data;
    GLuint id;
};
