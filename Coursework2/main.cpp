#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <math.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <time.h>
#include <sys/timeb.h>

#include "./lib/animation.h"
#include "./lib/material.h"
#include "./lib/obj_loader.h"
#include "./lib/stb_image.h"
#include "./lib/solid.h"
#include "./lib/build.h"
#include "./lib/texture.h"

#include "data/window.h"
//#include "data/table.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

GLUquadricObj *objCylinder = gluNewQuadric();

using namespace std;

// Materials
mat::Material ambient(0.7, 0.7, 0.7, 0.8, 0.8, 0.8, 1.0, 1.0, 1.0, 100);
mat::Material chrome(0.25, 0.25, 0.25, 0.4, 0.4, 0.4, 0.774597, 0.774597, 0.774597, 76.8);
mat::Material silver(0.19225, 0.19225, 0.19225, 0.50754, 0.50754, 0.50754, 0.508273, 0.508273, 0.508273, 51.2);

// Light Props
static GLfloat light0_ambient[]        = { 0.7, 0.7, 0.7, 1.0 };  //各种光线照射到该材质上，经过很多次反射后最终遗留在环境中的光线强度
static GLfloat light0_diffuse[]        = { 0.6, 0.6, 0.6, 1.0 };
//光线照射到该材质上，经过漫反射后形成的光线强度
static GLfloat light0_specular[]    = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light0_position[]    = { 0.0, 0.0, 0.0, 1.0 };

static bool light1_enabled          = true;
static GLfloat light1_offset[]        = { 8.5, 5, -8.5 };
static GLfloat light1_ambient[]        = { 0.2, 0.2, 0.2, 1.0 };
static GLfloat light1_diffuse[]        = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light1_specular[]    = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light1_position[]    = { 0.0, 3.0, 0.0, 1.0 };
static GLfloat light1_direction[]    = { 0.0, -1.0, 0.0 };
static GLfloat light1_angle            = 20.0;
static GLfloat light1_exponent        = 2.0;

static bool light2_enabled          = true;
static GLfloat light2_offset[]        = { 0, 7, 0 };
static GLfloat light2_ambient[]        = { 0.2, 0.2, 0.2, 1.0 };
static GLfloat light2_diffuse[]        = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light2_specular[]    = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light2_position[]    = { 0.0, 3.0, 0.0, 1.0 };
static GLfloat light2_direction[]    = { 0.0, -1.0, 0.0 };
static GLfloat light2_angle            = 20.0;
static GLfloat light2_exponent        = 2.0;

static bool sun_enabled          = true;
static GLfloat sun_offset[]        = { -50, 100, -50 };
static GLfloat sun_ambient[]        = { 0.2, 0.2, 0.2, 1.0 };
static GLfloat sun_diffuse[]        = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat sun_specular[]    = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat sun_position[]    = { 0.0, 3.0, 0.0, 1.0 };
static GLfloat sun_direction[]    = { 0.0, -1.0, 0.0 };
static GLfloat sun_angle            = 20.0;
static GLfloat sun_exponent        = 2.0;

// Clock
const float clockR    = 1.0f,
            clockVol  = 1.2f,
            angle1min = M_PI / 30.0f,
            minStart  = 0.9f,
            minEnd    = 1.0f,
            stepStart = 0.8f,
            stepEnd   = 1.0f;
float angleHour = 0,
      angleMin  = 0,
      angleSec  = 0;

// Texture
Texture screenTexture;
Texture vanGoghTexture;
Texture GoghTexture;
Texture sunflowerTexture;
Texture flowerTexture;
Texture starTexture;
Texture woodTexture;
Texture wood1Texture;
Texture sheetTexture;
Texture couchTexture0;
Texture couchTexture1;
Texture couchTexture2;
Texture doorTexture;
Texture bookTexture1;
Texture soundboxTexture;
Texture planetTexture;
Texture carpetTexture;

Texture skyboxTexture1;
Texture skyboxTexture2;
Texture skyboxTexture3;
Texture skyboxTexture4;
Texture skyboxTexture5;
Texture skyboxTexture6;

Texture cityTexture1;
Texture cityTexture2;
Texture cityTexture3;
Texture cityTexture4;
Texture cityTexture5;
Texture cityTexture6;
Texture groundTexture;

Texture basketballTexture;

// Loader
objl::Loader fanLoader;
objl::MeshInfo fanMesh;
objl::MeshInfo fanPropellerMesh;

objl::Loader couchLoader;
objl::MeshInfo couchMesh0;
objl::MeshInfo couchMesh1;
objl::MeshInfo couchMesh2;
objl::MeshInfo couchMesh3;
objl::MeshInfo couchMesh4;
objl::MeshInfo couchMesh5;
objl::MeshInfo couchMesh6;
objl::MeshInfo couchMesh7;
objl::MeshInfo couchMesh8;
objl::MeshInfo couchMesh9;
objl::MeshInfo couchMesh10;
objl::MeshInfo couchMesh11;
objl::MeshInfo couchMesh12;
objl::MeshInfo couchMesh13;

objl::Loader tvLoader;
objl::MeshInfo tvMesh0;
objl::MeshInfo tvMesh1;

objl::Loader curtainLoader;
objl::MeshInfo curtainMesh0;
objl::MeshInfo curtainMesh1;
objl::MeshInfo curtainMesh2;
objl::MeshInfo curtainMesh3;
objl::MeshInfo curtainMesh4;
objl::MeshInfo curtainMesh5;
objl::MeshInfo curtainMesh6;

objl::Loader plantLoader;
objl::MeshInfo plantMesh0;
objl::MeshInfo plantMesh1;

objl::Loader telescopeLoader;
objl::MeshInfo telescopeMesh;

objl::Loader paperLoader;
objl::MeshInfo paperMesh;

// Animations
AnimationState doorAnimation = IDLE;
GLfloat doorAngle = 0.f;

AnimationState windowAnimation = IDLE;
GLfloat windowTranslation = 1.f;

AnimationState fanAnimation = FORWARDS;
const GLfloat FAN_LOW_SPEED = 8.0f;
const GLfloat FAN_MAX_SPEED = 16.0f;
GLfloat fanRotation = 1.f;
GLfloat fanRotationSpeed = FAN_LOW_SPEED;

AnimationState ballAnimation = FORWARDS;
const GLfloat BALL_LOW_SPEED = 6.0f;
const GLfloat BALL_MAX_SPEED = 10.0f;
GLfloat ballRotation = 1.f;
GLfloat ballRotationSpeed = BALL_LOW_SPEED;

GLfloat ballRadius = 0.5;
GLfloat relativeHeight = 0;
GLboolean ballStart = false;
GLboolean ballBouncing = false;
GLfloat ballStartSpeed = 1.0;
GLfloat ballCurrentSpeed = 0;
GLfloat gravity = 0.08;
GLfloat resistence = 0.1;

GLboolean flag = false;

// angle of rotation for the camera direction
float angle = 0.0, yAngle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = -5.0f, z = 18.0f;
float roll = 0.0f;

// for mouse movements
float halfWidth = (float)(WINDOW_WIDTH/2.0);
float halfHeight = (float)(WINDOW_HEIGHT/2.0);
float mouseX = 0.0f, mouseY = 0.0f;

void DrawBasketball() {
    glPushMatrix();
        GLUquadricObj *quadObj1;
        quadObj1 = gluNewQuadric();
        gluQuadricTexture(quadObj1,GL_TRUE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,basketballTexture.id);
        gluSphere(quadObj1,ballRadius,100,100);
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, NULL);
    // Stop performing texturing
    glDisable(GL_TEXTURE_2D);
}


void drawCube(){
    glBegin(GL_QUADS);
    // Front
    glNormal3f(0.0f, 0.0f, 1.0f);
//    glColor3f(0, 0, 1);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    // Back
    glNormal3f(0.0f, 0.0f, -1.0f);
//    glColor3f(0, 1, 1);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    // Left
    glNormal3f(-1.0f, 0.0f, 0.0f);
//    glColor3f(0, 0, 1);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    // Right
    glNormal3f(1.0f, 0.0f, 0.0f);
//    glColor3f(0, 1, 1);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    // Top
    glNormal3f(0.0f, 1.0f, 0.0f);
//    glColor3f(0, 0, 1);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    // Bottom
    glNormal3f(0.0f, -1.0f, 0.0f);
//    glColor3f(0, 1, 1);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glEnd();
}

// Draw circle
void DrawCircle(float radius)
{
    double n=100;//segment number
    int i;
    
    glPushMatrix();
//    glColor3f(1,1,1);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0,0.0);
    for(i=0; i<=n; i++){
        glVertex2f(radius*cos(2*3.1415926f/n*i), radius*sin(2*3.1415926f/n*i));
    }
    glEnd();
    glPopMatrix();
}

void DrawBall() {
    glPushMatrix();
        GLUquadricObj *quadObj1;
        quadObj1 = gluNewQuadric();
        // enable texturing
        gluQuadricTexture(quadObj1,GL_TRUE);
        // bind texture
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,planetTexture.id);
        // draw the ball
        gluSphere(quadObj1,1,100,100);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void DrawFramework(float base_a, float base_height, float stand_height) {
    // draw the base
    glPushMatrix();
        glPushMatrix();
            glScalef(base_a, base_height, base_a);
            glutSolidCube(1);
        glPopMatrix();
        
        // draw the stand
        glPushMatrix();
        glRotated(-90, 1, 0, 0);
            GLUquadric *pObj = gluNewQuadric();
            gluCylinder(pObj, 0.1, 0.1, stand_height, 100, 100);
        glPopMatrix();
    glPopMatrix();
}

void drawSkyBox(){
    // Enable Texturing
    glEnable(GL_TEXTURE_2D);

    // Enable setting the colour of the material the cube is made from
    // as well as the material for blending.
    glEnable(GL_COLOR_MATERIAL);

    // Tell openGL which texture buffer to use

    glBindTexture(GL_TEXTURE_2D, skyboxTexture1.id);
    glBegin(GL_QUADS);

    // draw the front sky

    // set the colour of the front sky
    glColor3f(1.0f, 1.0f, 1.0f);

    // set the normal of the front sky
    glNormal3f(0.0f, 0.0f, -1.0f);

    // define texture coordinates for the 4 vertices
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(50.0f, 50.0f, 50.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(50.0f, -50.0f, 50.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-50.0f, -50.0f, 50.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-50.0f, 50.0f, 50.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxTexture2.id);
    glBegin(GL_QUADS);
    // draw the back sky

    // set the normal of the back sky
    glNormal3f(0.0f, 0.0f, 1.0f);

    // define texture coordinates for the 4 vertices

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(50.0f, -50.0f, -50.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(50.0f, 50.0f, -50.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-50.0f, 50.0f, -50.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-50.0f, -50.0f, -50.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxTexture3.id);
    glBegin(GL_QUADS);
    // draw the left sky

    // set the normal of the left sky
    glNormal3f(1.0f, 0.0f, 0.0f);

    // define texture coordinates for the 4 vertices
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-50.0f, 50.0f, 50.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-50.0f, -50.0f, 50.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-50.0f, -50.0f, -50.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-50.0f, 50.0f, -50.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxTexture4.id);
    glBegin(GL_QUADS);
    // draw the right sky

    // set the normal of the right sky
    glNormal3f(-1.0f, 0.0f, 0.0f);

    // define texture coordinates for the 4 vertices
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(50.0f, 50.0f, -50.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(50.0f, -50.0f, -50.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(50.0f, -50.0f, 50.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(50.0f, 50.0f, 50.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxTexture5.id);
    glBegin(GL_QUADS);
    // draw the top sky

    // set the normal of the top sky
    glNormal3f(0.0f, -1.0f, 0.0f);

    // define texture coordinates for the 4 vertices
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-50.0f, 50.0f, 50.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-50.0f, 50.0f, -50.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(50.0f, 50.0f, -50.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(50.0f, 50.0f, 50.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxTexture6.id);
    glBegin(GL_QUADS);
    // draw the bottom sky

    // set the normal of the bottom sky
    glNormal3f(0.0f, 1.0f, 0.0f);

    // define texture coordinates for the 4 vertices
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-50.0f, -50.0f, -50.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-50.0f, -50.0f, 50.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(50.0f, -50.0f, 50.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(50.0f, -50.0f, -50.0f);

    glEnd();

    // Stop blending textures and colours
    glDisable(GL_COLOR_MATERIAL);

    // Bind to the blank buffer to stop ourselves accidentaly
    // using the wrong texture in the next draw call
    glBindTexture(GL_TEXTURE_2D, NULL);
    // Stop performing texturing
    glDisable(GL_TEXTURE_2D);
}


void drawCityBox(){
    // Enable Texturing
    glEnable(GL_TEXTURE_2D);

    // Enable setting the colour of the material the cube is made from
    // as well as the material for blending.
    glEnable(GL_COLOR_MATERIAL);

    // Tell openGL which texture buffer to use

    glBindTexture(GL_TEXTURE_2D, cityTexture1.id);
    glBegin(GL_QUADS);

    // draw the front sky

    // set the colour of the front sky
    glColor3f(1.0f, 1.0f, 1.0f);

    // set the normal of the front sky
    glNormal3f(0.0f, 0.0f, -1.0f);

    // define texture coordinates for the 4 vertices
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(50.0f, 50.0f, 50.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(50.0f, -50.0f, 50.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-50.0f, -50.0f, 50.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-50.0f, 50.0f, 50.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, cityTexture2.id);
    glBegin(GL_QUADS);
    // draw the back sky

    // set the normal of the back sky
    glNormal3f(0.0f, 0.0f, 1.0f);

    // define texture coordinates for the 4 vertices

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(50.0f, -50.0f, -50.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(50.0f, 50.0f, -50.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-50.0f, 50.0f, -50.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-50.0f, -50.0f, -50.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, cityTexture3.id);
    glBegin(GL_QUADS);
    // draw the left sky

    // set the normal of the left sky
    glNormal3f(1.0f, 0.0f, 0.0f);

    // define texture coordinates for the 4 vertices
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-50.0f, 50.0f, 50.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-50.0f, -50.0f, 50.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-50.0f, -50.0f, -50.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-50.0f, 50.0f, -50.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, cityTexture4.id);
    glBegin(GL_QUADS);
    // draw the right sky

    // set the normal of the right sky
    glNormal3f(-1.0f, 0.0f, 0.0f);

    // define texture coordinates for the 4 vertices
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(50.0f, 50.0f, -50.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(50.0f, -50.0f, -50.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(50.0f, -50.0f, 50.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(50.0f, 50.0f, 50.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, cityTexture5.id);
    glBegin(GL_QUADS);
    // draw the top sky

    // set the normal of the top sky
    glNormal3f(0.0f, -1.0f, 0.0f);

    // define texture coordinates for the 4 vertices
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-50.0f, 50.0f, 50.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-50.0f, 50.0f, -50.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(50.0f, 50.0f, -50.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(50.0f, 50.0f, 50.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, cityTexture6.id);
    glBegin(GL_QUADS);
    // draw the bottom sky

    // set the normal of the bottom sky
    glNormal3f(0.0f, 1.0f, 0.0f);

    // define texture coordinates for the 4 vertices
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-50.0f, -50.0f, -50.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-50.0f, -50.0f, 50.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(50.0f, -50.0f, 50.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(50.0f, -50.0f, -50.0f);

    glEnd();

    // Stop blending textures and colours
    glDisable(GL_COLOR_MATERIAL);

    // Bind to the blank buffer to stop ourselves accidentaly
    // using the wrong texture in the next draw call
    glBindTexture(GL_TEXTURE_2D, NULL);
    // Stop performing texturing
    glDisable(GL_TEXTURE_2D);
}


void drawGround(){
    glPushMatrix();
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,groundTexture.id);
        
        // bind the texture to the door
        glBegin(GL_QUADS);
            // draw the left board
            glNormal3f(0, 1, 0);
            // define texture coordinates for the 4 vertices
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-20, 0, 20);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(20, 0, 20);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(20, 0, -20);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-20, 0, -20);
        glEnd();
    
        // Stop blending textures and colours
        glDisable(GL_COLOR_MATERIAL);
        // Bind to the blank buffer to stop ourselves accidentaly
        // using the wrong texture in the next draw call
        glBindTexture(GL_TEXTURE_2D, NULL);
        
        // Stop performing texturing
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

// clock line
void newLine(float rStart, float rEnd, float angle)
{
    float c = cos(angle), s = sin(angle);
    glVertex2f( clockR*rStart*c, clockR*rStart*s);
    glVertex2f( clockR*rEnd*c, clockR*rEnd*s);
}

void SetupRC(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void TimerFunction(int value)
{
    struct timeb tb;
    time_t tim=time(0);
    struct tm* t;
    t=localtime(&tim);
    ftime(&tb);

    angleSec = (float)(t->tm_sec+ (float)tb.millitm/1000.0f)/30.0f * M_PI;
    angleMin = (float)(t->tm_min)/30.0f * M_PI + angleSec/60.0f;
    angleHour = (float)(t->tm_hour > 12 ? t->tm_hour-12 : t->tm_hour)/6.0f * M_PI + angleMin/12.0f;

    glutPostRedisplay();
    glutTimerFunc(33,TimerFunction, 1);
}

void loadTexture(const char* fileName, Texture* texture)
{
    // Generate Texture
    glBindTexture(GL_TEXTURE_2D, texture->id);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int nrChannels;

    texture->data = stbi_load(fileName, &texture->width, &texture->height, &nrChannels, 0);

    if (!texture->data) {
        cout << "Failed to load texture: " << fileName << endl;
        exit(1);
    }

    // stbi_image_free(data);
}

void setupTexture(Texture* texture)
{
    glPixelStoref(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width,
                    texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                    texture->data);
}

void initTextures()
{
    // Loading Textures
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/gogh.png", &vanGoghTexture);
    setupTexture(&vanGoghTexture);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/starry-night.png", &starTexture);
    setupTexture(&starTexture);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/vangogh.png", &GoghTexture);
    setupTexture(&GoghTexture);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/sunflower.png", &sunflowerTexture);
    setupTexture(&sunflowerTexture);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/flower.png", &flowerTexture);
    setupTexture(&flowerTexture);
    
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/sheet.png", &sheetTexture);
    setupTexture(&sheetTexture);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/wood.png", &woodTexture);
    setupTexture(&woodTexture);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/wood2.png", &wood1Texture);
    setupTexture(&wood1Texture);
    
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/couch/leather.png", &couchTexture0);
    setupTexture(&couchTexture0);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/couch/leather2.png", &couchTexture1);
    setupTexture(&couchTexture1);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/couch/leather3.png", &couchTexture2);
    setupTexture(&couchTexture2);
    
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/book.png", &bookTexture1);
    setupTexture(&bookTexture1);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/door.png", &doorTexture);
    setupTexture(&doorTexture);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/friends.png", &screenTexture);
    setupTexture(&screenTexture);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/sound.png", &soundboxTexture);
    setupTexture(&soundboxTexture);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/ball.png", &planetTexture);
    setupTexture(&planetTexture);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/floor.png", &carpetTexture);
    setupTexture(&carpetTexture);
    
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/skybox/back.png", &skyboxTexture1);
    setupTexture(&skyboxTexture1);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/skybox/front.png", &skyboxTexture2);
    setupTexture(&skyboxTexture2);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/skybox/left.png", &skyboxTexture3);
    setupTexture(&skyboxTexture3);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/skybox/right.png", &skyboxTexture4);
    setupTexture(&skyboxTexture4);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/skybox/top.png", &skyboxTexture5);
    setupTexture(&skyboxTexture5);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/skybox/bottom.png", &skyboxTexture6);
    setupTexture(&skyboxTexture6);
    
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/city/back.png", &cityTexture1);
    setupTexture(&cityTexture1);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/city/front.png", &cityTexture2);
    setupTexture(&cityTexture2);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/city/left.png", &cityTexture3);
    setupTexture(&cityTexture3);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/city/right.png", &cityTexture4);
    setupTexture(&cityTexture4);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/city/top.png", &cityTexture5);
    setupTexture(&cityTexture5);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/city/bottom.png", &cityTexture6);
    setupTexture(&cityTexture6);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/ground.png", &groundTexture);
    setupTexture(&groundTexture);
    loadTexture("/Users/hailey/Desktop/Coursework2/Coursework2/texture/basketball.png", &basketballTexture);
    setupTexture(&basketballTexture);
}


void init(void)
{
//    glClearColor (0.31, 0.61, 0.85, 1.0);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

//     Anti aliasing smoothing props
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);

    // Lighting and Shade setup
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    // Turn on OpenGL lighting
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, light1_angle);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, light1_exponent);
    glEnable(GL_LIGHT1);
    
    glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, light2_angle);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_direction);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, light2_exponent);
    glEnable(GL_LIGHT2);
    
    glLightfv(GL_LIGHT3, GL_AMBIENT, sun_ambient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, sun_diffuse);
    glLightfv(GL_LIGHT3, GL_SPECULAR, sun_specular);
    glLightfv(GL_LIGHT3, GL_POSITION, sun_position);
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, sun_angle);
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, sun_direction);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, sun_exponent);
    glEnable(GL_LIGHT3);
    
    // Enable color material mode:
    // The ambient and diffuse color of the front faces will track the color set by glColor().
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    
    initTextures();
//    ambient.active();
    
    fanMesh = fanLoader.LoadedMeshes[0].setup();
    fanPropellerMesh = fanLoader.LoadedMeshes[1].setup();
    
    //这里变true
    couchMesh0 = couchLoader.LoadedMeshes[0].setup();
    couchMesh1 = couchLoader.LoadedMeshes[1].setup();
    couchMesh2 = couchLoader.LoadedMeshes[2].setup();
    couchMesh3 = couchLoader.LoadedMeshes[3].setup();
    couchMesh4 = couchLoader.LoadedMeshes[4].setup();
    couchMesh5 = couchLoader.LoadedMeshes[5].setup();
    couchMesh6 = couchLoader.LoadedMeshes[6].setup();
    couchMesh7 = couchLoader.LoadedMeshes[7].setup();
    couchMesh8 = couchLoader.LoadedMeshes[8].setup();
    couchMesh9 = couchLoader.LoadedMeshes[9].setup();
    couchMesh10 = couchLoader.LoadedMeshes[10].setup();
    couchMesh11 = couchLoader.LoadedMeshes[11].setup();
    couchMesh12 = couchLoader.LoadedMeshes[12].setup();
    couchMesh13 = couchLoader.LoadedMeshes[13].setup();

    tvMesh0 = tvLoader.LoadedMeshes[0].setup();
    tvMesh1 = tvLoader.LoadedMeshes[1].setup();
    
    curtainMesh0 = curtainLoader.LoadedMeshes[0].setup();
    curtainMesh1 = curtainLoader.LoadedMeshes[1].setup();
    curtainMesh2 = curtainLoader.LoadedMeshes[2].setup();
    curtainMesh3 = curtainLoader.LoadedMeshes[3].setup();
    curtainMesh4 = curtainLoader.LoadedMeshes[4].setup();
    curtainMesh5 = curtainLoader.LoadedMeshes[5].setup();
    curtainMesh6 = curtainLoader.LoadedMeshes[6].setup();
    
    plantMesh0 = plantLoader.LoadedMeshes[0].setup();
    plantMesh1 = plantLoader.LoadedMeshes[1].setup();
    
    telescopeMesh = telescopeLoader.LoadedMeshes[0].setup();
    
    paperMesh = paperLoader.LoadedMeshes[0].setup();
    
}


void setupLightning()
{
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
    glPushMatrix();
        glTranslatef(-10, 10, -10);
        GLfloat light0_position[]    = {0, 0, 0, 1};
        glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glPopMatrix();

    if (light1_enabled) {
        glEnable(GL_LIGHT1);
        glPushMatrix();
            glTranslatef(light1_offset[0], light1_offset[1], light1_offset[2]);
            glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
            glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, light1_angle);
            glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);
            glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, light1_exponent);
        glPopMatrix();
    }
    if (light2_enabled) {
        glEnable(GL_LIGHT2);
        glPushMatrix();
            glTranslatef(light2_offset[0], light2_offset[1], light2_offset[2]);
            glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
            glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, light2_angle);
            glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_direction);
            glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, light2_exponent);
        glPopMatrix();
    }
    if (sun_enabled) {
        glEnable(GL_LIGHT3);
        glPushMatrix();
            glTranslatef(sun_offset[0], sun_offset[1], sun_offset[2]);
            glLightfv(GL_LIGHT3, GL_POSITION, sun_position);
            glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, sun_angle);
            glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, sun_direction);
            glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, sun_exponent);
        glPopMatrix();
    }
}

void checkDoorAngle()
{
    if (doorAnimation == FORWARDS || doorAnimation == BACKWARDS)
    {
        doorAngle += doorAnimation == FORWARDS ? 1 : -1;
        if (doorAngle == 90.0f || doorAngle == 0.0f) doorAnimation = IDLE;
    }
}

void checkWindowTranslation()
{
    if (windowAnimation == FORWARDS || windowAnimation == BACKWARDS)
    {
        windowTranslation += windowAnimation == FORWARDS ? 0.02 : -0.02;
        if (windowTranslation >= 1.0f || windowTranslation <= 0.0f) windowAnimation = IDLE;
    }
}

void checkFanRotation()
{
    if (fanAnimation == FORWARDS || fanAnimation ==  BACKWARDS)
    {
        fanRotation += fanAnimation == FORWARDS ? fanRotationSpeed : -fanRotationSpeed;
        if (fanRotationSpeed > 360) fanRotationSpeed = 0;
        else if (fanRotationSpeed < 0) fanRotationSpeed = 360;
    }
}

void checkTellurionRotation(){
    if (ballAnimation == FORWARDS || ballAnimation ==  BACKWARDS)
    {
        ballRotation += ballAnimation == FORWARDS ? ballRotationSpeed : -ballRotationSpeed;
        if (ballRotationSpeed > 360) ballRotationSpeed = 0;
        else if (ballRotationSpeed < 0) ballRotationSpeed = 360;
    }
}

void checkBasketballRotation(){
    if (ballStart) {
        ballCurrentSpeed = ballStartSpeed;
        ballBouncing = true;
        ballStart = false;
    }
    if (ballBouncing) {
        ballCurrentSpeed -= gravity;
        if (relativeHeight + ballCurrentSpeed <= 0) {
            // collide with ground
            relativeHeight = 0;
            ballCurrentSpeed += resistence;
            if (ballCurrentSpeed >= 0) {
                // stop bouncing
                ballBouncing = false;
                ballCurrentSpeed = 0;
                relativeHeight = 0;
            } else {
                ballCurrentSpeed *= -1.0;
            }
        } else {
            relativeHeight += ballCurrentSpeed;
        }
    }
}


void drawSoundBox(){
    glPushMatrix();
    glColor3f(0.149f, 0.149f, 0.149f);
    glTranslatef(0, 1.85, 0);
    glScalef(0.5, 3.7, 0.5);
    drawCube();
    glPopMatrix();
    glPushMatrix();
//    glTranslatef(-0.25, -1.85, -0.51);
    glTranslatef(-0.25, 0, -0.26);
    buildBoard(&soundboxTexture, rgb(38,38,38), {0.5, 3.7, 0.01});
    glPopMatrix();
}

void drawCurtain(){
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glPushMatrix();
    curtainMesh0.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &curtainMesh0.vertices_pointers[0]);
    glNormalPointer(GL_FLOAT, 0, &curtainMesh0.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, curtainMesh0.indices_pointers.size(), GL_UNSIGNED_INT, &curtainMesh0.indices_pointers[0]);
    
    
    glPushMatrix();
    curtainMesh1.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &curtainMesh1.vertices_pointers[0]);
    glNormalPointer(GL_FLOAT, 0, &curtainMesh1.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, curtainMesh1.indices_pointers.size(), GL_UNSIGNED_INT, &curtainMesh1.indices_pointers[0]);
    glPopMatrix();
    
    glPushMatrix();
    curtainMesh2.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &curtainMesh2.vertices_pointers[0]);
    glNormalPointer(GL_FLOAT, 0, &curtainMesh2.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, curtainMesh2.indices_pointers.size(), GL_UNSIGNED_INT, &curtainMesh2.indices_pointers[0]);
    glPopMatrix();
    
    glPushMatrix();
    curtainMesh3.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &curtainMesh3.vertices_pointers[0]);
    glNormalPointer(GL_FLOAT, 0, &curtainMesh3.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, curtainMesh3.indices_pointers.size(), GL_UNSIGNED_INT, &curtainMesh3.indices_pointers[0]);
    glPopMatrix();
    
    glPushMatrix();
    curtainMesh4.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &curtainMesh4.vertices_pointers[0]);
    glNormalPointer(GL_FLOAT, 0, &curtainMesh4.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, curtainMesh4.indices_pointers.size(), GL_UNSIGNED_INT, &curtainMesh4.indices_pointers[0]);
    glPopMatrix();
    
    glPushMatrix();
    curtainMesh5.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &curtainMesh5.vertices_pointers[0]);
    glNormalPointer(GL_FLOAT, 0, &curtainMesh5.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, curtainMesh5.indices_pointers.size(), GL_UNSIGNED_INT, &curtainMesh5.indices_pointers[0]);
    
    glPopMatrix();
    
    glPushMatrix();
//    glEnable(GL_TEXTURE_2D);
//    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//    glBindTexture(GL_TEXTURE_2D, couchTexture1.id);
    curtainMesh6.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &curtainMesh6.vertices_pointers[0]);
//    glTexCoordPointer(2, GL_FLOAT, 0, &curtainMesh0.vertices_tex_coords[0]);
    glNormalPointer(GL_FLOAT, 0, &curtainMesh6.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, curtainMesh6.indices_pointers.size(), GL_UNSIGNED_INT, &curtainMesh6.indices_pointers[0]);
//    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPopMatrix();
}

void drawLamp(){
    glPushMatrix();

    glColor3f(0, 0, 0);
    glTranslatef(0, 0.2f, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(objCylinder, 0.5, 0.5, 0.2, 100, 100);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0.2f, 0);
    glRotatef(90, 1, 0, 0);
    DrawCircle(0.5);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.149f, 0.149f, 0.149f);
    glTranslatef(0, 5, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(objCylinder, 0.1, 0.1, 4.8, 100, 100);
//    DrawCircle(0.3);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 5, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(objCylinder, 0.3, 0.6, 1.0, 100, 100);
    glPopMatrix();
    glPopMatrix();
}

void drawCouch(){
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glEnableClientState(GL_NORMAL_ARRAY);
//    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    // left 2 pillow
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, couchTexture1.id);
    couchMesh0.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &couchMesh0.vertices_pointers[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &couchMesh0.vertices_tex_coords[0]);
    glNormalPointer(GL_FLOAT, 0, &couchMesh0.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, couchMesh0.indices_pointers.size(), GL_UNSIGNED_INT, &couchMesh0.indices_pointers[0]);
    glDisable(GL_TEXTURE_2D);
    // glFlush();

    // left 3 pillow
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, couchTexture1.id);
    couchMesh1.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &couchMesh1.vertices_pointers[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &couchMesh1.vertices_tex_coords[0]);
    glNormalPointer(GL_FLOAT, 0, &couchMesh1.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, couchMesh1.indices_pointers.size(), GL_UNSIGNED_INT, &couchMesh1.indices_pointers[0]);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //left 1 pillow
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, couchTexture2.id);
    couchMesh2.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &couchMesh2.vertices_pointers[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &couchMesh2.vertices_tex_coords[0]);
    glNormalPointer(GL_FLOAT, 0, &couchMesh2.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, couchMesh2.indices_pointers.size(), GL_UNSIGNED_INT, &couchMesh2.indices_pointers[0]);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, sheetTexture.id);
    couchMesh3.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &couchMesh3.vertices_pointers[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &couchMesh3.vertices_tex_coords[0]);
    glNormalPointer(GL_FLOAT, 0, &couchMesh3.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, couchMesh3.indices_pointers.size(), GL_UNSIGNED_INT, &couchMesh3.indices_pointers[0]);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, sheetTexture.id);
    couchMesh4.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &couchMesh4.vertices_pointers[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &couchMesh4.vertices_tex_coords[0]);
    glNormalPointer(GL_FLOAT, 0, &couchMesh4.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, couchMesh4.indices_pointers.size(), GL_UNSIGNED_INT, &couchMesh4.indices_pointers[0]);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, sheetTexture.id);
    couchMesh5.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &couchMesh5.vertices_pointers[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &couchMesh5.vertices_tex_coords[0]);
    glNormalPointer(GL_FLOAT, 0, &couchMesh5.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, couchMesh5.indices_pointers.size(), GL_UNSIGNED_INT, &couchMesh5.indices_pointers[0]);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, sheetTexture.id);
        couchMesh6.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &couchMesh6.vertices_pointers[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &couchMesh6.vertices_tex_coords[0]);
    glNormalPointer(GL_FLOAT, 0, &couchMesh6.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, couchMesh6.indices_pointers.size(), GL_UNSIGNED_INT, &couchMesh6.indices_pointers[0]);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, sheetTexture.id);
        couchMesh7.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &couchMesh7.vertices_pointers[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &couchMesh7.vertices_tex_coords[0]);
    glNormalPointer(GL_FLOAT, 0, &couchMesh7.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, couchMesh7.indices_pointers.size(), GL_UNSIGNED_INT, &couchMesh7.indices_pointers[0]);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, sheetTexture.id);
        couchMesh8.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &couchMesh8.vertices_pointers[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &couchMesh8.vertices_tex_coords[0]);
    glNormalPointer(GL_FLOAT, 0, &couchMesh8.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, couchMesh8.indices_pointers.size(), GL_UNSIGNED_INT, &couchMesh8.indices_pointers[0]);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, sheetTexture.id);
        couchMesh9.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &couchMesh9.vertices_pointers[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &couchMesh9.vertices_tex_coords[0]);
    glNormalPointer(GL_FLOAT, 0, &couchMesh9.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, couchMesh9.indices_pointers.size(), GL_UNSIGNED_INT, &couchMesh9.indices_pointers[0]);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, sheetTexture.id);
        couchMesh10.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &couchMesh10.vertices_pointers[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &couchMesh10.vertices_tex_coords[0]);
    glNormalPointer(GL_FLOAT, 0, &couchMesh10.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, couchMesh10.indices_pointers.size(), GL_UNSIGNED_INT, &couchMesh10.indices_pointers[0]);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, sheetTexture.id);
        couchMesh11.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &couchMesh11.vertices_pointers[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &couchMesh11.vertices_tex_coords[0]);
    glNormalPointer(GL_FLOAT, 0, &couchMesh11.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, couchMesh11.indices_pointers.size(), GL_UNSIGNED_INT, &couchMesh11.indices_pointers[0]);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, sheetTexture.id);
        couchMesh12.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &couchMesh12.vertices_pointers[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &couchMesh12.vertices_tex_coords[0]);
    glNormalPointer(GL_FLOAT, 0, &couchMesh12.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, couchMesh12.indices_pointers.size(), GL_UNSIGNED_INT, &couchMesh12.indices_pointers[0]);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, sheetTexture.id);
        couchMesh13.material.active();
    glVertexPointer(3, GL_FLOAT, 0, &couchMesh13.vertices_pointers[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &couchMesh13.vertices_tex_coords[0]);
    glNormalPointer(GL_FLOAT, 0, &couchMesh13.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, couchMesh13.indices_pointers.size(), GL_UNSIGNED_INT, &couchMesh13.indices_pointers[0]);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawBookShelf(){
    glPushMatrix();
    glTranslatef(9, 5, -3);
    glRotatef(90, 1, 0, 0);
    buildBoard(&woodTexture, rgb(212,134,84), {1, 4, 0.1});
    glPopMatrix();
    // Books
    glPushMatrix();
    glMaterialf(GL_FRONT, GL_SHININESS, 100);
    glColor3f(0.58f, 0, 0);
    glTranslatef(9.2f, 5.4f, -1);
    glScalef(0.5, 0.8, 0.1);
    drawCube();
    glPopMatrix();
    glPushMatrix();
    glMaterialf(GL_FRONT, GL_SHININESS, 100);
    glColor3f(0.04f, 0.39f, 0.58f);
    glTranslatef(9.2f, 5.4f, -0.8);
    glScalef(0.5, 0.8, 0.1);
    drawCube();
    glPopMatrix();
    glPushMatrix();
    glMaterialf(GL_FRONT, GL_SHININESS, 100);
    glColor3f(0.58f, 0.39f, 0);
    glTranslatef(9.2f, 5.4f, -1.3);
    glScalef(0.5, 0.8, 0.1);
    drawCube();
    glPopMatrix();
}

void renderScene(void) {

    // Clear Color and Depth Buffers

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Reset transformations
    glLoadIdentity();
    // Set the camera
    gluLookAt(x, 2.5f, z,
        x + lx, 2.5f + ly, z + lz,
        roll + 0.0f, 2.5f, 0.0f);
    
    setupLightning();
    ambient.active();
    
    
    // draw ground
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glTranslatef(0, -0.1, 0);
    drawGround();
    glPopAttrib();
    glPopMatrix();
    
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    if(flag == false){
        drawSkyBox();
    } else if (flag == true){
        drawCityBox();
    }
    glPopAttrib();
    glPopMatrix();
    
    // Tellurion on table
//    int iTimeElapsed = glutGet(GLUT_ELAPSED_TIME);
//    float fRevolveScale = 0.05142857f; //360/7000
    glPushMatrix();
        glTranslatef(9, 2.5, -1.5);
        glScaled(0.5, 0.5, 0.5);
        
        glPushMatrix();
            DrawFramework(1, 0.1, 1);
            checkTellurionRotation();
            glTranslatef(0, 1.1, 0);
            glRotatef(ballRotation, 0, 1, 0);
//            glRotatef(iTimeElapsed * fRevolveScale,0.0,1.0,0.0);
            glRotatef(-90, 1, 0, 0);
            DrawBall();
        glPopMatrix();
    glPopMatrix();
    
    // basketball
    glPushMatrix();
        checkBasketballRotation();
        glTranslatef(7.5, 0.5+relativeHeight, 6);
        DrawBasketball();
    glPopMatrix();
    
    // Draw floor
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 0.0f, -10.0f);
    glVertex3f(-10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, -10.0f);
    glEnd();
    
    // draw carpet
    glPushMatrix();
    glTranslatef(-7, 0.025, -7);
    glRotatef(90, 1, 0, 0);
    buildBoard(&carpetTexture, rgb(212,134,84), {13, 13, 0.001});
    glPopMatrix();

    //wall
    glColor3f(0.9294f, 0.9216f, 0.8353f);
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 0.0f, -10.0f);
    glVertex3f(-10.0f, 7.0f, -10.0f);
    glVertex3f(10.0f, 7.0f, -10.0f);
    glVertex3f(10.0f, 0.0f, -10.0f);
    glEnd();

    //wall with window
    glColor3f(1.0f, 0.851f, 0.702f);
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 0.0f, -10.0f);
    glVertex3f(-10.0f, 2.0f, -10.0f);
    glVertex3f(-10.0f, 2.0f, 10.0f);
    glVertex3f(-10.0f, 0.0f, 10.0f);
    glEnd();
    
    glColor3f(1.0f, 0.851f, 0.702f);
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 4.8f, -10.0f);
    glVertex3f(-10.0f, 7.0f, -10.0f);
    glVertex3f(-10.0f, 7.0f, 10.0f);
    glVertex3f(-10.0f, 4.8f, 10.0f);
    glEnd();
    
    glColor3f(1.0f, 0.851f, 0.702f);
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 4.8f, 10.0f);
    glVertex3f(-10.0f, 4.8f, 6.0f);
    glVertex3f(-10.0f, 2.0f, 6.0f);
    glVertex3f(-10.0f, 2.0f, 10.0f);
    glEnd();
    
    glColor3f(1.0f, 0.851f, 0.702f);
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 4.8f, 2.0f);
    glVertex3f(-10.0f, 4.8f, -2.0f);
    glVertex3f(-10.0f, 2.0f, -2.0f);
    glVertex3f(-10.0f, 2.0f, 2.0f);
    glEnd();
    
    glColor3f(1.0f, 0.851f, 0.702f);
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 4.8f, -6.0f);
    glVertex3f(-10.0f, 4.8f, -10.0f);
    glVertex3f(-10.0f, 2.0f, -10.0f);
    glVertex3f(-10.0f, 2.0f, -6.0f);
    glEnd();
//    glColor3f(1.0f, 0.851f, 0.702f);
//    glBegin(GL_QUADS);
//    glVertex3f(-10.0f, 0.0f, -10.0f);
//    glVertex3f(-10.0f, 7.0f, -10.0f);
//    glVertex3f(-10.0f, 7.0f, 10.0f);
//    glVertex3f(-10.0f, 0.0f, 10.0f);
//    glEnd();
    
    //wall with door
    glColor3f(1.0f, 0.851f, 0.702f);
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 0.0f, 10.0f);
    glVertex3f(-10.0f, 7.0f, 10.0f);
    glVertex3f(-7.0f, 7.0f, 10.0f);
    glVertex3f(-7.0f, 0.0f, 10.0f);
    glEnd();

    glColor3f(1.0f, 0.851f, 0.702f);
    glBegin(GL_QUADS);
    glVertex3f(-4.0f, 0.0f, 10.0f);
    glVertex3f(-4.0f, 7.0f, 10.0f);
    glVertex3f(10.0f, 7.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, 10.0f);
    glEnd();

    glColor3f(1.0f, 0.851f, 0.702f);
    glBegin(GL_QUADS);
    glVertex3f(-7.0f, 7.0f, 10.0f);
    glVertex3f(-7.0f, 4.9f, 10.0f);
    glVertex3f(-4.0f, 4.9f, 10.0f);
    glVertex3f(-4.0f, 7.0f, 10.0f);
    glEnd();

    glColor3f(0.4f, 0.2f, 0.0f);
    glLineWidth(30.0f);
    glBegin(GL_LINES);
    glVertex3f(-7.0f, 4.9f, 10.01f);
    glVertex3f(-4.0f, 4.9f, 10.01f);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(-7.0f, 4.9f, 10.01f);
    glVertex3f(-7.0f, 0.0f, 10.01f);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(-4.0f, 0.0f, 10.01f);
    glVertex3f(-4.0f, 4.9f, 10.01f);
    glEnd();

    //wall
    glColor3f(1.0f, 0.851f, 0.702f);
    glBegin(GL_QUADS);
    glVertex3f(10.0f, 0.0f, -10.0f);
    glVertex3f(10.0f, 7.0f, -10.0f);
    glVertex3f(10.0f, 7.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, 10.0f);
    glEnd();

    //ceiling
    glColor3f(0.95f, 0.95f, 0.95f);
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 7.0f, -10.0f);
    glVertex3f(10.0f, 7.0f, -10.0f);
    glVertex3f(10.0f, 7.0f, 10.0f);
    glVertex3f(-10.0f, 7.0f, 10.0f);
    glEnd();
    
//  left couch
    glPushMatrix();
        glTranslatef(-6, 0, -4);
        glRotatef(90, 0, 1, 0);
        glScalef(3, 3, 3);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        drawCouch();
        glFlush();
    glPopMatrix();
    
    // Draw tea table
    glPushMatrix();
        glTranslatef(-2, 0, -4.3);
        buildBoard(&woodTexture, rgb(212,134,84), {2.5, 1, 0.3});
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-2, 1, -4);
        glRotatef(90, 1, 0, 0);
        buildBoard(&woodTexture, rgb(212,134,84), {2.5, 5, 0.3});
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-2, 0, 1);
        buildBoard(&woodTexture, rgb(212,134,84), {2.5, 1, 0.3});
    glPopMatrix();
    
    // books on tea table
    glPushMatrix();
        glColor3f(0.91, 0.9, 0.93);
        glTranslatef(-1, 1.2f, 0.5f);
        glRotatef(90, 1, 0, 0);
        glScalef(0.8f, 0.5f, 0.1f);
        drawCube();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-1.3f, 1.4f, 0.2f);
        glRotatef(90, 1, 0, 0);
        buildBoard(&bookTexture1, rgb(232,230,237), {0.8f, 0.5f, 0.15f});
    glPopMatrix();
    
    // paper on tea table
    glPushMatrix();
    glTranslatef(-1, 1.2f, -1);
    glScalef(0.1, 0.1, 0.1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    paperMesh.material.active();
    paperMesh.material.dye();
    glVertexPointer(3, GL_FLOAT, 0, &paperMesh.vertices_pointers[0]);
    glNormalPointer(GL_FLOAT, 0, &paperMesh.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, paperMesh.indices_pointers.size(), GL_UNSIGNED_INT, &paperMesh.indices_pointers[0]);
    glPopMatrix();
    
    // celling lamp
    glPushMatrix();
    glTranslatef(0, 6.5f, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(objCylinder, 0.1, 0.1, 0.5, 100, 100);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.149f, 0.149f, 0.149f);
    glTranslatef(0, 6.1f, 0);
    glRotatef(90, 1, 0, 0);
    
    gluCylinder(objCylinder, 0.3, 0.3, 0.2, 100, 100);
    DrawCircle(0.3);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 6, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(objCylinder, 0.3, 1.0, 1.0, 100, 100);
    glPopMatrix();
    glPopMatrix();
    
    // right couch
    glPushMatrix();
        glTranslatef(4, 0, 2);
        glRotatef(-90, 0, 1, 0);
        glScalef(3, 3, 3);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        drawCouch();
        glFlush();
    glPopMatrix();
    
    //Floor pattern
    glColor3f(0.149f, 0.149f, 0.149f);
    glLineWidth(3.0f);
    for(int i = 0; i < 20; i += 2)
    {
        glBegin(GL_LINES);
        glVertex3f(-10.0f + i, 0.001f, -10.01f);
        glVertex3f(-10.0f + i, 0.001f, 10.01f);
        glEnd();
    }
    for(int i = 0; i < 20; i += 2)
    {
        glBegin(GL_LINES);
        glVertex3f(-10.0f, 0.001f, -10.01f + i);
        glVertex3f(10.0f, 0.001f, -10.01f + i);
        glEnd();
    }
    
    //windows
    Window w;
//    w.drawWindow1();
//    w.drawWindow2();
    w.drawWindowSill();
    glPushMatrix();
    glTranslatef( 0.0f, 0.0f, 8.0f);
    w.drawWindowSill();
    glPopMatrix();
    
    // curtain
    glPushMatrix();
    glTranslatef(-9.3, 2.8, -2);
//    glRotatef(90, 0, 1, 0);
    glScalef(1.5, 1.5, 1.5);
    drawCurtain();
    glFlush();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-9.3, 2.8, 5.45);
//    glRotatef(90, 0, 1, 0);
    glScalef(1.5, 1.5, 1.5);
    drawCurtain();
    glFlush();
    glPopMatrix();
    
    // star
    glPushMatrix();
        glTranslatef(7.25, 3, -9.8);
        glRotatef(180, 0, 1, 0);
        buildBoard(&vanGoghTexture, rgb(154, 149, 143), {4.5, 3, 0.1});
    glPopMatrix();
    
    // gogh
    glPushMatrix();
        glTranslatef(9.8, 3, 7);
        glRotatef(90, 0, 1, 0);
        buildBoard(&GoghTexture, rgb(154, 149, 143), {2, 3, 0.1});
    glPopMatrix();
    
    // sun flower
    glPushMatrix();
        glTranslatef(9.8, 3, -5);
        glRotatef(90, 0, 1, 0);
        buildBoard(&sunflowerTexture, rgb(154, 149, 143), {2, 3, 0.1});
    glPopMatrix();
    
    // bookshelf
    glPushMatrix();
    drawBookShelf();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, -1, 2.5);
    drawBookShelf();
    glPopMatrix();
    
    // sound box
    glPushMatrix();
    glTranslatef(-3.4, 0, 8.6);
    drawSoundBox();
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(2.5, 0, 8.6);
    drawSoundBox();
    glPopMatrix();
    
    // flower
    glPushMatrix();
    glTranslatef(5.5, 3, 9.8);
    buildBoard(&flowerTexture, rgb(154, 149, 143), {2, 3, 0.1});
    glPopMatrix();
    
    //tv
    glPushMatrix();
    glTranslatef(4, 2.7, 9.99);
    glScalef(0.12, 0.12, 0.12);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, wood1Texture.id);
    tvMesh0.material.active();
//    tvMesh0.material.dye();
    glVertexPointer(3, GL_FLOAT, 0, &tvMesh0.vertices_pointers[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &tvMesh0.vertices_tex_coords[0]);
    glNormalPointer(GL_FLOAT, 0, &tvMesh0.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, tvMesh0.indices_pointers.size(), GL_UNSIGNED_INT, &tvMesh0.indices_pointers[0]);
    glDisable(GL_TEXTURE_2D);
    
    glPushMatrix();
    tvMesh1.material.active();
    tvMesh1.material.dye();
    glVertexPointer(3, GL_FLOAT, 0, &tvMesh1.vertices_pointers[0]);
    glNormalPointer(GL_FLOAT, 0, &tvMesh1.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, tvMesh1.indices_pointers.size(), GL_UNSIGNED_INT, &tvMesh1.indices_pointers[0]);
    glPopMatrix();
    glPopMatrix();
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glBindTexture(GL_TEXTURE_2D, screenTexture.id);

        glTranslatef(-2.5, 2.2, 8);
        glBegin(GL_QUADS);
            glColor3f(1, 0, 0);

            glTexCoord2f(0, 1);
            glVertex3f(0, 0, 0);

            glTexCoord2f(1, 1);
            glVertex3f(3.6, 0, 0);

            glTexCoord2f(1, 0);
            glVertex3f(3.6, 1.9, 0);

            glTexCoord2f(0, 0);
            glVertex3f(0, 1.9, 0);
        glEnd();

        glDisable(GL_TEXTURE_2D);
        glFlush();
    glPopMatrix();
    
    // table along wall
    glPushMatrix();
        glTranslatef(8.5, 0, -2.5);
        buildBoard(&woodTexture, rgb(212,134,84), {1.5, 2.5, 0.1});
    glPopMatrix();
    glPushMatrix();
        glTranslatef(8.5, 2.5, -2.5);
        glRotatef(90, 1, 0, 0);
        buildBoard(&woodTexture, rgb(212,134,84), {1.5, 5, 0.1});
    glPopMatrix();
    glPushMatrix();
        glTranslatef(8.5, 0, 2.5);
        buildBoard(&woodTexture, rgb(212,134,84), {1.5, 2.5, 0.1});
    glPopMatrix();
    
    // plant on table
    glPushMatrix();
    glTranslatef(9, 2.5, 1.5);
    glRotatef(-90, 1, 0, 0);
    glScalef(0.01, 0.01, 0.01);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    plantMesh0.material.active();
    plantMesh0.material.dye();
    glVertexPointer(3, GL_FLOAT, 0, &plantMesh0.vertices_pointers[0]);
    glNormalPointer(GL_FLOAT, 0, &plantMesh0.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, plantMesh0.indices_pointers.size(), GL_UNSIGNED_INT, &plantMesh0.indices_pointers[0]);
    glPushMatrix();
    plantMesh1.material.active();
    plantMesh1.material.dye();
    glVertexPointer(3, GL_FLOAT, 0, &plantMesh1.vertices_pointers[0]);
    glNormalPointer(GL_FLOAT, 0, &plantMesh1.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, plantMesh1.indices_pointers.size(), GL_UNSIGNED_INT, &plantMesh1.indices_pointers[0]);
    glPopMatrix();
    glPopMatrix();
    
    // telescope
    glPushMatrix();
    glTranslatef(7.5, 1, 7.5);
    glScalef(0.15, 0.15, 0.15);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    telescopeMesh.material.active();
    telescopeMesh.material.dye();
    glVertexPointer(3, GL_FLOAT, 0, &telescopeMesh.vertices_pointers[0]);
    glNormalPointer(GL_FLOAT, 0, &telescopeMesh.vertices_normals[0]);
    glDrawElements(GL_TRIANGLES, telescopeMesh.indices_pointers.size(), GL_UNSIGNED_INT, &telescopeMesh.indices_pointers[0]);
    glPopMatrix();
    
    // floor lamp
    glPushMatrix();
    glTranslatef(8.5, 0, -8.5);
    drawLamp();
    glPopMatrix();
    
    // clock
    glPushMatrix();
    GLUquadricObj *objCylinder = gluNewQuadric();
    glColor3f(0,0,1);
    glTranslatef(0.25, 4.5, -9.8);
    gluCylinder(objCylinder, 1.1, 1.1, 0.4, 100, 100);
    glPopMatrix();
    int i;
    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(0.25, 4.5, -9.4);
    DrawCircle(1.1);
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    glBegin(GL_LINES);
    for(i=0; i<60; i++)
    {
        if(i%5)
        {
            // normal minute
            if(i%5 == 1)
                //glColor3f(1.0f, 1.0f, 1.0f);
                glColor3f(0.0f, 0.0f, 0.0f);
            newLine(minStart, minEnd, i*angle1min);
        }
        else
        {
            glColor3f(1.0f, 0.0f, 0.0f);
            newLine(stepStart, stepEnd, i*angle1min);
        }
    }
    glEnd();

    glLineWidth(3.0f);
    glBegin(GL_LINES);
    newLine(0.0f, 0.5f, -angleHour+M_PI/2);
    newLine(0.0f, 0.8f, -angleMin+M_PI/2);
    glEnd();

    glLineWidth(1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    newLine(0.0f, 0.8f, -angleSec+M_PI/2);
    glEnd();
    glPopMatrix();
    
    // van gogh
    glPushMatrix();
        glTranslatef(-2.25, 3, -9.8);
        glRotatef(180, 0, 1, 0);
        buildBoard(&starTexture, rgb(0, 77, 255), {4.5, 3, 0.1});
    glPopMatrix();

    // Fan
    glPushMatrix();
        GLfloat fanTranslate[3] = {-8, 2, -7};
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

        glPushMatrix();
            glTranslatef(fanTranslate[0], fanTranslate[1], fanTranslate[2]);
            glRotatef(30, 0, 1, 0);
            glRotatef(-90, 1, 0, 0);
            glScalef(0.05, 0.05, 0.05);

            fanMesh.material.active();
            fanMesh.material.dye();

            glVertexPointer(3, GL_FLOAT, 0, &fanMesh.vertices_pointers[0]);
            glNormalPointer(GL_FLOAT, 0, &fanMesh.vertices_normals[0]);
            glDrawElements(GL_TRIANGLES, fanMesh.indices_pointers.size(), GL_UNSIGNED_INT, &fanMesh.indices_pointers[0]);
        glPopMatrix();

        checkFanRotation();
        glTranslatef(fanTranslate[0], fanTranslate[1], fanTranslate[2]);
        glScalef(0.05, 0.05, 0.05);
        glRotatef(-90, 1, 0, 0);
        glRotatef(fanRotation, 0, 1, 0);

        glPushMatrix();
            fanPropellerMesh.material.active();
            fanPropellerMesh.material.dye();

            glVertexPointer(3, GL_FLOAT, 0, &fanPropellerMesh.vertices_pointers[0]);
            glNormalPointer(GL_FLOAT, 0, &fanPropellerMesh.vertices_normals[0]);
            glDrawElements(GL_TRIANGLES, fanPropellerMesh.indices_pointers.size(), GL_UNSIGNED_INT, &fanPropellerMesh.indices_pointers[0]);
        glPopMatrix();
    glPopMatrix();
    
    checkDoorAngle();
    buildDoor(&doorTexture, doorAngle);
    if(abs(mouseX) > 0.3){
        angle -= (0.004f * mouseX);
        lx = sin(angle);
        lz = -cos(angle);
    }
    if(abs(mouseY) > 0.3){
        if(abs(yAngle) < (M_PI/2)){
            yAngle += (0.002f * mouseY);
        }
        ly = sin(yAngle);
    }
    
    // window
    checkWindowTranslation();
    glPushMatrix();
        glTranslatef(-10, windowPosition.y, windowPosition.x);
        buildWindowGlass();
    glPopMatrix();
    glPushMatrix();
        GLfloat windowTranslationOffset = ((windowDimention.x - (2 * windowDimention.z)) / 2) * windowTranslation;
        glTranslatef(-9.9, windowPosition.y, (windowPosition.x) + windowTranslationOffset);
        buildWindowGlass();
    glPopMatrix();
//    buildWindow(silver);
    
    checkWindowTranslation();
    glPushMatrix();
        glTranslatef(-10, windowPosition1.y, windowPosition1.x);
        buildWindowGlass();
    glPopMatrix();
    glPushMatrix();
        GLfloat windowTranslationOffset1 = ((windowDimention.x - (2 * windowDimention.z)) / 2) * windowTranslation;
        glTranslatef(-9.9, windowPosition1.y, (windowPosition1.x) + windowTranslationOffset1);
        buildWindowGlass();
    glPopMatrix();
    glutSwapBuffers();
}

// Handles the events triggered when one of the arrow keys are pressed.
// @param key : key pressed
// @param xx : x coordinate of mouse position
// @param yy : y coordinate of mouse position
void processSpecialKeys(int key, int xx, int yy) {

    float fraction = 0.1f;

    switch (key) {
    case GLUT_KEY_LEFT:
        x += sin(angle - M_PI/2.0) * fraction;
        z += -cos(angle - M_PI/2.0) * fraction;
        break;
    case GLUT_KEY_RIGHT:
        x += sin(M_PI/2.0 + angle) * fraction;
        z += -cos(M_PI/2.0 + angle) * fraction;
        break;
    case GLUT_KEY_UP:
        x += lx * fraction;
        z += lz * fraction;
        break;
    case GLUT_KEY_DOWN:
        x -= lx * fraction;
        z -= lz * fraction;
        break;
    }
}


// Handles the events triggered when any key on the keyboard is pressed.
//Specifically, handles w,a,s,d and Esc.
// moves the camera frward, backward and sideways.
// @param key : key pressed
// @param xx : x coordinate of mouse position
// @param yy : y coordinate of mouse position
void processNormalKeys(unsigned char key, int xx, int yy) {
    float fraction = 0.1f;
    if(key == 'w'){
        x += lx * fraction;
        z += lz * fraction;
    } else if(key == 'a'){
        x += sin(angle - M_PI/2.0) * fraction;
        z += -cos(angle - M_PI/2.0) * fraction;
    } else if(key == 's'){
        x -= lx * fraction;
        z -= lz * fraction;
    } else if(key == 'd'){
        x += sin(M_PI/2.0 + angle) * fraction;
        z += -cos(M_PI/2.0 + angle) * fraction;
    } else if(key == 'r'){
        x = 9.5;
        z = 9.5;
    } else if(key == 'e'){
        x = -4;
        z = -1;
    } else if (key == 'x') {
        roll += 0.5f;
    } else if (key == 'z') {
        roll -= 0.5f;
    } else if (key == 'o') {
        doorAnimation = doorAngle == 90.0f ? BACKWARDS : FORWARDS;
    } else if (key == 'l') {
        light1_enabled = !light1_enabled;
        if (!light1_enabled) glDisable(GL_LIGHT1);
    } else if (key == 'v') {
        if (fanAnimation == IDLE) {
            fanRotationSpeed = FAN_LOW_SPEED;
            fanAnimation = FORWARDS;
        } else if (fanRotationSpeed == FAN_LOW_SPEED) {
            fanRotationSpeed = FAN_MAX_SPEED;
        } else if (fanRotationSpeed == FAN_MAX_SPEED) {
            fanAnimation = IDLE;
            fanRotationSpeed = FAN_LOW_SPEED;
        }
    } else if (key == 'k') {
        light2_enabled = !light2_enabled;
        if (!light2_enabled) glDisable(GL_LIGHT2);
    } else if (key == 'p') {
        windowAnimation = windowTranslation == 1.0f ? BACKWARDS : FORWARDS;
    } else if (key == 'j') {
        sun_enabled = !sun_enabled;
        if (!sun_enabled) glDisable(GL_LIGHT3);
    } else if (key == 't') {
        if (ballAnimation == IDLE) {
            ballRotationSpeed = BALL_LOW_SPEED;
            ballAnimation = FORWARDS;
        } else if (ballRotationSpeed == BALL_LOW_SPEED) {
            ballRotationSpeed = BALL_MAX_SPEED;
        } else if (ballRotationSpeed == BALL_MAX_SPEED) {
            ballAnimation = IDLE;
            ballRotationSpeed = BALL_LOW_SPEED;
        }
    } else if (key == 'b') {
        ballStart = true;
    } else if (key == 'y') {
        flag = !flag;
    }
    if (key == 27)
        exit(0);
}

 // Handles the events triggered when the mouse is moved in the window area.
// Handles yaw and pitch of the camera.
// @param xx : x coordinate of mouse position
// @param yy : y coordinate of mouse position
void processMouseMovement(int xx, int yy){
    mouseX = (float)(halfWidth - xx)/halfWidth;
    mouseY = (float)(halfHeight - yy)/halfHeight;
    angle -= (0.005f * mouseX);
    lx = sin(angle);
    lz = -cos(angle);

    if(abs(yAngle) < (M_PI/2)){
        yAngle += (0.005f * mouseY);
    }
    ly = sin(yAngle);
}

 // Adjusts the viewport sie when the window size is changed and sets the projection.
 // @param w : new width of window
 // @param h : new height of window
void changeSize(int w, int h) {
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;
    float ratio = w * 1.0 / h;

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    halfWidth = (float)(w/2.0);
    halfHeight = (float)(h/2.0);

    // Set the correct perspective.
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}

void animate () {
    //f.rotateFan();
    /* refresh screen */
    glutPostRedisplay();
}

int main(int argc, char **argv) {

    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_RGB);
    glutInitWindowPosition(5, 0);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Living Room");
    
    // register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutTimerFunc(33, TimerFunction, 1);        //void glutTimerFunc(unsigned int msecs,void (*func)(int value), value);

    //SetupRC();
    
    glutIdleFunc(animate);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutPassiveMotionFunc(processMouseMovement);

    
    fanLoader.LoadFile("/Users/hailey/Desktop/Coursework2/Coursework2/data/fan/fan.obj");
    couchLoader.LoadFile("/Users/hailey/Desktop/Coursework2/Coursework2/data/couch/Couch.obj");
    tvLoader.LoadFile("/Users/hailey/Desktop/Coursework2/Coursework2/data/tv/tv.obj");
    curtainLoader.LoadFile("/Users/hailey/Desktop/Coursework2/Coursework2/data/curtain/curtain.obj");
    plantLoader.LoadFile("/Users/hailey/Desktop/Coursework2/Coursework2/data/plant/plant.obj");
    telescopeLoader.LoadFile("/Users/hailey/Desktop/Coursework2/Coursework2/data/telescope/telescope.obj");
    paperLoader.LoadFile("/Users/hailey/Desktop/Coursework2/Coursework2/data/paper/paper.obj");
    

    // OpenGL init
//    glEnable(GL_DEPTH_TEST);

    // enter GLUT event processing cycle
    init();
    glutMainLoop();

    return 1;
}
