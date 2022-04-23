/*********

ADDITIONAL FEATURES:  I draw a star that shines and rotates in an orbit, there's a blinking red light when the spaceship gets close to moon, gives the promp message to user
*********/


#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"
#include <math.h>
#define V_D2R 0.0174532
#define V_R2D 57.29608323

typedef struct {
    vec_t pos;
    vec_t vel;  //velocity
    bool  visibility; // visibility: true if it has not catched the moon
} enemy_t;


enemy_t  E = { { 0, 200 },{},0 };
vec_t star = { -100,-100 };
int timeCnt = 0;


#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500

#define TIMER_PERIOD  26 // Period for the timer.
#define TIMER_ON       1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
float angle = 0;
typedef struct {
    float r, g, b;
} color_t;

typedef struct {
    vec_t   pos;
    color_t color;
    vec_t   vel;
} light_t;

typedef struct {
    vec_t pos;
    vec_t N;
} vertex_t;

#define NUM 3

light_t light ={ {    -250, -100 }, {0.992,0.984,0.839}, { 3,  2} };

color_t mulColor(float k, color_t c) {
    color_t tmp = { k * c.r, k * c.g, k * c.b };
    return tmp;
}

color_t addColor(color_t c1, color_t c2) {
    color_t tmp = { c1.r + c2.r, c1.g + c2.g, c1.b + c2.b };
    return tmp;
}

// To add distance into calculation
// when distance is 0  => its impact is 1.0
// when distance is 350 => impact is 0.0
// Linear impact of distance on light calculation.
double distanceImpact(double d) {
    return (-1.0 / 350.0) * d + 1.0;
}

color_t calculateColor(light_t source, vertex_t v) {
    vec_t L = subV(source.pos, v.pos);
    vec_t uL = unitV(L);
    float factor = dotP(uL, v.N) * distanceImpact(magV(L));
    return mulColor(factor, source.color);
}


//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

//
// To display onto window using OpenGL commands
//
void drawRocket(enemy_t e)
{
    glColor3f(1, 1, 1);
    glLineWidth(3);
    glBegin(GL_LINES);
   glVertex2f(e.pos.x, e.pos.y);
   glVertex2f(e.pos.x, e.pos.y+30);
   glEnd();
   glColor3f(1, 0, 0);
   if (E.pos.x >= light.pos.x - 80 && E.pos.x <= light.pos.x + 80 && E.pos.y <= light.pos.y + 80 && E.pos.y >= light.pos.y - 80)
       if(timeCnt%2==0)
       circle(e.pos.x, e.pos.y + 30, 3);
   glColor3f(1, 1, 1);
    circle_wire(e.pos.x, e.pos.y, 9);
    glColor3f(0.3, 0.5, 0.7);
    circle(e.pos.x, e.pos.y, 8);
    if (light.pos.x > e.pos.x) {
        glColor3f(0.7, 0.7, 1);
        glBegin(GL_TRIANGLES);
        glVertex2f(e.pos.x - 15, e.pos.y - 15);
        glVertex2f(e.pos.x - 15, e.pos.y + 15);
        glVertex2f(e.pos.x, e.pos.y);
        glEnd();
        glColor3f(0.5, 0.5, 0.9);
        glBegin(GL_TRIANGLES);
        glVertex2f(e.pos.x - 10, e.pos.y - 10);
        glVertex2f(e.pos.x - 10, e.pos.y + 10);
        glVertex2f(e.pos.x, e.pos.y);
        glEnd();
    }
    else
    {
       
    
        glColor3f(0.7, 0.7, 1);
        glBegin(GL_TRIANGLES);
        glVertex2f(e.pos.x + 15, e.pos.y - 15);
        glVertex2f(e.pos.x + 15, e.pos.y + 15);
        glVertex2f(e.pos.x, e.pos.y);
        glEnd();
        glColor3f(0.5, 0.5, 0.9);
        glBegin(GL_TRIANGLES);
        glVertex2f(e.pos.x + 10, e.pos.y - 10);
        glVertex2f(e.pos.x + 10, e.pos.y + 10);
        glVertex2f(e.pos.x, e.pos.y);
        glEnd();
       
    }

}
void drawStar(vec_t s)
{
    s.x = s.x * cos(angle * D2R) ;
    s.y = s.y* sin(angle * D2R) ;
  
    circle(s.x-200, s.y+200, 5);
    glBegin(GL_TRIANGLES);
    glVertex2f(s.x-204, s.y+204);
    glVertex2f(s.x-196, s.y+204);
    glVertex2f(s.x-200, s.y+220);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(s.x-204,s.y+ 204);
    glVertex2f(s.x-204, s.y+196);
    glVertex2f(s.x-220, s.y+200);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(s.x-196, s.y+204);
    glVertex2f(s.x-196, s.y+196);
    glVertex2f(s.x-180, s.y+200);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(s.x-204, s.y+196);
    glVertex2f(s.x-196, s.y+196);
    glVertex2f(s.x-200, s.y+180);
    glEnd();
}
void display() {

    //
    // clear window to black
    //
    glClearColor(0.71, 0.71, 0.71, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    if (timeCnt % 5 != 0) {
        glColor3f(0.85, 0.85, 0);
        drawStar(star);
        drawStar({ 200,-10 });
    }
    else
    {
        glColor3f(0.999999, 0.99999, 0);
        drawStar(star);
        drawStar({ 200,-10 });
    }
  
    // light source 
    glColor3f(0.3, 0.3, 0.7);
    if (!E.visibility)
        vprint(-110, 240, GLUT_BITMAP_HELVETICA_10, "Left click on the screen to display the spaceship");
   
        glColor3f(light.color.r, light.color.g, light.color.b);
        circle(light.pos.x, light.pos.y, 20);
    
        double y=-200;
        for (int x = -250; x <= 250; x++) {
            vertex_t P = { { x, -100 }, { 0, 1 } };

            color_t res = { 0, 0, 0 };
            {
                res = addColor(res, calculateColor(light, P));
            }

            glBegin(GL_LINES);
            glColor3f(res.r, res.g, res.b);
            glVertex2f(x, y += 0.2);

            glColor3f(0, 0, 0);
            glVertex2f(x, -250);
            glEnd();

        }
        if (E.visibility)
            drawRocket(E);

    glutSwapBuffers();

}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}
//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.
    x *= 1.0;
    y *= 1.0;
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
    {
        
        E.pos = { x - winWidth / 2.0 , winHeight / 2.0 - y };
        E.visibility = 1;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
    E.vel = mulV(2, unitV(subV(light.pos, E.pos)));
    E.pos = addV(E.pos, E.vel); // chase
    if (E.pos.x >= light.pos.x-15&& E.pos.x <= light.pos.x + 15&&  E.pos.y <= light.pos.y+15&& E.pos.y >= light.pos.y - 15)
        E.visibility = 0;
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    angle ++;
    if (angle > 360)
        angle -= 360;

    timeCnt ++;
  {
        light.pos = addV(light.pos, {3,2});

        // Reflection from Walls.
        if (light.pos.x > 250 || light.pos.x < -250) {
            light.pos = {-250,-100};
        }

        if (light.pos.y > 250 || light.pos.y < -250) {
            light.pos = { -250,-100 };
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Hit the Moon by Niyouasha Mojoudi");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}