/*********

I added bombs, you have to hit the targets without hitting the bombs in order to win.
I display an ending screen to let user know if they've win or lost the game.
*********/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500

#define  TIMER_PERIOD  20 
#define  TIMER_ON     1     // 0:disable timer, 1:enable timer

#define D2R 0.0174532
#define TARGET_RADIUS 20

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, spacebar = false, gamestart=true,win=false;
int  winWidth, winHeight,timeCnt=0; // current Window width and height
float r = 50, //amplitude
fq = 1,  //frequency
C = 0,   //horizontal phase shift
B = 0;   //vertical phase shift
float angle = 0;
/* User defined Types and Variables */
typedef struct {
    float x, y;
} point_t;

typedef struct {
    point_t pos;    // position of the object
    float   angle;  // view angle 
    float   r;
} player_t;

typedef struct {
    point_t pos;
    float angle;
    bool active;
} fire_t;

typedef struct {
    int r, g, b;
} color_t;

typedef struct {
    point_t center;
    bool active;
        
} target_t;
int radiusB=20;
#define MAX_FIRE 20 // 20 fires at a time.
#define FIRE_RATE 8 // after 8 frames you can throw another one.

player_t p = { {-250,0}, 45, 20 };
fire_t   fr[MAX_FIRE];
int fire_rate = 0;
target_t target = {{100,-250},1},target2= { {100,-250},1 },target3= { {-30,10},1 },target4= { {180,0},1 };

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



void drawPlayer(player_t tp) {
    glColor3f(1, 1, 1);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(tp.pos.x, tp.pos.y);
    glVertex2f(tp.pos.x + 50 * cos(tp.angle * D2R), tp.pos.y + 50 * sin(tp.angle * D2R));
    glEnd();
    glLineWidth(1);

   
   
  
    glColor3f(1, 1, 1);
    circle(tp.pos.x, tp.pos.y, tp.r - 5);
    glColor3f(0, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(tp.pos.x + 50 * cos(tp.angle * D2R), tp.pos.y + 50 * sin(tp.angle * D2R));
    glVertex2f(tp.pos.x, tp.pos.y + tp.r);
    glVertex2f(tp.pos.x, tp.pos.y - tp.r);
    glEnd();
    glColor3f(0.8, 0.8, 0.8);
    circle(tp.pos.x-5, tp.pos.y-5, tp.r-5);
    glColor3f(0, 0, 0);
    circle(tp.pos.x , tp.pos.y-4, 4);
  
}

void drawFires() {
    for (int i = 0; i < MAX_FIRE; i++) {
        if (fr[i].active) {
            glColor3f(0.2,0.3,0.5);
            circle(fr[i].pos.x, fr[i].pos.y, 4);
            if (fr[i].pos.x <target.center.x+radiusB&& fr[i].pos.x > target.center.x - radiusB && fr[i].pos.y < target.center.y+radiusB&& fr[i].pos.y> target.center.y - radiusB)
                target.active = 0;
            if (fr[i].pos.x <target2.center.x + 10 && fr[i].pos.x > target2.center.x - 10 && fr[i].pos.y < target2.center.y + 10 && fr[i].pos.y> target2.center.y - 10)
                target2.active = 0;
            if (fr[i].pos.x <target3.center.x + 5 && fr[i].pos.x > target3.center.x - 5 && fr[i].pos.y < target3.center.y + 5 && fr[i].pos.y> target3.center.y - 5)
                target3.active = 0;
            if (fr[i].pos.x <target4.center.x + 8 && fr[i].pos.x > target4.center.x - 8 && fr[i].pos.y < target4.center.y + 8 && fr[i].pos.y> target4.center.y - 8)
                target4.active = 0;
          
        }
    }
}

void drawBalloon() {
    glLineWidth(3);
    glColor3f(1, 1, 0); 
    glBegin(GL_LINE_STRIP);
            glVertex2f(target.center.x, target.center.y - radiusB);
            glVertex2f(target.center.x, target.center.y - radiusB - 40);
    glEnd();
    glLineWidth(1);

    glColor3f(0.9, 0.5, 0.7);
    circle(target.center.x, target.center.y, radiusB);

    glBegin(GL_TRIANGLES);
    glVertex2f(target.center.x, target.center.y - (radiusB * 0.66));
    glVertex2f(target.center.x - (radiusB / 5), target.center.y - (radiusB * 1.2));
    glVertex2f(target.center.x + (radiusB / 5), target.center.y - (radiusB * 1.2));
    glEnd();

    glColor3f(0.9, 0.9, 0.9);
    circle(target.center.x + (radiusB / 3), target.center.y + (radiusB / 3), (radiusB / 3));
}
float f(float x) {
    return r * sin((fq * x + C) * D2R) + B;
}
void drawHarmonic() {
  
    target2.center.x = r * cos(angle * D2R) - 100;
    target2.center.y = r * sin(angle * D2R) + 25;
    glColor3f(1, 1, 0);
    circle(r * cos(angle * D2R) - 100, r * sin(angle * D2R) + 25, 10);
    glBegin(GL_TRIANGLES);
    glVertex2f(r * cos(angle * D2R) - 90, r * sin(angle * D2R) + 30);
    glVertex2f(r * cos(angle * D2R) - 90, r * sin(angle * D2R) + 25);
    glVertex2f(r * cos(angle * D2R) - 80, r * sin(angle * D2R) + 27);
        glEnd();
        glColor3f(0.9, 0.6, 0);
        glBegin(GL_TRIANGLES);
        glVertex2f(r * cos(angle * D2R) - 100, r * sin(angle * D2R) + 30);
        glVertex2f(r * cos(angle * D2R) - 100, r * sin(angle * D2R) + 20);
        glVertex2f(r * cos(angle * D2R) - 115, r * sin(angle * D2R) + 25);
        glEnd();
        glColor3f(0, 0, 0);
        circle(r * cos(angle * D2R) - 95, r * sin(angle * D2R) + 30, 2);
        glBegin(GL_LINES);
        glVertex2f(r * cos(angle * D2R) - 100, r * sin(angle * D2R) +15);
        glVertex2f(r * cos(angle * D2R) - 100, r * sin(angle * D2R)+5 );
        glVertex2f(r * cos(angle * D2R) - 105, r * sin(angle * D2R) + 15);
        glVertex2f(r * cos(angle * D2R) - 105, r * sin(angle * D2R) + 5);
            glEnd();
           


   
}
void displayEnding()
{
    timeCnt++;
    if (win)
    {
        glColor3f(0.2, 0.9, 0.2);
        glRectf(-250, -250, 250, 250);
        glColor3f(0.2, 0.5, 0.2);
        circle(0, 0, (timeCnt / 2));
        if (timeCnt > 100) {
            glColor3f(1, 1, 1);
            vprint2(-60, 0, 0.2, "YOU WIN!");
        }
        if (timeCnt > 200)
            exit(0);
    }
    else if(!win)
    {

        glColor3f(0.9, 0.2, 0.2);
        glRectf(-250, -250, 250, 250);
        glColor3f(0.9, 0.5, 0);
        circle(0, 0, (timeCnt/2));
        if (timeCnt > 100) {
            glColor3f(1, 1, 1);
            vprint2(-70, -10, 0.2, "YOU LOOSE!");
        }
        if (timeCnt > 200)
            exit(0);
    }
  
}
void drawBombs(target_t target3)
{
    glColor3f(0, 0, 0);
    circle(target3.center.x, target3.center.y, 8);
    glColor3f(1, 1, 1);
    circle(target3.center.x+3, target3.center.y+3, 2);
    glLineWidth(3);
    glColor3f(0.62, 0.5, 0.381);
    glBegin(GL_LINES);
    glVertex2f(target3.center.x + 3, target3.center.y + 8);
    glVertex2f(target3.center.x + 3, target3.center.y + 13);
    glEnd(); 
    glLineWidth(1);
   

}
void display()
{
    // clear window to black
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.664,0.675,0.765);
    glRectf(-250, -250, 250, 250);
    glColor3f(0.4, 0.7, 0.85);
    glRectf(-250, -250, 250, -220);
    glColor3f(1,1,1);
    vprint2(-220, -230, 0.08, " Hit the targets without hitting the bombs");
    vprint2(-240, -240, 0.08, "press arrow keys to move the cannon / spacebar to throw the ball / F1 to restart ");
    drawFires();
    drawPlayer(p);
    if (target.active)
        drawBalloon();
    if (target2.active)
        drawHarmonic();
    
        if (!target.active && !target2.active) {

            win = true;
            displayEnding();

        }
        if (!target3.active || !target4.active)
        {

            win = false;
            displayEnding();
        }
        if(!win&&target3.active!=0&&target4.active!=0){
        srand(time(NULL));
        float randx = (rand() % 481) - 230;
        float randy = (rand() % 480) - 230;
        target3.center = { randx,randy };
        if (target3.active)
            drawBombs(target3);
        if (target4.active) {
            float randx = (rand() % 481) - 230;
            float randy = (rand() % 481) - 230;
            target4.center = { randx,randy };
            drawBombs(target4);
        }
    }
   
    glutSwapBuffers();
  
  

}

// In the fire array, check if any fire is available.
// if there is a fire that we can use for firing, return its index.
int findAvailableFire() {
    for (int i = 0; i < MAX_FIRE; i++) {
        if (fr[i].active == false) return i;
    }
    return -1;
}



//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
    if (key == ' ')
        spacebar = true;
    // to refresh the window it calls display() function
    //glutPostRedisplay() ;
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
    if (key == ' ')
        spacebar = false;
    // to refresh the window it calls display() function
    //glutPostRedisplay() ;
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
    if (key == GLUT_KEY_F1) {
        if (target.active)
            !target.active;
        else
            target.active = true;
        if (target2.active)
            !target2.active;
        else
            target2.active = true;

    }
    // to refresh the window it calls display() function
    //glutPostRedisplay() ;

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
    //glutPostRedisplay() ;
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



    // to refresh the window it calls display() function
    //glutPostRedisplay() ; 
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



void turnPlayer(player_t* tp, float inc) {
    tp->angle += inc;
    if (tp->angle > 85) tp->angle = 85;
    if (tp->angle < -85) tp->angle =-85;
}

void movePlayer(player_t* tp, float speed) {
   

    tp->pos.y += speed;
    float r = tp->r;

}



#if TIMER_ON == 1
void onTimer(int v) {

    
  
    angle-=3;
    if (angle <= 360)
        angle += 360;
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    target.center.y++;
    if (target.center.y >= 240)
        target.center.y -= 500;
    if (right) {
        turnPlayer(&p, -3);
    }
    if (left) {
        turnPlayer(&p, 3);
    }
    if (up) {
        movePlayer(&p, 3);
    }
    if (down) {
        movePlayer(&p, -3);
    }

    if (spacebar && fire_rate == 0) {
        int availFire = findAvailableFire();
        if (availFire != -1) {
            fr[availFire].pos = p.pos;
            fr[availFire].angle = p.angle;
            fr[availFire].active = true;
            fire_rate = FIRE_RATE;
        }
    }

    if (fire_rate > 0) fire_rate--;

    // Move all fires that are active.
    for (int i = 0; i < MAX_FIRE; i++) {
        if (fr[i].active) {
            fr[i].pos.x += 10 * cos(fr[i].angle * D2R);
            fr[i].pos.y += 10 * sin(fr[i].angle * D2R);

            if (fr[i].pos.x > 250 || fr[i].pos.x < -250 || fr[i].pos.y>250 || fr[i].pos.y < -250) {
                fr[i].active = false;
            }
           
           
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


void main(int argc, char* argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Kill the Bird, Pop the Ballon by Niyousha Mojoudi"); 

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


#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}