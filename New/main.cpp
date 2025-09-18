#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>+
#include <windows.h>  // for MS Windows
#include <mmsystem.h>

using namespace std;
//----------------global variable----------
 // Global variable for train movement

bool trainMoving = true;
float trainSpeed = 0.3f;
bool isDay = true; // Default to day mode
float trainPosX= 15.0f; // Controls train's horizontal movement bool moving = true; // Train moving toggle bool doorOpen = false; // Not used yet, reserved for door logic bool lightOn = true; // Controls front light
//-------------------------------------------------------
// cloud floting points
float cloudOffset = 0.0f;
float cloudSpeed = 0.02f;
float cloudX = -15.0f; // Initial position (off-screen left)

//------------------------------------
// values for rain
const int numDrops = 300;
float rainX[numDrops];
float rainY[numDrops];
bool isRaining = false;

void circle(float cx, float cy, float radius, int r, int g, int b)
{
    float pi = 3.1416;

    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);

    for (int i = 0; i < 200; i++)
    {
        float A = (i * 2 * pi) / 200;
        float x = cx + radius * cos(A);
        float y = cy + radius * sin(A);
        glVertex2f(x, y);
    }
    glEnd();
}



void drawCircle(float x, float y, float r)
{
    int n = 50;
    float t;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= n; i++)
    {
        t = 2 * 3.1415926f * i / n;
        glVertex2f(x + cos(t) * r, y + sin(t) * r);
    }
    glEnd();
}



void drawPerson(float x, float y, float scale, float r, float g, float b)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1);

    // Legs
    glColor3f(0.2f, 0.2f, 0.4f); // Dark blue pants
    glBegin(GL_QUADS);
    // Left Leg
    glVertex2f(-0.2f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.8f);
    glVertex2f(-0.2f, 0.8f);
    // Right Leg
    glVertex2f(0.05f, 0.0f);
    glVertex2f(0.25f, 0.0f);
    glVertex2f(0.25f, 0.8f);
    glVertex2f(0.05f, 0.8f);
    glEnd();

    // Torso (shirt color)
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(-0.25f, 0.8f);
    glVertex2f(0.3f, 0.8f);
    glVertex2f(0.3f, 1.8f);
    glVertex2f(-0.25f, 1.8f);
    glEnd();

    // Head
    glColor3f(0.9f, 0.7f, 0.5f); // Skin tone
    drawCircle(0.05f, 2.1f, 0.3f);
glEnd();
    glPopMatrix();
}

void drawAllPeople()
{

    // --- People on Platforms ---
  drawPerson(-8.0f, -4.4f, 1.1f, 0.8f, 0.3f, 0.8f); // Magenta shirt
    drawPerson(-2.0f, -4.4f, 0.9f, 0.5f, 0.2f, 0.8f); // Purple shirt
    drawPerson(0.0f, -4.4f, 1.0f, 0.8f, 0.3f, 0.8f);  // Magenta shirt
    drawPerson(2.0f, -4.4f, 0.9f, 0.8f, 0.3f, 0.5f);

      glEnd();
   glPopMatrix();
}


// ------------------------- Drawing Functions -------------------------

// Draw circle using GL_POLYGON
void drawCircle(float centerX, float centerY, float radius, int r, int g, int b, int segments = 100)
{
    glColor3ub(r, g, b); // Set fill color
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++)
    {
        float angle = 2.0f * 3.1416f * i / segments;
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}
void drawSky(float r, float g, float b)
{
    glColor3f(r, g, b); // Dynamic sky color
    glBegin(GL_QUADS);
    glVertex2f(-15.0f, 2.0f);
    glVertex2f(15.0f, 2.0f);
    glVertex2f(15.0f, 15.0f);
    glVertex2f(-15.0f, 15.0f);
    glEnd();
}
// Obj2
void drawSun(float cx, float cy, float radius, float r, float g, float b)
{
    glColor3f(r, g, b); // Dynamic sun/moon color
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 100; i++)
    {
        float angle = 2 * 3.1416f * i / 100;
        glVertex2f(cx + cos(angle) * radius, cy + sin(angle) * radius);
    }
    glEnd();
}


void drawRoad1()
{
    // uperpath
    glColor3ub(174, 136, 103);
    glBegin(GL_POLYGON);
    glVertex2f(-15, 2);
    glVertex2f(15, 2);
    glVertex2f(15, -1);
    glVertex2f(-15, -1);
    glEnd();

    // Draw road border (top edge)
    glColor3ub(120, 120, 120);
    glBegin(GL_QUADS);
    glVertex2f(-15.0f, 0.0f);
    glVertex2f(-15.0f, -0.5f);
    glVertex2f(15.0f, -0.5f);
    glVertex2f(15.0f, 0.0f);
    glEnd();

    // Draw main road
    glColor3f(0.7f, 0.7f, 0.7f); // Road surface color
    glBegin(GL_QUADS);
    glVertex2f(-15.0f, -0.5f);
    glVertex2f(-15.0f, -6.0f);
    glVertex2f(15.0f, -6.0f);
    glVertex2f(15.0f, -0.5f);
    glEnd();

    // Draw 3D-looking sidewalk below the road (beveled effect)
    // Top surface of sidewalk
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.6f, 0.6f); // Lighter top
    glVertex2f(-15.0f, -6.0f);
    glVertex2f(15.0f, -6.0f);
    glColor3f(0.4f, 0.4f, 0.4f); // Darker bottom
    glVertex2f(15.0f, -6.9);
    glVertex2f(-15.0f, -6.9f);
    glEnd();
}

// Tree inside garden3
void drawTree()
{
    // 1st
    glColor3ub(150, 75, 0);
    glBegin(GL_POLYGON);
    glVertex2f(12.9, 3.5);
    glVertex2f(12, 6);
    glVertex2f(12.1, 6);
    glVertex2f(12.9, 3.7);
    glEnd();
    drawCircle(11.7f, 5.5f, 0.6f, 0, 150, 0); // Draw green circle
    drawCircle(11.9f, 4.6f, 0.6f, 0, 150, 0); // Draw green circle
    drawCircle(12.3f, 5.2f, 0.6f, 0, 150, 0); // Draw green circle

    // 2nd Branch
    glColor3ub(150, 75, 0);
    glBegin(GL_POLYGON);
    glVertex2f(12.9, 6);
    glVertex2f(13.2, 6);
    glVertex2f(13.2, 2);
    glVertex2f(12.9, 2);
    glEnd();
    drawCircle(12.5f, 7.0f, 0.8f, 0, 190, 0); // Draw green circle
    drawCircle(13.5f, 7.0f, 0.8f, 0, 190, 0); // Draw green circle
    drawCircle(12.0f, 5.8f, 0.7f, 0, 190, 0); // Draw green circle
    drawCircle(13.0f, 5.9f, 0.7f, 0, 190, 0); // Draw green circle
    drawCircle(14.0f, 5.8f, 0.7f, 0, 190, 0); // Draw green circle

    // 3rd
    glColor3ub(150, 75, 0);
    glBegin(GL_POLYGON);
    glVertex2f(13.2, 4);
    glVertex2f(14.70, 5.8);
    glVertex2f(14.80, 5.8);
    glVertex2f(13.2, 3.8);
    glEnd();
    drawCircle(14.5f, 5.6f, 0.6f, 0, 150, 0); // Draw green circle
    drawCircle(13.9f, 5.3f, 0.6f, 0, 150, 0); // Draw green circle
    drawCircle(14.5f, 4.7f, 0.6f, 0, 150, 0); // Draw green circle
}
// tree at back
void drawTreeAt(float offsetX, float offsetY)
{
    // 1st Branch (left-leaning)
    glColor3ub(150, 75, 0);
    glBegin(GL_POLYGON);
    glVertex2f(offsetX + 0.0f, offsetY + 0.0f);
    glVertex2f(offsetX - 0.9f, offsetY + 2.5f);
    glVertex2f(offsetX - 0.8f, offsetY + 2.5f);
    glVertex2f(offsetX + 0.0f, offsetY + 0.2f);
    glEnd();

    drawCircle(offsetX - 1.2f, offsetY + 2.0f, 0.6f, 0, 150, 0);
    drawCircle(offsetX - 1.0f, offsetY + 1.1f, 0.6f, 0, 150, 0);
    drawCircle(offsetX - 0.6f, offsetY + 1.7f, 0.6f, 0, 150, 0);

    // 2nd Branch (center trunk)
    glColor3ub(150, 75, 0);
    glBegin(GL_POLYGON);
    glVertex2f(offsetX + 0.0f, offsetY + 2.5f);
    glVertex2f(offsetX + 0.3f, offsetY + 2.5f);
    glVertex2f(offsetX + 0.3f, offsetY - 1.5f);
    glVertex2f(offsetX + 0.0f, offsetY - 1.5f);
    glEnd();

    drawCircle(offsetX - 0.4f, offsetY + 3.5f, 0.8f, 0, 190, 0);
    drawCircle(offsetX + 0.6f, offsetY + 3.5f, 0.8f, 0, 190, 0);
    drawCircle(offsetX - 1.0f, offsetY + 2.3f, 0.7f, 0, 190, 0);
    drawCircle(offsetX + 0.0f, offsetY + 2.4f, 0.7f, 0, 190, 0);
    drawCircle(offsetX + 1.0f, offsetY + 2.3f, 0.7f, 0, 190, 0);

    // 3rd Branch (right-leaning)
    glColor3ub(150, 75, 0);
    glBegin(GL_POLYGON);
    glVertex2f(offsetX + 0.3f, offsetY + 1.0f);
    glVertex2f(offsetX + 1.8f, offsetY + 2.8f);
    glVertex2f(offsetX + 1.9f, offsetY + 2.8f);
    glVertex2f(offsetX + 0.3f, offsetY + 0.8f);
    glEnd();

    drawCircle(offsetX + 1.6f, offsetY + 2.6f, 0.6f, 0, 150, 0);
    drawCircle(offsetX + 1.0f, offsetY + 2.3f, 0.6f, 0, 150, 0);
    drawCircle(offsetX + 1.6f, offsetY + 1.7f, 0.6f, 0, 150, 0);
}

void drawShurb()
{

    // Shrub circles (bushes)
    float radius = 0.6f;

    // Shrubs in garden1 (-15 to -12.2, y = 1 to 2)
    drawCircle(-14.5f, 2.2f, radius, 0, 180, 0);
    drawCircle(-14.1f, 2.2f, radius, 0, 180, 0);
    drawCircle(-14.3f, 2.7f, radius, 0, 180, 0);

    drawCircle(-13.5f, 2.2f, radius, 0, 170, 0);
    drawCircle(-13.1f, 2.2f, radius, 0, 170, 0);
    drawCircle(-13.3f, 2.7f, radius, 0, 170, 0);




    // Left bush(12 to 15)
    drawCircle(13.5f, 2.2f, radius, 0, 180, 0);
    drawCircle(13.3f, 2.2f, radius, 0, 180, 0);
    drawCircle(13.1f, 2.7f, radius, 0, 180, 0);


    // Right bush
    drawCircle(14.0f, 2.2f, radius, 0, 160, 0);
    drawCircle(14.4f, 2.2f, radius, 0, 160, 0);
    drawCircle(14.2f, 2.7f, radius, 0, 160, 0);

}
//rail station khulna
void station()
{
    glPushMatrix();
    // --- Wall ---
    glColor3ub(211,211,211);
    glBegin(GL_POLYGON);
    glVertex2f(-13,5.5);//d
    glVertex2f(-8.5,5.5);//e
    glVertex2f(-8.5, -1.5);//f
    glVertex2f(-13, -1.5);//c
    glEnd();

    glColor3ub(128,128,128);
    glBegin(GL_POLYGON);
    glVertex2f(-8.5,5.5);//d
    glVertex2f(-7.8,5.6);//e
    glVertex2f(-7.8, -1);//f
    glVertex2f(-8.5, -1.5);//c
    glEnd();

    glColor3ub(211,211,211);
    glBegin(GL_POLYGON);
     glVertex2f(-7.8, -1);//h
    glVertex2f(-7.8,5.6);//g
    glVertex2f(-3,5.6);//i
    glVertex2f(-3, 7);//j
    glVertex2f(1,9);//k
    glVertex2f(5, 7);//l
    glVertex2f(5,7.6);//m
    glVertex2f(8,7.6);//n
    glVertex2f(8,6);//0
    glVertex2f(12,6);//p
    glVertex2f(12,-1);//q
    glEnd();


    //topleft  rectangle
    glColor3ub(128, 128, 128);
    glBegin(GL_POLYGON);
    glVertex2f(-7,5.6);//h
    glVertex2f(-7,6.5);//g
    glVertex2f(-3,6.5);//i
    glVertex2f(-3,5.6);//j
    glEnd();


    //topleft  rectangle
    glColor3ub(128, 128, 128);
    glBegin(GL_POLYGON);
    glVertex2f(8,6);//h
    glVertex2f(11,6);//g
    glVertex2f(11,6.8);//i
    glVertex2f(8,6.8);//j
    glEnd();

//topright  quard 1
    glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(11,5.4);//h
    glVertex2f(11.4,5.4);//g
    glVertex2f(11.4,5.8);//i
    glVertex2f(11,5.8);//j
    glEnd();

    //topright  quard 2
    glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(10,5.4);//h
    glVertex2f(10.4,5.4);//g
    glVertex2f(10.4,5.8);//i
    glVertex2f(10,5.8);//j
    glEnd();

    //topright  quard 3
    glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(9,5.4);//h
    glVertex2f(9.4,5.4);//g
    glVertex2f(9.4,5.8);//i
    glVertex2f(9,5.8);//j
    glEnd();

    //topright  quard 4
    glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(8,5.4);//h
    glVertex2f(8.4,5.4);//g
    glVertex2f(8.4,5.8);//i
    glVertex2f(8,5.8);//j
    glEnd();



    //topleft  quard 1
       glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(-6.4,5.8);//h
    glVertex2f(-6,5.8);//g
    glVertex2f(-6,6.2);//i
    glVertex2f(-6.4,6.2);//j
    glEnd();

    //topleft  quard 2
    glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(-5.6,5.8);//h
    glVertex2f(-5.2,5.8);//g
    glVertex2f(-5.2,6.2);//i
    glVertex2f(-5.6,6.2);//j
    glEnd();
    //topleft  quard 3

       glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(-4.8,5.8);//h
    glVertex2f(-4.4,5.8);//g
    glVertex2f(-4.4,6.2);//i
    glVertex2f(-4.8,6.2);//j
    glEnd();

    //topleft  quard 4

       glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(-4,5.8);//h
    glVertex2f(-3.6,5.8);//g
    glVertex2f(-3.6,6.2);//i
    glVertex2f(-4,6.2);//j
    glEnd();

    //topleft  quard 4

       glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(7.8,7);//h
    glVertex2f(7.8,7.4);//g
    glVertex2f(7.4,7.4);//i
    glVertex2f(7.4,7);//j
    glEnd();

     //topleft  quard 4

       glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(6.8,7);//h
    glVertex2f(6.8,7.4);//g
    glVertex2f(6.4,7.4);//i
    glVertex2f(6.4,7);//j
    glEnd();

    //topleft  quard 4

       glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(5.8,7);//h
    glVertex2f(5.8,7.4);//g
    glVertex2f(5.4,7.4);//i
    glVertex2f(5.4,7);//j
    glEnd();


    //topright big  quard 1
    glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(8.5,6);//h
    glVertex2f(9.2,6);//g
    glVertex2f(9.2,6.6);//i
    glVertex2f(8.5,6.6);//j
    glEnd();

    //topright big  quard 2
    glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(9.5,6);//h
    glVertex2f(10.2,6);//g
    glVertex2f(10.2,6.6);//i
    glVertex2f(9.5,6.6);//j
    glEnd();


    //lower surface 1
    glColor3ub(60,60,60);
    glBegin(GL_POLYGON);
    glVertex2f(-13,-1.5);//h
    glVertex2f(-13,-2);//g
    glVertex2f(-8.5,-2);//i
    glVertex2f(-8.5,-1.5);//j
    glEnd();

    //lower surface 1
    glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(-8.5,-1.5);//j
    glVertex2f(-8.5,-2);//j
    glVertex2f(-7.8,-1.5);//j
    glVertex2f(-7.8,-1);//j

    glEnd();




//lower surface 3
    glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(-7.8,-1);//j
    glVertex2f(-7.8,-1.5);//j
    glVertex2f(12,-1.5);//j
    glVertex2f(12,-1);//j

    glEnd();




    glPopMatrix();
}

void Window1(float x,float y,float z)
{
 glMatrixMode(GL_MODELVIEW);
 glPushMatrix();
glTranslated(x,y,z);
glColor3ub(140, 190, 214);
    glBegin(GL_POLYGON);
    glVertex2f(-12.5,1.5);//j
    glVertex2f(-12.5,4);//j
    glVertex2f(-11.75,4.5);//j
    glVertex2f(-11,4);//j
    glVertex2f(-11,1.5);//j

    glEnd();

//white lines

glLineWidth(7);
	glBegin(GL_LINES);
	glColor3f(255,255,255);
	glVertex2f(-12.2, 1.5);    // x, y
	glVertex2f(-12.2,4.24);    // x, y
	glEnd();

glLineWidth(7);
	glBegin(GL_LINES);
	glColor3f(255,255,255);
	glVertex2f(-11.4, 1.5);    // x, y
	glVertex2f(-11.4,4.27);    // x, y
	glEnd();
//blackline
	glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(0,0,0);
	glVertex2f(-12.5, 2);    // x, y
	glVertex2f(-11,2);    // x, y
	glEnd();


    glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(0,0,0);
	glVertex2f(-12.5, 3.5);    // x, y
	glVertex2f(-11,3.5);    // x, y
	glEnd();

	glLineWidth(1);

	glBegin(GL_LINES);
	glColor3f(0,0,0);
	glVertex2f(-11.75, 4.5);    // x, y
	glVertex2f(-11.75,1.5);    // x, y
	glEnd();
//black poly
	glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(-11.75,4.5);//j
    glVertex2f(-12.5,4);//j
    glVertex2f(-12.7,4);//j
    glVertex2f(-11.75,4.7);//j
    glVertex2f(-10.8,4);//j
        glVertex2f(-11,4);//j

    glEnd();

       glPopMatrix();
}
void Window2(float x,float y,float z)
{
    glMatrixMode(GL_MODELVIEW);
 glPushMatrix();
glTranslated(x,y,z);
  glColor3ub(140, 190, 214);
    glBegin(GL_POLYGON);
    glVertex2f(-12.5,-1.5);//j
    glVertex2f(-12.5,1);//j
    glVertex2f(-11,1);//j
    glVertex2f(-11,-1.5);//j

    glEnd();
//white lines 1


glLineWidth(7);
	glBegin(GL_LINES);
	glColor3f(255,255,255);
	glVertex2f(-12.2, -1.5);    // x, y
	glVertex2f(-12.2,1);    // x, y
	glEnd();


glLineWidth(7);
	glBegin(GL_LINES);
	glColor3f(255,255,255);
	glVertex2f(-11.4, -1.5);    // x, y
	glVertex2f(-11.4,1);    // x, y
	glEnd();

	glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(0,0,0);
	glVertex2f(-12.5, -1);    // x, y
	glVertex2f(-11,-1);    // x, y
	glEnd();


    glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(0,0,0);
	glVertex2f(-12.5, 0.5);    // x, y
	glVertex2f(-11,0.5);    // x, y
	glEnd();

	glLineWidth(1);

	glBegin(GL_LINES);
	glColor3f(0,0,0);
	glVertex2f(-11.75, -1.5);    // x, y
	glVertex2f(-11.75,1);    // x, y
	glEnd();

   glPopMatrix();

}

void Window3(float x,float y,float z)
{
    glMatrixMode(GL_MODELVIEW);
 glPushMatrix();
glTranslated(x,y,z);
  glColor3ub(140, 190, 214);
    glBegin(GL_POLYGON);
    glVertex2f(-7.5,-1);//j
    glVertex2f(-7.5,0.5);//j
    glVertex2f(-6,0.5);//j
    glVertex2f(-6,-1);//j

    glEnd();

    glColor3ub(255, 255, 255);
    glBegin(GL_POLYGON);
    glVertex2f(-7.5,0.5);//j
    glVertex2f(-7.65,0.5);//j
    glVertex2f(-7.65,0.6);//j
    glVertex2f(-5.85,0.6);//j
    glVertex2f(-5.85,0.5);//j
     glVertex2f(-6,0.5);//j

    glEnd();



//white lines 1


glLineWidth(7);
	glBegin(GL_LINES);
	glColor3f(255,255,255);
	glVertex2f(-7.2, -1);    // x, y
	glVertex2f(-7.2,0.5);    // x, y
	glEnd();


glLineWidth(7);
	glBegin(GL_LINES);
	glColor3f(255,255,255);
	glVertex2f(-6.3, -1);    // x, y
	glVertex2f(-6.3,0.5);    // x, y
	glEnd();

	glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(0,0,0);
	glVertex2f(-7.5, -0.8);    // x, y
	glVertex2f(-6,-0.8);    // x, y
	glEnd();


    glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(0,0,0);
	glVertex2f(-7.5, 0.2);    // x, y
	glVertex2f(-6,0.2);    // x, y
	glEnd();

	glLineWidth(1);

	glBegin(GL_LINES);
	glColor3f(0,0,0);
	glVertex2f(-6.75, -1);    // x, y
	glVertex2f(-6.75,0.5);    // x, y
	glEnd();

   glPopMatrix();

}

void MiddleObject()
{
// middle object main
 glColor3ub(150,75,0);
    glBegin(GL_POLYGON);
    glVertex2f(-2.5,1.8);//h
    glVertex2f(3.39,1.8);//g
    glVertex2f(6.68,0.87);//i
    glVertex2f(-5,0.87);//j
    glEnd();

   //2

 glColor3ub(101,67,33);
    glBegin(GL_POLYGON);
    glVertex2f(-5,0.87);//j
    glVertex2f(6.68,0.87);//i
     glVertex2f(6.68,0.62);//i
       glVertex2f(-5,0.62);//j
    glEnd();

//3
 glColor3ub(150,75,0);
    glBegin(GL_POLYGON);
    glVertex2f(6.46,0.62);//j
    glVertex2f(6.46,0.2);//i
     glVertex2f(-4.75,0.2);//i
       glVertex2f(-4.75,0.62);//j

    glEnd();

    //4
    glColor3ub(255,165,0);
    glBegin(GL_POLYGON);
    glVertex2f(-4.6,0.2);//j
     glVertex2f(-4.6,-2.8);//i
       glVertex2f(-4.2,-2.8);//j
       glVertex2f(-4.2,0.2);//j

    glEnd();
//5
    glColor3ub(255,165,0);
    glBegin(GL_POLYGON);
    glVertex2f(-4,0.2);//j
     glVertex2f(-4,-2);//i
       glVertex2f(-3.6,-2);//j
       glVertex2f(-3.6,0.2);//j

    glEnd();
//6
    glColor3ub(255,165,0);
    glBegin(GL_POLYGON);
    glVertex2f(-3.2,0.2);//j
     glVertex2f(-3.2,-1.6);//i
       glVertex2f(-2.8,-1.6);//j
       glVertex2f(-2.8,0.2);//j

    glEnd();
//7
    glColor3ub(255,165,0);
    glBegin(GL_POLYGON);
    glVertex2f(0,0.2);//j
     glVertex2f(0,-1.6);//i
       glVertex2f(0.4,-1.6);//j
       glVertex2f(0.4,0.2);//j

    glEnd();

    //8
    glColor3ub(255,165,0);
    glBegin(GL_POLYGON);
    glVertex2f(4.2,0.2);//j
     glVertex2f(4.2,-1.6);//i
       glVertex2f(4.6,-1.6);//j
       glVertex2f(4.6,0.2);//j

    glEnd();
    //9
    glColor3ub(255,165,0);
    glBegin(GL_POLYGON);
    glVertex2f(5,0.2);//j
     glVertex2f(5,-2);//i
       glVertex2f(5.4,-2);//j
       glVertex2f(5.4,0.2);//j

    glEnd();
//10
    glColor3ub(255,165,0);
    glBegin(GL_POLYGON);
    glVertex2f(5.8,0.2);//j
     glVertex2f(5.8,-2.8);//i
       glVertex2f(6.2,-2.8);//j
       glVertex2f(6.2,0.2);//j

    glEnd();


 glPopMatrix();
}

void renderBitmapString(float x, float y, float z, void *font, char *string)
{
    char *c;
    glRasterPos3f(x, y,z);
    for (c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void AddText() {


    glColor3f(255, 0,0);
    renderBitmapString(-2.6f, 1.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Khulna Railway Station");

    glFlush(); // Render now
}


void LowerPath()
{
    glColor3ub(220,220,220);
    glBegin(GL_POLYGON);
    glVertex2f(-3.4,-1.5);//h
    glVertex2f(-6,-3);//g
    glVertex2f(8,-3);//i
    glVertex2f(4.6,-1.5);//j
    glEnd();
glPopMatrix();
}
void drawBench(float x, float y, float scale)
{
    float width = 2.0f * scale;
    float legHeight = 0.5f * scale;
    float seatHeight = 0.3f * scale;
    float backHeight = 0.4f * scale;
    float legWidth = 0.2f * scale;

    // Seat
    glColor3f(0.6f, 0.4f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y - seatHeight);
    glVertex2f(x, y - seatHeight);
    glEnd();

    // Legs
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(x, y - seatHeight);
    glVertex2f(x + legWidth, y - seatHeight);
    glVertex2f(x + legWidth, y - seatHeight - legHeight);
    glVertex2f(x, y - seatHeight - legHeight);

    glVertex2f(x + width - legWidth, y - seatHeight);
    glVertex2f(x + width, y - seatHeight);
    glVertex2f(x + width, y - seatHeight - legHeight);
    glVertex2f(x + width - legWidth, y - seatHeight - legHeight);
    glEnd();

    // Backrest
    glColor3f(0.6f, 0.4f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(x, y + 0.1f * scale);
    glVertex2f(x + width, y + 0.1f * scale);
    glVertex2f(x + width, y + 0.1f * scale + backHeight);
    glVertex2f(x, y + 0.1f * scale + backHeight);
    glEnd();
}


void drawMoon()
{
    drawCircle(13.5f, 11.5f, 1.5f, 230, 230, 230);
    drawCircle(12.9f, 11.2f, 1.5f, 13, 13, 51);
}
// obj10
void drawStars()
{
    glColor3f(1.0f, 1.0f, 1.0f); // White stars

    glPointSize(2.0f);
    glBegin(GL_POINTS);

    // Random-like coordinates for stars
    glVertex2f(-12.0f, 12.0f);
    glVertex2f(-10.0f, 10.0f);
    glVertex2f(-8.0f, 13.0f);
    glVertex2f(-6.0f, 11.5f);
    glVertex2f(-4.0f, 14.0f);
    glVertex2f(-2.0f, 10.0f);
    glVertex2f(0.0f, 13.0f);
    glVertex2f(2.0f, 12.5f);
    glVertex2f(4.0f, 11.0f);
    glVertex2f(6.0f, 14.0f);
    glVertex2f(8.0f, 12.0f);
    glVertex2f(10.0f, 13.5f);
    glVertex2f(12.0f, 11.0f);

    glEnd();
}

// obj11

// Draw rain lines
void drawRain()
{
    glColor3f(0.5, 0.5, 1.0); // Light blue
    glBegin(GL_LINES);
    for (int i = 0; i < numDrops; i++)
    {
        glVertex2f(rainX[i], rainY[i]);
        glVertex2f(rainX[i], rainY[i] - 0.5);
    }
    glEnd();
}

// Initialize rain positions
void initRain()
{
    for (int i = 0; i < numDrops; i++)
    {
        rainX[i] = rand() % 40 - 20; // X range: -20 to 20
        rainY[i] = rand() % 30;      // Y range: 0 to 30
    }
}

// obj12
void drawLamp(float x, float y, float s)
{
    glColor3ub(255, 255, 153); // Light yellow color
    glBegin(GL_POLYGON);
    glVertex2f(x + s * 1, y + s * 5);
    glVertex2f(x + s * 1.5, y + s * 2);
    glVertex2f(x + s * 3.5, y + s * 2);
    glVertex2f(x + s * 4, y + s * 5);
    glEnd();

    glColor3ub(128, 128, 128); // Grey color
    glBegin(GL_POLYGON);
    glVertex2f(x + s * 0.8, y + s * 4.8);
    glVertex2f(x + s * 1.6, y + s * 5.6);
    glVertex2f(x + s * 3.4, y + s * 5.6);
    glVertex2f(x + s * 4.2, y + s * 4.8);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x + s * 1.6, y + s * 5.6);
    glVertex2f(x + s * 3.4, y + s * 5.6);
    glVertex2f(x + s * 3.4, y + s * 6);
    glVertex2f(x + s * 1.6, y + s * 6);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x + s * 3.6, y + s * 6);
    glVertex2f(x + s * 1.4, y + s * 6);
    glVertex2f(x + s * 1.8, y + s * 6.2);
    glVertex2f(x + s * 3.2, y + s * 6.2);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x + s * 1.8, y + s * 6.2);
    glVertex2f(x + s * 3.2, y + s * 6.2);
    glVertex2f(x + s * 3, y + s * 6.4);
    glVertex2f(x + s * 2, y + s * 6.4);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x + s * 1.5, y + s * 2);
    glVertex2f(x + s * 3.5, y + s * 2);
    glVertex2f(x + s * 2.8, y + s * 1.6);
    glVertex2f(x + s * 2.2, y + s * 1.6);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x + s * 2.8, y + s * 1.6);
    glVertex2f(x + s * 2.2, y + s * 1.6);
    glVertex2f(x + s * 2.2, y + s * -10);
    glVertex2f(x + s * 2.8, y + s * -10);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x + s * 2.2, y + s * -10);
    glVertex2f(x + s * 2.8, y + s * -10);
    glVertex2f(x + s * 3.5, y + s * -10.5);
    glVertex2f(x + s * 1.5, y + s * -10.5);
    glEnd();
}

// obj14

void drawTrainTrack(float x, float y, float s)
{

    glBegin(GL_POLYGON);
    glColor3ub(150, 150, 150);
    glVertex2f(-15, -9.5);
    glVertex2f(15, -9.5);
    glVertex2f(15, -12.5);
    glVertex2f(-15, -12.5);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(120, 120, 120);
    glVertex2f(15, -12.4);
    glVertex2f(-15, -12.4);
    glVertex2f(-15, -12.9);
    glVertex2f(15, -12.9);
    glEnd();

    // === Rails ===
    glColor3ub(80, 80, 80); // Dark gray
    glBegin(GL_QUADS);
    glVertex2f(x + s * -6.0, y + s * -0.35);
    glVertex2f(x + s * 6.0, y + s * -0.35);
    glVertex2f(x + s * 6.0, y + s * -0.2);
    glVertex2f(x + s * -6.0, y + s * -0.2);

    glVertex2f(x + s * -6.0, y + s * 0.2);
    glVertex2f(x + s * 6.0, y + s * 0.2);
    glVertex2f(x + s * 6.0, y + s * 0.1);
    glVertex2f(x + s * -6.0, y + s * 0.1);
    glEnd();

    // === Sleepers ===
    glColor3ub(139, 69, 19); // Brown
    float spacing = 0.6f;
    for (float i = -5.5; i <= 5.5; i += spacing)
    {
        glBegin(GL_QUADS);
        glVertex2f(x + s * i, y + s * -0.4);
        glVertex2f(x + s * i + s * 0.2, y + s * -0.4);
        glVertex2f(x + s * i + s * 0.2, y + s * 0.4);
        glVertex2f(x + s * i, y + s * 0.4);
        glEnd();
    }
}



void drawFild()
{
    glBegin(GL_QUADS);

    // Top (closer to viewer) – lighter green
    glColor3ub(0, 180, 20); // Light green
    glVertex2f(-15.0f, -6.0f);
    glVertex2f(15.0f, -6.0f);

    // Bottom (farther) – darker green
    glColor3ub(255, 255, 100); // Dark green
    glVertex2f(15.0f, -15.0f);
    glVertex2f(-15.0f, -15.0f);

    glEnd();
}

// obj17
void drawCloudes(float offsetX, float offsetY)
{
    drawCircle(cloudX + offsetX - 0.4f, offsetY + 3.5f, 0.8f, 255, 255, 255);
    drawCircle(cloudX + offsetX + 0.6f, offsetY + 3.5f, 0.8f, 255, 255, 255);
    drawCircle(cloudX + offsetX - 1.0f, offsetY + 2.3f, 0.7f, 255, 255, 255);
    drawCircle(cloudX + offsetX + 0.0f, offsetY + 2.4f, 0.7f, 255, 255, 255);
    drawCircle(cloudX + offsetX + 1.0f, offsetY + 2.3f, 0.7f, 255, 255, 255);
}



// Day night functions
void setDay()
{
    drawSky(0.7f, 0.9f, 1.0f);                     // Light blue sky
    drawSun(12.0f, 12.0f, 1.0f, 1.0f, 1.0f, 0.0f); // Yellow sun
}
// night functions
void setNight()
{
    drawSky(0.05f, 0.05f, 0.2f); // Dark blue sky
    drawMoon();                  // White moon
    drawStars();                 // Show stars at night
}

// ------------------------- Scene Logic -------------------------

void update(int value)
{
    float speed = 0.1f;


    // cloudes movement
    cloudX += 0.02f; // Speed of cloud movement

    if (cloudX > 18.0f) // Reset when cloud goes off screen
        cloudX = -15.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}
// Update rain positions
void updateRain(int value)
{
    if (isRaining)
    {
        for (int i = 0; i < numDrops; i++)
        {
            rainY[i] -= 0.5;
            if (rainY[i] < -10)
            {
                rainY[i] = 30; // Reset to top
                rainX[i] = rand() % 40 - 20;
            }
        }
        glutPostRedisplay();              // Redraw
        glutTimerFunc(30, updateRain, 0); // Repeat
    }
}



//----------------mouse functions---------------
// Mouse input
void mouse1(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        isRaining = true;
        glutTimerFunc(0, updateRain, 0); // Start animation
    }
}
//-------------------------keyBoard Funtions---------------
/*void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'd':
    case 'D':
        isDay = true;
        glutPostRedisplay();
        break;
    case 'n':
    case 'N':
        isDay = false;
        glutPostRedisplay();
        break;
    case 'r':
    case 'R':
        isRaining = true;
        glutTimerFunc(0, updateRain, 0);
        break;
         case 's': // Stop train
    case 'S':
        trainMoving = !trainMoving; // Toggle movement
        break;



    /*case '1':
        glutDestroyWindow(glutGetWindow());
        system("Railway_station.exe");
        break;
    case '2':
        glutDestroyWindow(glutGetWindow());
        system("City.exe");
        break;
    case '3':
        glutDestroyWindow(glutGetWindow());
        system("Village.exe");
        break;

    }
}
*/
void drawWindow(float x, float y) {
    glColor3f(0.6f, 0.8f, 1.0f);
    glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x + 0.8f, y);
        glVertex2f(x + 0.8f, y + 1.2f);
        glVertex2f(x, y + 1.2f);
    glEnd();
}

void drawWheel(float x, float y, float r) {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i=0;i<360;i++){
        float a=i*3.1416/180;
        glVertex2f(x + r*cos(a), y + r*sin(a));
    }
    glEnd();
}

void drawConnector(float x, float y){
    glColor3f(0.2f,0.2f,0.2f);
    glBegin(GL_POLYGON);
        glVertex2f(x,y+0.8f);
        glVertex2f(x+0.8f,y+0.8f);
        glVertex2f(x+0.8f,y+1.2f);
        glVertex2f(x,y+1.2f);
    glEnd();
}

// ------------ BOGIE ------------
void drawBogie(float x,float y){
    glColor3f(0.4f,0.2f,0.1f);
    glBegin(GL_POLYGON);
        glVertex2f(x,y);
        glVertex2f(x+6.4f,y);
        glVertex2f(x+6.4f,y+2.8f);
        glVertex2f(x,y+2.8f);
    glEnd();

    glColor3f(0.8f,0.5f,0.2f);
    glBegin(GL_POLYGON);
        glVertex2f(x,y+2.8f);
        glVertex2f(x+6.4f,y+2.8f);
        glVertex2f(x+6.2f,y+3.2f);
        glVertex2f(x+0.2f,y+3.2f);
    glEnd();

    for(int i=0;i<6;i++)
        drawWindow(x+0.4f+i*1.0f,y+1.2f);

    drawWheel(x+1.2f,y-0.2f,0.4f);
    drawWheel(x+2.4f,y-0.2f,0.4f);
    drawWheel(x+4.0f,y-0.2f,0.4f);
    drawWheel(x+5.2f,y-0.2f,0.4f);
}

// ------------ ENGINE ------------
void drawEngine(float x,float y){
    glColor3f(0.4f,0.2f,0.1f);
    glBegin(GL_POLYGON);
        glVertex2f(x,y);
        glVertex2f(x+6.4f,y);
        glVertex2f(x+6.8f,y+0.4f);
        glVertex2f(x+4.8f,y+2.8f);
        glVertex2f(x,y+2.8f);
    glEnd();

    glColor3f(0.3f,0.15f,0.08f);
    glBegin(GL_POLYGON);
        glVertex2f(x+4.8f,y+0.8f);
        glVertex2f(x+4.8f,y+2.8f);
        glVertex2f(x+5.6f,y+2.8f);
        glVertex2f(x+6.4f,y+2.0f);
        glVertex2f(x+6.4f,y+0.8f);
    glEnd();

    glColor3f(0.5f,0.5f,0.5f);
    glBegin(GL_POLYGON);
        glVertex2f(x+6.4f,y);
        glVertex2f(x+6.8f,y);
        glVertex2f(x+6.8f,y+0.8f);
        glVertex2f(x+6.4f,y+0.8f);
    glEnd();

    glColor3f(0.2f,0.2f,0.2f);
    glBegin(GL_POLYGON);
        glVertex2f(x+5.2f,y+2.8f);
        glVertex2f(x+5.6f,y+2.8f);
        glVertex2f(x+5.6f,y+3.8f);
        glVertex2f(x+5.2f,y+3.8f);
    glEnd();

    drawWindow(x+2.0f,y+1.2f);
    drawWindow(x+3.2f,y+1.2f);

    drawWheel(x+1.4f,y-0.2f,0.5f);
    drawWheel(x+3.0f,y-0.2f,0.5f);
    drawWheel(x+4.4f,y-0.2f,0.5f);
    drawWheel(x+5.6f,y-0.2f,0.5f);

    glColor3f(0.3f,0.3f,0.3f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x+6.8f,y);
        glVertex2f(x+7.6f,y-0.6f);
        glVertex2f(x+6.8f,y+0.8f);
    glEnd();
}

// ------------ TRAIN ------------
void drawTrain(){
    float baseY = -11.5f;
    float startX = -15.0f + trainPosX;
    float gap = 0.05f;

    for(int i=0;i<5;i++){
        drawBogie(startX + i*(6.4f+gap), baseY);
        drawConnector(startX + i*(6.4f+gap)+6.4f, baseY);
    }

    drawEngine(startX+5*(6.4f+gap), baseY);


    }


void updateTrain()
{
    if (trainMoving)
    { // Only move if trainMoving is true
        trainPosX += trainSpeed;
        if (trainPosX > 15.0f)
        {
            trainPosX = -40.0f;
        }
        glutPostRedisplay();
    }
}

void sound()
{

    //PlaySound("a.wav", NULL, SND_ASYNC|SND_FILENAME);
    PlaySound("ab.wav", NULL,SND_ASYNC|SND_FILENAME|SND_LOOP);

}


// Train animation timer
void trainTimer(int value)
{
    updateTrain();
    glutTimerFunc(16, trainTimer, 0); // Call every ~16ms (60 FPS)
}

//-------------chanege senario------------
void RailwayStation()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (isDay)
    {
        setDay();
    }
    else
    {
        setNight();
    }

    drawCloudes(1.0f, 10.0f);
    drawCloudes(5.0f, 8.0f);
    drawCloudes(9.0f, 9.0f);
    drawCloudes(-12.0f, 9.0f);
    drawCloudes(-8.0, 10.0f);
    drawCloudes(-16.0, 10.0f);
    drawCloudes(18.0, 11.0f);
    drawCloudes(20.0, 10.0f);
    drawCloudes(12.5, 11.0f);


    drawTreeAt(13.4f, 4.4f);
    drawTreeAt(-13.25, 3.1);
   drawRoad1();
   //drawShurb();


drawShurb();

    drawLamp(-15.0f, 4.3f, 0.4f); // Position at (-14, 3.3) with 30% size

    station();
    //windows
    Window1(0,0,0);
      Window1(2,0,0);
      Window2(0,0,0);
       Window2(2,0,0);
        Window3(0,0,0);
       Window3(0,2,0);
       Window3(0,4,0);

        Window3(2,0,0);
       Window3(2,2,0);
       Window3(2,4,0);

Window3(5,0,0);
Window3(6,0,0);
Window3(7,0,0);
Window3(8,0,0);
Window3(9,0,0);
Window3(10,0,0);
Window3(11,0,0);

Window3(5,2,0);
Window3(7,2,0);
Window3(9,2,0);
  Window3(11,2,0);
Window2(18.5,5,0);

Window1(10,2,0);
Window1(12,2,0);
Window1(14,2,0);
Window1(16,2,0);

 Window2(18.5,0.5,0);
 Window2(18.5,2,0);
 Window2(18.5,4,0);
 Window2(18.5,5,0);

 Window1(20.5,0,0);
  Window1(22.5,0,0);
  Window3(15.5,0,0);
   Window3(17.5,0,0);


   LowerPath();

    drawAllPeople();


   MiddleObject();

 float scale = 1.5f;
    drawBench(8, -3, scale);//Left

    drawBench(-11.0f, -4.0f, scale); // Right

MiddleObject();

AddText();
    drawFild();
    drawTrainTrack(0.0f, -11.0f, 3.0f);
    drawTrain();
   // drawTrain(trainPosX, -1.5f, 1.9f); // ✅ Use trainPosX instead of fixed value

    if (isRaining)
        drawRain();

    glutSwapBuffers();
}

bool isDay1 = true; // Default to day mode
// bus
float busX = -50.0f; // Start off-screen
// truck
float truckX = -50.0f; // Start off-screen
const float angleRad = 120.0f * 3.14159265f / 180.0f;
const float dirX = cos(angleRad);
const float dirY = sin(angleRad);
//-------------------------------------------------------
// cloud floting points
float cloudOffset1 = 0.0f;
float cloudSpeed2 = 0.04f;
float cloudX1 = -15.0f; // Initial position (off-screen left)
// car
float carX = -20.0f; // Start position (off-screen left)
//------------------------------------
// values for rain
const int numDrops1 = 500;
float rainX1[numDrops1];
float rainY1[numDrops1];
bool isRaining1 = false;

// Draw circle using GL_POLYGON


void circle1(float cx, float cy, float radius, int r, int g, int b)

{

    float pi = 3.1416;

    glColor3ub(r, g, b);

    glBegin(GL_POLYGON);

    for (int i = 0; i < 500; i++)

    {

        float A1 = (i * 2 * pi) / 500;

        float x1 = cx + radius * cos(A1);

        float y1 = cy + radius * sin(A1);

        glVertex2f(x1, y1);

    }

    glEnd();

}

// ------------------------- Drawing Functions -------------------------

// Draw circle using GL_POLYGON

void drawCircle1(float centerX, float centerY, float radius, int r, int g, int b, int segments = 100)

{

    glColor3ub(r, g, b); // Set fill color

    glBegin(GL_POLYGON);

    for (int i = 0; i < segments; i++)

    {

        float angle = 2.0f * 3.1416f * i / segments;

        float x = centerX + radius * cos(angle);

        float y = centerY + radius * sin(angle);

        glVertex2f(x, y);

    }

    glEnd();

}

void drawSky1(float r, float g, float b)

{

    glColor3f(r, g, b); // Dynamic sky color

    glBegin(GL_QUADS);

    glVertex2f(-15.0f, 2.0f);

    glVertex2f(15.0f, 2.0f);

    glVertex2f(15.0f, 15.0f);

    glVertex2f(-15.0f, 15.0f);

    glEnd();

}

// Obj2

void drawSun1(float cx, float cy, float radius, float r, float g, float b)

{

    glColor3f(r, g, b); // Dynamic sun/moon color

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(cx, cy);

    for (int i = 0; i <= 100; i++)

    {

        float angle = 2 * 3.1416f * i / 100;

        glVertex2f(cx + cos(angle) * radius, cy + sin(angle) * radius);

    }

    glEnd();

}




// Obj3

//  Function to draw a house with color parameters

void drawhouse(GLubyte r1, GLubyte g1, GLubyte b1, // primary wall color

               GLubyte r2, GLubyte g2, GLubyte b2, // roof color

               GLubyte r3, GLubyte g3, GLubyte b3, // border/door color

               GLubyte r4, GLubyte g4, GLubyte b4  // garage/road color

)

{

    // Chimney

    glColor3ub(178, 34, 34);

    glBegin(GL_POLYGON);

    glVertex2f(2.5, 8);

    glVertex2f(2.5, 9.5);

    glVertex2f(3.5, 9.5);

    glVertex2f(3.5, 8);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(2.2, 9.6);

    glVertex2f(2.2, 9.3);

    glVertex2f(3.8, 9.3);

    glVertex2f(3.8, 9.6);

    glEnd();

    // Roof

    glColor3ub(r2, g2, b2);

    glBegin(GL_POLYGON);

    glVertex2f(4.5f, 9.7f);

    glVertex2f(1.5f, 7.5f);

    glVertex2f(2.0f, 7.5f);

    glVertex2f(4.5f, 9.0f);

    glVertex2f(7.0f, 7.5f);

    glVertex2f(7.5f, 7.5f);

    glEnd();

    // Wall

    glColor3ub(r1, g1, b1);

    glBegin(GL_POLYGON);

    glVertex2f(2.0f, 7.5f);

    glVertex2f(2.0f, 2.5f);

    glVertex2f(7.0f, 2.5f);

    glVertex2f(7.0f, 7.5f);

    glVertex2f(4.5f, 9.0f);

    glEnd();

    // House Borders

    glColor3ub(r3, g3, b3);

    glBegin(GL_POLYGON);

    glVertex2f(2, 2.5);

    glVertex2f(1.6, 2);

    glVertex2f(7, 2);

    glVertex2f(7, 2.5);

    glEnd();

    // Side house borders

    glColor3ub(r3 - 30, g3 - 30, b3 - 30);

    glBegin(GL_POLYGON);

    glVertex2f(7, 2);

    glVertex2f(7, 2.3);

    glVertex2f(11.5, 2.3);

    glVertex2f(11.5, 2);

    glEnd();

    // Roof 2

    glColor3ub(r2, g2, b2);

    glBegin(GL_POLYGON);

    glVertex2f(7, 5.5);

    glVertex2f(12, 5.5);

    glVertex2f(11, 7);

    glVertex2f(7, 7);

    glEnd();

    // Wall 2

    glColor3ub(r1 - 20, g1 - 20, b1 - 20);

    glBegin(GL_POLYGON);

    glVertex2f(7, 2.3);

    glVertex2f(7, 5.5);

    glVertex2f(11.5, 5.5);

    glVertex2f(11.5, 2.3);

    glEnd();

    // Garage

    glColor3ub(r4, g4, b4);

    glBegin(GL_POLYGON);

    glVertex2f(8, 4.5);

    glVertex2f(8, 2);

    glVertex2f(10.5, 2);

    glVertex2f(10.5, 4.5);

    glEnd();

    // Garage hood

    glBegin(GL_POLYGON);

    glVertex2f(7.8, 4.4);

    glVertex2f(7.8, 4.5);

    glVertex2f(10.7, 4.5);

    glVertex2f(10.7, 4.4);

    glEnd();

    // Border 3

    glBegin(GL_POLYGON);

    glColor3ub(210, 210, 210);

    glVertex2f(7, 5.5);

    glVertex2f(7, 5.2);

    glVertex2f(12, 5.2);

    glVertex2f(12, 5.5);

    glEnd();

    // Door

    glBegin(GL_POLYGON);

    glColor3ub(222, 150, 150);

    glVertex2f(4, 5);

    glVertex2f(5, 5);

    glVertex2f(5, 2.5);

    glVertex2f(4, 2.5);

    glEnd();

    // door border

    glBegin(GL_POLYGON);

    glColor3ub(240, 240, 240);

    glVertex2f(3.8, 2.6);

    glVertex2f(3.8, 2);

    glVertex2f(5.2, 2);

    glVertex2f(5.2, 2.6);

    glEnd();

    // House Road

    glColor3ub(150, 150, 150);

    glBegin(GL_POLYGON);

    glVertex2f(5.2, 2);

    glVertex2f(5.5, 0);

    glVertex2f(3.5, 0);

    glVertex2f(3.8, 2);

    glEnd();

}

void drawRoad5()

{

    // footpath

    glColor3ub(200, 200, 200);

    glBegin(GL_POLYGON);

    glVertex2f(-15, 2);

    glVertex2f(15, 2);

    glVertex2f(15, -1);

    glVertex2f(-15, -1);

    glEnd();

    // Draw road border (top edge)

    glColor3ub(120, 120, 120);

    glBegin(GL_QUADS);

    glVertex2f(-15.0f, 0.0f);

    glVertex2f(-15.0f, -0.5f);

    glVertex2f(15.0f, -0.5f);

    glVertex2f(15.0f, 0.0f);

    glEnd();

    // Draw main road

    glColor3f(0.7f, 0.7f, 0.7f); // Road surface color

    glBegin(GL_QUADS);

    glVertex2f(-15.0f, -0.5f);

    glVertex2f(-15.0f, -6.0f);

    glVertex2f(15.0f, -6.0f);

    glVertex2f(15.0f, -0.5f);

    glEnd();

    // Draw white dashed center lines

    glColor3f(1.0f, 1.0f, 1.0f);

    for (float i = -15.0f; i < 15.0f; i += 5.0f)

    {

        glBegin(GL_QUADS);

        glVertex2f(i + 0.6f, -3.2f);

        glVertex2f(i + 1.8f, -3.2f);

        glVertex2f(i + 1.8f, -3.6f);

        glVertex2f(i + 0.6f, -3.6f);

        glEnd();

    }

    // Draw 3D-looking sidewalk below the road (beveled effect)

    // Top surface of sidewalk

    glBegin(GL_QUADS);

    glColor3f(0.6f, 0.6f, 0.6f); // Lighter top

    glVertex2f(-15.0f, -6.0f);

    glVertex2f(15.0f, -6.0f);

    glColor3f(0.4f, 0.4f, 0.4f); // Darker bottom

    glVertex2f(15.0f, -6.9);

    glVertex2f(-15.0f, -6.9f);

    glEnd();

}

void drawGarden()

{

    // garden1

    glColor3ub(100, 200, 100);

    glBegin(GL_POLYGON);

    glVertex2f(-15, 2);

    glVertex2f(-12.2, 2);

    glColor3ub(1, 255, 1);

    glVertex2f(-12.2, 1);

    glVertex2f(-15, 1);

    glEnd();

    // garden2

    glColor3ub(100, 200, 100);

    glBegin(GL_POLYGON);

    glVertex2f(-2.6, 2);

    glVertex2f(1.7, 2);

    glColor3ub(1, 255, 1);

    glVertex2f(1.7, 1);

    glVertex2f(-2.6, 1);

    glEnd();

    // garden3

    glColor3ub(100, 200, 100);

    glBegin(GL_POLYGON);

    glVertex2f(11.5, 2);

    glVertex2f(15, 2);

    glColor3ub(1, 255, 1);

    glVertex2f(15, 1);

    glVertex2f(11.5, 1);

    glEnd();

}

// Tree inside garden3

void drawTreeInGarden1()

{

    // 1st

    glColor3ub(150, 75, 0);

    glBegin(GL_POLYGON);

    glVertex2f(12.9, 3.5);

    glVertex2f(12, 6);

    glVertex2f(12.1, 6);

    glVertex2f(12.9, 3.7);

    glEnd();

    drawCircle1(11.7f, 5.5f, 0.6f, 0, 150, 0); // Draw green circle

    drawCircle1(11.9f, 4.6f, 0.6f, 0, 150, 0); // Draw green circle

    drawCircle1(12.3f, 5.2f, 0.6f, 0, 150, 0); // Draw green circle

    // 2nd Branch

    glColor3ub(150, 75, 0);

    glBegin(GL_POLYGON);

    glVertex2f(12.9, 6);

    glVertex2f(13.2, 6);

    glVertex2f(13.2, 2);

    glVertex2f(12.9, 2);

    glEnd();

    drawCircle1(12.5f, 7.0f, 0.8f, 0, 190, 0); // Draw green circle

    drawCircle1(13.5f, 7.0f, 0.8f, 0, 190, 0); // Draw green circle

    drawCircle1(12.0f, 5.8f, 0.7f, 0, 190, 0); // Draw green circle

    drawCircle1(13.0f, 5.9f, 0.7f, 0, 190, 0); // Draw green circle

    drawCircle1(14.0f, 5.8f, 0.7f, 0, 190, 0); // Draw green circle

    // 3rd

    glColor3ub(150, 75, 0);

    glBegin(GL_POLYGON);

    glVertex2f(13.2, 4);

    glVertex2f(14.70, 5.8);

    glVertex2f(14.80, 5.8);

    glVertex2f(13.2, 3.8);

    glEnd();

    drawCircle1(14.5f, 5.6f, 0.6f, 0, 150, 0); // Draw green circle

    drawCircle1(13.9f, 5.3f, 0.6f, 0, 150, 0); // Draw green circle

    drawCircle1(14.5f, 4.7f, 0.6f, 0, 150, 0); // Draw green circle

}

// tree in garden 1 and 3

void drawTreeAt1(float offsetX, float offsetY)

{

    // 1st Branch (left-leaning)

    glColor3ub(150, 75, 0);

    glBegin(GL_POLYGON);

    glVertex2f(offsetX + 0.0f, offsetY + 0.0f);

    glVertex2f(offsetX - 0.9f, offsetY + 2.5f);

    glVertex2f(offsetX - 0.8f, offsetY + 2.5f);

    glVertex2f(offsetX + 0.0f, offsetY + 0.2f);

    glEnd();

    drawCircle1(offsetX - 1.2f, offsetY + 2.0f, 0.6f, 0, 150, 0);

    drawCircle1(offsetX - 1.0f, offsetY + 1.1f, 0.6f, 0, 150, 0);

    drawCircle1(offsetX - 0.6f, offsetY + 1.7f, 0.6f, 0, 150, 0);

    // 2nd Branch (center trunk)

    glColor3ub(150, 75, 0);

    glBegin(GL_POLYGON);

    glVertex2f(offsetX + 0.0f, offsetY + 2.5f);

    glVertex2f(offsetX + 0.3f, offsetY + 2.5f);

    glVertex2f(offsetX + 0.3f, offsetY - 1.5f);

    glVertex2f(offsetX + 0.0f, offsetY - 1.5f);

    glEnd();

    drawCircle1(offsetX - 0.4f, offsetY + 3.5f, 0.8f, 0, 190, 0);

    drawCircle1(offsetX + 0.6f, offsetY + 3.5f, 0.8f, 0, 190, 0);

    drawCircle1(offsetX - 1.0f, offsetY + 2.3f, 0.7f, 0, 190, 0);

    drawCircle1(offsetX + 0.0f, offsetY + 2.4f, 0.7f, 0, 190, 0);

    drawCircle1(offsetX + 1.0f, offsetY + 2.3f, 0.7f, 0, 190, 0);

    // 3rd Branch (right-leaning)

    glColor3ub(150, 75, 0);

    glBegin(GL_POLYGON);

    glVertex2f(offsetX + 0.3f, offsetY + 1.0f);

    glVertex2f(offsetX + 1.8f, offsetY + 2.8f);

    glVertex2f(offsetX + 1.9f, offsetY + 2.8f);

    glVertex2f(offsetX + 0.3f, offsetY + 0.8f);

    glEnd();

    drawCircle1(offsetX + 1.6f, offsetY + 2.6f, 0.6f, 0, 150, 0);

    drawCircle1(offsetX + 1.0f, offsetY + 2.3f, 0.6f, 0, 150, 0);

    drawCircle1(offsetX + 1.6f, offsetY + 1.7f, 0.6f, 0, 150, 0);

}

void drawShurb1()

{

    // Shrub circles (bushes)

    float radius = 0.6f;

    // Shrubs in garden1 (-15 to -12.2, y = 1 to 2)

    drawCircle1(-14.5f, 2.2f, radius, 0, 180, 0);

    drawCircle1(-14.1f, 2.2f, radius, 0, 180, 0);

    drawCircle1(-14.3f, 2.7f, radius, 0, 180, 0);

    drawCircle1(-13.5f, 2.2f, radius, 0, 170, 0);

    drawCircle1(-13.1f, 2.2f, radius, 0, 170, 0);

    drawCircle1(-13.3f, 2.7f, radius, 0, 170, 0);

    // Shrubs in garden2 (-2.6 to 1.7, y = 1 to 2)

    drawCircle1(-2.0f, 2.2f, radius, 0, 180, 0);

    drawCircle1(-1.6f, 2.2f, radius, 0, 180, 0);

    drawCircle1(-1.8f, 2.7f, radius, 0, 180, 0);

    // Middle shrub (new one)

    drawCircle1(-0.5f, 2.2f, radius, 0, 175, 0);

    drawCircle1(-0.1f, 2.2f, radius, 0, 175, 0);

    drawCircle1(-0.3f, 2.7f, radius, 0, 175, 0);

    drawCircle1(0.6f, 2.2f, radius, 0, 180, 0);

    drawCircle1(1.1f, 2.2f, radius, 0, 180, 0);

    drawCircle1(0.9f, 2.8f, radius, 0, 180, 0);

    //-----------------------------------------------

    //-------------------------------------------------

    // Left bush(12 to 15)

    drawCircle1(12.0f, 2.2f, radius, 0, 180, 0);

    drawCircle1(12.4f, 2.2f, radius, 0, 180, 0);

    drawCircle1(12.2f, 2.7f, radius, 0, 180, 0);

    // Middle bush

    drawCircle1(13.0f, 2.2f, radius, 0, 170, 0);

    drawCircle1(13.4f, 2.2f, radius, 0, 170, 0);

    drawCircle1(13.2f, 2.7f, radius, 0, 170, 0);

    // Right bush

    drawCircle1(14.0f, 2.2f, radius, 0, 160, 0);

    drawCircle1(14.4f, 2.2f, radius, 0, 160, 0);

    drawCircle1(14.2f, 2.7f, radius, 0, 160, 0);


}

// obj8

void drawBuilding(float x, float y, float scale,

                  GLubyte wallR, GLubyte wallG, GLubyte wallB,

                  GLubyte windowR, GLubyte windowG, GLubyte windowB,

                  GLubyte doorR, GLubyte doorG, GLubyte doorB)




{

    glPushMatrix();

    glTranslatef(x, y, 0.0f);  // Move to desired position

    glScalef(scale, scale, 1); // Scale down or up

    // --- Wall ---

    glColor3ub(wallR, wallG, wallB);

    glBegin(GL_POLYGON);

    glVertex2f(-10, 10);

    glVertex2f(-5, 10);

    glVertex2f(-5, 11.5);

    glVertex2f(-10, 11.5);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(-13, 10);

    glVertex2f(-2, 10);

    glVertex2f(-2, -6);

    glVertex2f(-13, -6);

    glEnd();

    glColor3ub(144, 238, 144); // Trim Top

    glBegin(GL_POLYGON);

    glVertex2f(-13.5, 10);

    glVertex2f(-1.5, 10);

    glVertex2f(-1.5, 10.5);

    glVertex2f(-13.5, 10.5);

    glEnd();

    glBegin(GL_POLYGON); // Trim Bottom

    glVertex2f(-13.5, -6);

    glVertex2f(-1.5, -6);

    glVertex2f(-1.3, -6.5);

    glVertex2f(-13.7, -6.5);

    glEnd();

    // --- Windows ---

    glColor3ub(windowR, windowG, windowB);

    float wx[][4] = {

        {-12, 9, -10, 7}, {-5, 9, -3, 7}, {-8.5, 9, -6.5, 7}, {-12, 6.3, -10, 4.3}, {-5, 6.3, -3, 4.3}, {-8.5, 6.3, -6.5, 4.3}, {-12, 3.5, -10, 1.5}, {-5, 3.5, -3, 1.5}, {-8.5, 3.5, -6.5, 1.5}, {-12, 0.7, -10, -1.2}, {-5, 0.7, -3, -1.2}, {-8.5, 0.7, -6.5, -1.2}};

    for (int i = 0; i < 12; i++)

    {

        glBegin(GL_POLYGON);

        glVertex2f(wx[i][0], wx[i][1]);

        glVertex2f(wx[i][2], wx[i][1]);

        glVertex2f(wx[i][2], wx[i][3]);

        glVertex2f(wx[i][0], wx[i][3]);

        glEnd();

    }

    // --- Door ---

    glColor3ub(143, 143, 142);

    glBegin(GL_POLYGON);

    glVertex2f(-10, -3);

    glVertex2f(-5, -3);

    glVertex2f(-5, -6);

    glVertex2f(-10, -6);

    glEnd();

    glColor3ub(doorR, doorG, doorB);

    glBegin(GL_POLYGON);

    glVertex2f(-9.5, -3);

    glVertex2f(-5.5, -3);

    glVertex2f(-5.5, -6);

    glVertex2f(-9.5, -6);

    glEnd();

    glColor3ub(144, 238, 144);

    glBegin(GL_POLYGON); // Door Trim

    glVertex2f(-11, -3);

    glVertex2f(-4, -3);

    glVertex2f(-4, -2.3);

    glVertex2f(-11, -2.3);

    glEnd();

    // --- Door Divider Line ---

    glColor3ub(0, 0, 0);

    glBegin(GL_LINES);

    glVertex2f(-7.5, -3);

    glVertex2f(-7.5, -6);

    glEnd();

    glPopMatrix();

}

// obj9

void drawMoon1()

{

    drawCircle1(13.5f, 11.5f, 1.5f, 230, 230, 230);

    drawCircle1(12.9f, 11.2f, 1.5f, 13, 13, 51);

}

// obj10

void drawStars1()

{

    glColor3f(1.0f, 1.0f, 1.0f); // White stars

    glPointSize(2.0f);

    glBegin(GL_POINTS);

    // Random-like coordinates for stars

    glVertex2f(-12.0f, 12.0f);

    glVertex2f(-10.0f, 10.0f);

    glVertex2f(-8.0f, 13.0f);

    glVertex2f(-6.0f, 11.5f);

    glVertex2f(-4.0f, 14.0f);

    glVertex2f(-2.0f, 10.0f);

    glVertex2f(0.0f, 13.0f);

    glVertex2f(2.0f, 12.5f);

    glVertex2f(4.0f, 11.0f);

    glVertex2f(6.0f, 14.0f);

    glVertex2f(8.0f, 12.0f);

    glVertex2f(10.0f, 13.5f);

    glVertex2f(12.0f, 11.0f);

    glEnd();

}

// obj11

// Draw rain lines

void drawRain1()

{

    glColor3f(0.5, 0.5, 1.0); // Light blue

    glBegin(GL_LINES);

    for (int i = 0; i < numDrops1; i++)

    {

        glVertex2f(rainX1[i], rainY1[i]);

        glVertex2f(rainX1[i], rainY1[i] - 0.7);

    }

    glEnd();

}

// Initialize rain positions

void initRain1()

{

    for (int i = 0; i < numDrops1; i++)

    {

        rainX1[i] = rand() % 50- 20; // X range: -20 to 20

        rainY1[i] = rand() % 30-15;      // Y range: 0 to 30

    }

}

// obj12


void drawFlower(float x, float y)
{
    // Petals (4 circles)
    drawCircle1(x - 0.2f, y, 0.15f, 255, 0, 0);  // Left petal
    drawCircle1(x + 0.2f, y, 0.15f, 255, 0, 0);  // Right petal
    drawCircle1(x, y + 0.2f, 0.15f, 255, 0, 0);  // Top petal
    drawCircle1(x, y - 0.2f, 0.15f, 255, 0, 0);  // Bottom petal

    // Center (yellow circle)
    drawCircle1(x, y, 0.12f, 255, 255, 0);
}




void drawLamp1(float x, float y, float s)

{

    glColor3ub(255, 255, 153); // Light yellow color

    glBegin(GL_POLYGON);

    glVertex2f(x + s * 1, y + s * 5);

    glVertex2f(x + s * 1.5, y + s * 2);

    glVertex2f(x + s * 3.5, y + s * 2);

    glVertex2f(x + s * 4, y + s * 5);

    glEnd();

    glColor3ub(128, 128, 128); // Grey color

    glBegin(GL_POLYGON);

    glVertex2f(x + s * 0.8, y + s * 4.8);

    glVertex2f(x + s * 1.6, y + s * 5.6);

    glVertex2f(x + s * 3.4, y + s * 5.6);

    glVertex2f(x + s * 4.2, y + s * 4.8);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(x + s * 1.6, y + s * 5.6);

    glVertex2f(x + s * 3.4, y + s * 5.6);

    glVertex2f(x + s * 3.4, y + s * 6);

    glVertex2f(x + s * 1.6, y + s * 6);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(x + s * 3.6, y + s * 6);

    glVertex2f(x + s * 1.4, y + s * 6);

    glVertex2f(x + s * 1.8, y + s * 6.2);

    glVertex2f(x + s * 3.2, y + s * 6.2);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(x + s * 1.8, y + s * 6.2);

    glVertex2f(x + s * 3.2, y + s * 6.2);

    glVertex2f(x + s * 3, y + s * 6.4);

    glVertex2f(x + s * 2, y + s * 6.4);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(x + s * 1.5, y + s * 2);

    glVertex2f(x + s * 3.5, y + s * 2);

    glVertex2f(x + s * 2.8, y + s * 1.6);

    glVertex2f(x + s * 2.2, y + s * 1.6);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(x + s * 2.8, y + s * 1.6);

    glVertex2f(x + s * 2.2, y + s * 1.6);

    glVertex2f(x + s * 2.2, y + s * -10);

    glVertex2f(x + s * 2.8, y + s * -10);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(x + s * 2.2, y + s * -10);

    glVertex2f(x + s * 2.8, y + s * -10);

    glVertex2f(x + s * 3.5, y + s * -10.5);

    glVertex2f(x + s * 1.5, y + s * -10.5);

    glEnd();

}

// obj13

void drawBench1(float x, float y, float s)

{

    // === SUPPORT ===

    glColor3ub(143, 143, 142);

    glBegin(GL_POLYGON);

    glVertex2f(x + s * -1.8, y + s * 3.2);

    glVertex2f(x + s * -1.8, y + s * -1.9);

    glVertex2f(x + s * -1.4, y + s * -1.9);

    glVertex2f(x + s * -1.4, y + s * 3.2);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(x + s * -2.0, y + s * -2.1);

    glVertex2f(x + s * -1.8, y + s * -1.9);

    glVertex2f(x + s * -1.4, y + s * -1.9);

    glVertex2f(x + s * -1.2, y + s * -2.1);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(x + s * 1.8, y + s * 3.2);

    glVertex2f(x + s * 1.8, y + s * -1.9);

    glVertex2f(x + s * 1.4, y + s * -1.9);

    glVertex2f(x + s * 1.4, y + s * 3.2);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(x + s * 2.0, y + s * -2.1);

    glVertex2f(x + s * 1.8, y + s * -1.9);

    glVertex2f(x + s * 1.4, y + s * -1.9);

    glVertex2f(x + s * 1.2, y + s * -2.1);

    glEnd();

    // === MID RECTANGLES ===

    glColor3ub(186, 140, 99);

    float midY[3][2] = {{3.0, 2.5}, {2.2, 1.7}, {1.4, 0.9}};

    for (int i = 0; i < 3; i++)

    {

        glBegin(GL_POLYGON);

        glVertex2f(x + s * -3, y + s * midY[i][0]);

        glVertex2f(x + s * 3, y + s * midY[i][0]);

        glVertex2f(x + s * 3, y + s * midY[i][1]);

        glVertex2f(x + s * -3, y + s * midY[i][1]);

        glEnd();

    }

    // === SIT AREA ===

    glColor3ub(186, 140, 99);

    glBegin(GL_POLYGON);

    glVertex2f(x + s * -3, y + s * 0.4);

    glVertex2f(x + s * 3, y + s * 0.4);

    glVertex2f(x + s * 3.6, y + s * -0.6);

    glVertex2f(x + s * -3.6, y + s * -0.6);

    glEnd();

    glColor3ub(161, 102, 47);

    glBegin(GL_POLYGON);

    glVertex2f(x + s * -3.6, y + s * -0.6);

    glVertex2f(x + s * -3.6, y + s * -1.0);

    glVertex2f(x + s * 3.6, y + s * -1.0);

    glVertex2f(x + s * 3.6, y + s * -0.6);

    glEnd();

    // === SUPPORT2 ===

    glColor3ub(143, 143, 142);

    glBegin(GL_POLYGON);

    glVertex2f(x + s * -3.2, y + s * -1.0);

    glVertex2f(x + s * -3.2, y + s * -2.3);

    glVertex2f(x + s * -2.8, y + s * -2.3);

    glVertex2f(x + s * -2.8, y + s * -1.0);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(x + s * -3.2, y + s * -2.3);

    glVertex2f(x + s * -2.8, y + s * -2.3);

    glVertex2f(x + s * -2.6, y + s * -2.5);

    glVertex2f(x + s * -3.4, y + s * -2.5);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(x + s * 2.6, y + s * -1.0);

    glVertex2f(x + s * 2.6, y + s * -2.3);

    glVertex2f(x + s * 3.0, y + s * -2.3);

    glVertex2f(x + s * 3.0, y + s * -1.0);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(x + s * 2.6, y + s * -2.3);

    glVertex2f(x + s * 3.0, y + s * -2.3);

    glVertex2f(x + s * 3.2, y + s * -2.5);

    glVertex2f(x + s * 2.4, y + s * -2.5);

    glEnd();

}



// obj14
void drawSmallFlower(float x, float y)
{
    float petalRadius = 0.08f; // smaller petals
    // Petals (soft pink)
    drawCircle1(x - 0.1f, y, petalRadius, 255, 182, 193);  // Left
    drawCircle1(x + 0.1f, y, petalRadius, 255, 182, 193);  // Right
    drawCircle1(x, y + 0.1f, petalRadius, 255, 182, 193);  // Top
    drawCircle1(x, y - 0.1f, petalRadius, 255, 182, 193);  // Bottom

    // Center (yellow)
    drawCircle1(x, y, 0.06f, 255, 255, 100);
}

// obj15

void drawBus()

{

    glPushMatrix();

    glTranslatef(busX, 0.0f, 0.0f); // Move the bus along X-axis

    glColor3ub(255, 255, 255);

    glBegin(GL_POLYGON); // Door Trim

    glVertex2f(-29.3, 2.7);

    glVertex2f(-29, 3);

    glVertex2f(-18, 3);

    glVertex2f(-17.5, 2.5);

    glVertex2f(-16.5, 0);

    glVertex2f(-16.5, -1.9);

    glVertex2f(-29, -1.9);

    glVertex2f(-29.3, -1.7);

    glEnd();

    // door

    glColor3ub(0, 0, 0);

    glBegin(GL_POLYGON); // Door Trim

    glVertex2f(-21.6, 2.5);

    glVertex2f(-20, 2.5);

    glVertex2f(-20, -1.9);

    glVertex2f(-21.6, -1.9);

    glEnd();

    glColor3ub(0, 0, 0);

    glBegin(GL_POLYGON); // Door Trim

    glVertex2f(-19.4, 2.5);

    glVertex2f(-17.5, 2.5);

    glVertex2f(-16.7, 0.5);

    glVertex2f(-19.4, 0.5);

    glEnd();

    glColor3ub(0, 0, 0);

    glBegin(GL_POLYGON); // Door Trim

    glVertex2f(-22, 0.5);

    glVertex2f(-22, 2.5);

    glVertex2f(-28.6, 2.5);

    glVertex2f(-28.6, 0.5);

    glEnd();

    // wheel

    drawCircle1(-26.1f, -1.8f, 0.7f, 3, 3, 3);

    drawCircle1(-18.8f, -1.8f, 0.7f, 3, 3, 3);

    glPopMatrix();

}

// obj16

void drawTruck()

{

    glPushMatrix();

    glTranslatef(truckX, 0.0f, 0.0f); // Move the bus along X-axis

    glColor3ub(0, 0, 0);

    glBegin(GL_POLYGON); // Door Trim

    glVertex2f(13.8, -5);

    glVertex2f(-5.4, -5);

    glVertex2f(-5.4, -4.6);

    glVertex2f(13.8, -4.6);

    glEnd();

    glColor3ub(255, 0, 0);

    glBegin(GL_POLYGON);

    glVertex2f(-5.4, -4.6);

    glVertex2f(-5.4, -2.0);

    glVertex2f(-3.5, -1);

    glVertex2f(-2.5, 1);

    glVertex2f(1, 1);

    glVertex2f(1, -4.6);

    glEnd();

    glColor3ub(173, 216, 230);

    glBegin(GL_POLYGON);

    glVertex2f(-3.5, -1);

    glVertex2f(0.5, -1);

    glVertex2f(0.5, 1);

    glVertex2f(-2.5, 1);

    glEnd();

    // container
   // glColor3ub(175, 238, 238);

    glColor3ub(245, 245, 200);

    glBegin(GL_POLYGON); // Door Trim

    glVertex2f(1, -4.6);

    glVertex2f(1, 3);

    glVertex2f(13.8, 3);

    glVertex2f(13.8, -4.6);

    glEnd();

    // wheel

    drawCircle1(-2.0f, -4.6f, 0.9f, 3, 3, 3);

    drawCircle1(9.2f, -4.6f, 0.9f, 3, 3, 3);

    glPopMatrix();

}

/*void drawFild()

{

    glBegin(GL_QUADS);

    // Top (closer to viewer) – lighter green

    glColor3ub(0, 180, 20); // Light green

    glVertex2f(-15.0f, -6.0f);

    glVertex2f(15.0f, -6.0f);

    // Bottom (farther) – darker green

    glColor3ub(255, 255, 100); // Dark green

    glVertex2f(15.0f, -15.0f);

    glVertex2f(-15.0f, -15.0f);

    glEnd();

}
*/
// obj17

void drawCloudes1(float offsetX, float offsetY)

{

    drawCircle1(cloudX1 + offsetX - 0.4f, offsetY + 3.5f, 0.8f, 255, 255, 255);

    drawCircle1(cloudX1 + offsetX + 0.6f, offsetY + 3.5f, 0.8f, 255, 255, 255);

    drawCircle1(cloudX1 + offsetX - 1.0f, offsetY + 2.3f, 0.7f, 255, 255, 255);

    drawCircle1(cloudX1 + offsetX + 0.0f, offsetY + 2.4f, 0.7f, 255, 255, 255);

    drawCircle1(cloudX1 + offsetX + 1.0f, offsetY + 2.3f, 0.7f, 255, 255, 255);

}

void drawFild1()

{

    glBegin(GL_QUADS);

    // Top (closer to viewer) – lighter green

    glColor3ub(0, 180, 20); // Light green

    glVertex2f(-15.0f, -6.0f);

    glVertex2f(15.0f, -6.0f);

    // Bottom (farther) – darker green

    glColor3ub(255, 255, 100); // Dark green

    glVertex2f(15.0f, -15.0f);

    glVertex2f(-15.0f, -15.0f);
     //  Scatter small pink flowers
         drawSmallFlower(-12, -8);
         drawSmallFlower(-10, -9);
        drawSmallFlower(-8, -7);
       drawSmallFlower(-6, -10);
          drawSmallFlower(-2, -8.5);
          drawSmallFlower(0, -9);
        drawSmallFlower(3, -8);
        drawSmallFlower(6, -10);
         drawSmallFlower(9, -8.5);
        drawSmallFlower(12, -9);

    drawFlower(-10, -8);
           drawFlower(-5, -9);
         drawFlower(0, -7);
         drawFlower(5, -10);
        drawFlower(12, -8);
        drawFlower(-8, -10);
         drawFlower(9, -7);
         drawFlower(11.5, -13);
        drawFlower(11, -8);


    glEnd();

}

// obj17




// obj17

void drawCars()

{

    glPushMatrix();

    glTranslatef(carX, 0.0f, 0.0f); // Move the car based on carX

    // roof
     glColor3ub(0, 0, 0);


    glBegin(GL_POLYGON);

    glVertex2f(-13, 0.5);

    glVertex2f(-8, 0.5);

    glVertex2f(-9, 2.1);

    glVertex2f(-12.2, 2.1);

    glEnd();

    // body

    glColor3ub(150, 75, 0);

    glBegin(GL_POLYGON);

    glVertex2f(-14, 0.5);

    glVertex2f(-6, 0.5);

    glVertex2f(-6, -1);

    glVertex2f(-14, -1);

    glEnd();

    // wheel

    drawCircle1(-12.0f, -1.0f, 0.7f, 3, 3, 3);

    drawCircle1(-8.0f, -1.0f, 0.7f, 3, 3, 3);

    glPopMatrix();

}

// obj18


// Day night functions

void setDay1()

{

    drawSky1(0.7f, 0.9f, 1.0f);                     // Light blue sky

    drawSun1(12.0f, 12.0f, 1.0f, 1.0f, 1.0f, 0.0f); // Yellow sun

}

// night functions

void setNight1()

{

    drawSky1(0.05f, 0.05f, 0.2f); // Dark blue sky

    drawMoon1();                  // White moon

    drawStars1();                 // Show stars at night

}

// trasnlation house and color change

void drawTranslated(float x, float y,

                    GLubyte r1, GLubyte g1, GLubyte b1, // primary wall color

                    GLubyte r2, GLubyte g2, GLubyte b2, // roof color

                    GLubyte r3, GLubyte g3, GLubyte b3, // border/door color

                    GLubyte r4, GLubyte g4, GLubyte b4) // garage/road color

{

    glPushMatrix();

    glTranslatef(x, y, 0.0f); // Apply translation

    // ✅ Call the function without redeclaring types

    drawhouse(r1, g1, b1,

              r2, g2, b2,

              r3, g3, b3,

              r4, g4, b4);

    glPopMatrix();

}

// ------------------------- Scene Logic -------------------------

void update2(int value)

{

    float speed = 0.1f;

    // bus movement

    busX += 0.3f; // Speed of the bus

    if (busX > 50.0f) // Reset position when it goes off screen

        busX = -50.0f;

    // truck movement

    truckX -= 0.2f; // Move left

    if (truckX < -50.0f) // Reset when off-screen on the left

        truckX = 50.0f;  // Reappear from right

    // 🚗 Car movement

    carX += 0.2f;

    if (carX > 30.0f)

        carX = -30.0f;

    // cloudes movement

    cloudX1 += 0.02f; // Speed of cloud movement

    if (cloudX1 > 18.0f) // Reset when cloud goes off screen

        cloudX1 = -15.0f;

    glutPostRedisplay();

    glutTimerFunc(16, update2, 0);

}

// Update rain positions

void updateRain1(int value)

{

    if (isRaining1)

    {

        for (int i = 0; i < numDrops1; i++)

        {

            rainY1[i] -= 0.5;

            if (rainY1[i] < -15)

            {

                rainY1[i] = 25; // Reset to top

                rainX1[i] = rand() % 40 - 20;

            }

        }

        glutPostRedisplay();              // Redraw

        glutTimerFunc(30, updateRain1, 0); // Repeat

    }

}


//----------------mouse functions---------------

// Mouse input
/*
void mouse(int button, int state, int x, int y)

{

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)

    {

        isRaining1 = true;

        glutTimerFunc(0, updateRain1, 0); // Start animation



    }

}
*/
//-------------------------keyBoard Funtions---------------
/*
void keyboard2(unsigned char key, int x, int y)

{

    switch (key)

    {

    case 'x':

    case 'X':

        isDay1 = true;

        glutPostRedisplay();

        break;

    case 'g':

    case 'G':

        isDay1 = false;

        glutPostRedisplay();

        break;

    case 'k':

    case 'K':

        isRaining1 = true;

        glutTimerFunc(0, updateRain1, 0);

        break;


    }

}
*/

//-------------chanege senario------------

void City()

{

    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    if (isDay1)

    {

        setDay1();

    }

    else

    {

        setNight1();

    }



    drawCloudes1(1.0f, 10.0f);

    drawCloudes1(5.0f, 8.0f);

    drawCloudes1(9.0f, 9.0f);

    drawCloudes1(-12.0f, 9.0f);

    drawCloudes1(-8.0, 10.0f);

    drawCloudes1(-16.0, 10.0f);

    drawCloudes1(18.0, 11.0f);

    drawCloudes1(20.0, 10.0f);

    drawCloudes1(12.5, 11.0f);

    // another smaller building

    drawBuilding(-5, 5, 0.6,

                65, 105, 225,// Wall

                 200, 216, 230,  // Windows

                 169, 169, 169); // Door

    // small building  with custom colors

    drawBuilding(-2, 5, 0.5,     // Position (x,y), Scale

                 200, 180, 100,  // Wall Color

                 135, 206, 250,  // Window Color

                 100, 100, 255); // Door Color

    // Draw another smaller blue building(frnt1)

   drawBuilding(14, 5, 0.6,

             65, 105, 225,   // Wall (Royal Blue)

             173, 216, 230,  // Windows (Light Blue)

             245, 245, 245); // Door (Off-White)
   //pink building
   drawBuilding(15.5, 5, 0.4,

                 244, 270, 193,  // Wall

                 255, 255, 255,  // Windows

                 169, 169, 169); // Door
  //pink building
    drawBuilding(-9.5, 5, 0.4,

                 250, 282, 193,  // Wall

                 170, 216, 200,  // Windows

                 169, 169, 169); // Door


    // Draw another smaller building at (frnt2)

    drawBuilding(10, 5, 0.5,
                // 178, 34, 34,
                 110, 140, 120,  // Wall

                 173, 200, 230,  // Windows

                 169, 169, 122); // Door











    // Draw another red building(middle frnt)

   drawBuilding(5.5, 4.5, 0.7,

                 178, 34, 34,   // Wall

                 173, 216, 230,  // Windows

                 169, 169, 122); // Door



    // Tree in Garden1 (fits inside -15 to -12.2, 1 to 2)

    drawTreeAt1(-13.0f, 3.0f);

    //drawCircle(2.0f, 3.5f, 0.7f, 0, 150, 0);

    drawTreeAt1(5.5f, 3.0f);

    //drawCircle(1.6f, 2.9f, 0.7f, 0, 200, 50);

    //drawCircle(2.0f, 2.5f, 0.7f, 0, 150, 0);

    drawRoad1();

    // Draw house with specific colors

    /*drawhouse(

        244, 164, 96,  // Wall color (brown)c1
   150, 150, 150); // garage/road
*/
    drawTreeAt1(-4.8f, 3.0f);

    drawGarden();

    drawTreeInGarden1();

    drawFild1();

     //drawTreeAt(-0.5f, 3.1f);

    drawShurb1();

    drawBench1(-13.5f, 1.8f, 0.4f); // At origin, scaled down 30%

    drawBench1(-0.1f, 1.8f, 0.4f);  // Translated and scaled

    drawBench1(13.5f, 1.8f, 0.4f);  // Translated and scaled

    drawLamp1(10.5f, 4.3f, 0.4f);  // Position at (12, 3.3) with 30% size

    drawLamp1(-3.5f, 4.3f, 0.4f);  // Position at (-1, 3.3) with 30% size

    drawLamp1(3.0f, 4.3f, 0.4f); // Position at (-14, 3.3) with 30% size

    drawLamp1(-15.0f, 4.3f, 0.4f); // Position at (-14, 3.3) with 30% size

    drawFild1();

    drawCars();

    drawBus();

    drawTruck();

    // Default full-size track

    if (isRaining1)

        drawRain1();

    glutSwapBuffers();

}

float _angle1 = 0.0f; // Angle for blade rotation
float boat1PosX = -0.050f; // Boat 1: Left to right
float boat2PosX = 5.0f; // Boat 2: Right to left
float boatSpeed = 0.015f;
bool boatMoving = true; // Flag to control boat animation
bool isNight2 = false;
bool isRaining2 = false;
const int MAX_RAIN2 = 50;
float rainX2[MAX_RAIN2];
float rainY2[MAX_RAIN2];
bool rainPaused = false;
float cloud_x = -20.0f; // Initial cloud position
float cloud_speed = 0.007f; // Speed of cloud movement
float birdX = -15.0f;
float birdWingY = -3.0f;
float birdDirection = 1.0f;

/*
    void handleKeypressVillage(unsigned char key, int x, int y) {


    if (key == 'h' || key == 'H') {
        isNight2=true;
    }
    else if (key == 'e' || key == 'E') {
        isNight2=false;
    }
    else if (key == 'z' || key == 'z') {
         rainPaused = false;
    }
    else if (key == 'u' || key == 'U') {
        rainPaused = true;
    }

    else if (key == '3') sceneVillage = false; // Show scene objects
    else if (key == '4') sceneVillage= true;
    else if(key='5') sceneVillage= true;
}
*/
void circle2(float cx, float cy, float radius, int r, int g, int b)
{
float pi = 3.1416;

glColor3ub(r, g, b);
glBegin(GL_POLYGON);

for (int i = 0; i < 200; i++)
{
float A2 = (i * 2 * pi) / 200;
float x2 = cx + radius * cos(A2);
float y2 = cy + radius * sin(A2);
glVertex2f(x2, y2);
}
glEnd();
}
void drawClouds2()
{
int r, g, b;

if (isRaining2)
{
r = 123;
g = 134;
b = 143;
}
else
{
r = g = b = 255; // White clouds
}
// Cloud group 3
circle2(cloud_x - 6.0f, 13.0f, 1.2f, r, g, b);
circle2(cloud_x - 7.5f, 13.5f, 1.2f, r, g, b);
circle2(cloud_x - 9.0f, 13.0f, 1.2f, r, g, b);

// Cloud group 1 (Left side)
circle2(cloud_x - 10.0f, 8.0f, 1.0f, r, g, b);
circle2(cloud_x - 11.1f, 8.85f, 1.0f, r, g, b);
circle2(cloud_x - 11.9f, 8.0f, 1.0f, r, g, b);
}
void drawShurb2()
{
float radius = 0.6f;

// Shrubs in garden1 (in front of hut1)
// Left side bushes
circle2(-12.0f, 0.2f, radius, 0, 190, 0);
circle2(-12.4f, 0.2f, radius, 0, 180, 0);
circle2(-12.2f, 0.7f, radius, 0, 180, 0);

// Middle bush
circle2(-13.0f, 0.2f, radius, 0, 170, 0);
circle2(-13.4f, 0.2f, radius, 0, 170, 0);
circle2(-13.2f, 0.7f, radius, 0, 170, 0);

// Right bush
circle2(-14.0f, 0.2f, radius, 0, 160, 0);
circle2(-14.4f, 0.2f, radius, 0, 160, 0);
circle2(-14.2f, 0.7f, radius, 0, 160, 0);

// Scence middle bushes
circle2(-6.0f, 0.2f, radius, 0, 180, 0);
circle2(-6.4f, 0.2f, radius, 0, 180, 0);
circle2(-6.2f, 0.7f, radius, 0, 180, 0);

// Middle bush
circle2(-7.0f, 0.2f, radius, 0, 170, 0);
circle2(-7.4f, 0.2f, radius, 0, 170, 0);
circle2(-7.2f, 0.7f, radius, 0, 170, 0);

// Right bush
circle2(-8.0f, 0.2f, radius, 0, 160, 0);
circle2(-8.4f, 0.2f, radius, 0, 160, 0);
circle2(-8.2f, 0.7f, radius, 0, 160, 0);

// Left side bushes
circle2(12.0f, 0.2f, radius, 0, 180, 0);
circle2(12.4f, 0.2f, radius, 0, 180, 0);
circle2(12.2f, 0.7f, radius, 0, 180, 0);

// Middle bush
circle2(13.0f, 0.2f, radius, 0, 170, 0);
circle2(13.4f, 0.2f, radius, 0, 170, 0);
circle2(13.2f, 0.7f, radius, 0, 170, 0);

// Right bush
circle2(14.0f, 0.2f, radius, 0, 160, 0);
circle2(14.4f, 0.2f, radius, 0, 160, 0);
circle2(14.2f, 0.7f, radius, 0, 160, 0);
}
// OBJ006

void drawField()
{
glColor3ub(124, 110, 0);
glBegin(GL_QUADS);
glVertex2f(-15, 1.5);
glVertex2f(15.0f, 1.5f);

glColor3ub(0, 150, 0);
glVertex2f(15.0f, -4.8);
glVertex2f(-15, -4.8);

glEnd();
}

// OBJ007

void drawRiver()
{

glColor3ub(0, 0, 255);
glBegin(GL_QUADS);
glVertex2f(15, -7.0);
glVertex2f(-15, -7.0);
glVertex2f(-15, -15);
glVertex2f(15, -15);

glEnd();
}

// OBJ008
void drawBoat(float tx, float ty)
{
glPushMatrix();
glTranslatef(tx, ty, 0.0f); // Apply translation
glScalef(0.5, 0.5, 1.0f); // Scale down

// boat right pal
glColor3ub(255, 255, 197);
glBegin(GL_POLYGON);
glVertex2f(-5.8, -6);
glVertex2f(-2, -10);
glVertex2f(-5.8, -10);
glEnd();

// boat left pal
glColor3ub(255, 255, 197);
glBegin(GL_POLYGON);
glVertex2f(-10, -10);
glVertex2f(-5.8, -6);
glVertex2f(-6, -10);
glEnd();

// boat bamboo
glColor3ub(0, 0, 0);
glBegin(GL_POLYGON);
glVertex2f(-6, -12);
glVertex2f(-6, -6);
glVertex2f(-5.8, -6);
glVertex2f(-5.8, -12);
glEnd();

// boat deck
glColor3ub(215, 192, 136);
glBegin(GL_POLYGON);
glVertex2f(-11, -12);
glVertex2f(-8, -10.5);
glVertex2f(-4, -10.5);
glVertex2f(-1, -12);
glEnd();

// boat ground
glColor3ub(0, 0, 0);
glBegin(GL_POLYGON);
glVertex2f(-14, -12);
glVertex2f(4, -12);
glVertex2f(0, -15);
glVertex2f(-10, -15);
glEnd();

glPopMatrix();
}
void drawTree2()
{
// Trunk (scaled up)
glColor3f(0.55f, 0.27f, 0.07f); // Brown
glBegin(GL_POLYGON);
glVertex2f(-0.4f, -7.0f); // wider trunk
glVertex2f(0.4f, -7.0f);
glVertex2f(0.4f, -1.8f); // taller trunk
glVertex2f(-0.4f, -1.8f);
glEnd();

// Circular leaves
circle2(0.0f, -0.5f, 1.0f, 0, 170, 0); // center
circle2(-0.8f, -1.5f, 0.9f, 0, 175, 0); // left
circle2(0.8f, -1.5f, 0.9f, 0, 175, 0); // right
}
void drawTreeTransformed(float tx, float ty, float sx, float sy)
{
glPushMatrix();
glTranslatef(tx, ty, 0.0f);
glScalef(sx, sy, 1.0f);
drawTree2();
glPopMatrix();
}

void translateTree3(float dx, float dy)
{
glPushMatrix();
glTranslatef(dx, dy, 0.0f);
drawTree2();
glPopMatrix();
}
void drawHut() // obj4
{
// roof
glColor3ub(255, 165, 0);
glBegin(GL_QUADS);
glVertex2f(-10, 5);
glVertex2f(-14, 5);
glColor3ub(255, 0, 0);
glVertex2f(-16, 3);
glVertex2f(-12, 3);
glEnd();

// side-wall
glColor3ub(227, 191, 43);
glBegin(GL_POLYGON);
glVertex2f(-10, 5);
glVertex2f(-12, 3);
glVertex2f(-12, 0);
glVertex2f(-8.2, 0);
glVertex2f(-8.2, 3);
glEnd();

// side-wall-bar
glColor3ub(250, 0, 100);
glBegin(GL_POLYGON);
glVertex2f(-10.2, 5);
glVertex2f(-8.2, 3);
glVertex2f(-8, 3);
glVertex2f(-10, 5);
glEnd();

// front-wall
glColor3ub(178, 158, 37);
glBegin(GL_QUADS);
glVertex2f(-16, 3);
glVertex2f(-12, 3);
glVertex2f(-12, 0);
glVertex2f(-16, 0);
glEnd();

// window
glColor3ub(0 , 0, 0);
glBegin(GL_POLYGON);
glVertex2f(-15, 2);
glVertex2f(-13.5, 2);
glVertex2f(-13.5, 1);
glVertex2f(-15, 1);
glEnd();

// floor

glColor3ub(250, 10, 13);
glBegin(GL_POLYGON);
glVertex2f(-16.2, -0.2);
glVertex2f(-16, 0);
glVertex2f(-12, 0);
glVertex2f(-8.2, 0);
glVertex2f(-8, -0.2);
glVertex2f(-12, -0.2);
glEnd();

// door
glColor3ub(210, 210, 210);
glBegin(GL_QUADS);
glVertex2f(-10.8, 2.5);
glVertex2f(-9.3, 2.5);
glVertex2f(-9.3, 0);
glVertex2f(-10.8, 0);
glEnd();
}

void drawRoad2()
{
// long road
glColor3ub(193, 154, 107);
glBegin(GL_POLYGON);
glVertex2f(-15, -1);
glVertex2f(15, -1);
glColor3ub(210, 0, 0);
glVertex2f(15, -3);
glVertex2f(-15, -3);
glEnd();

// short road
glColor3ub(160, 82, 45);
glBegin(GL_POLYGON);
glVertex2f(-0.1, -1);
glVertex2f(2.5, -1);
glColor3ub(210, 0, 0);
glVertex2f(1.5, 0.5);
glVertex2f(0.5, 0.5);
glEnd();
}

void translateHut(float dx, float dy)
{
glPushMatrix();
glTranslatef(dx, dy, 0.0f);
drawHut();
glPopMatrix();
}

void scaleHut(float sx, float sy)
{
glPushMatrix();
glScalef(sx, sy, 1.0f);
drawHut();
glPopMatrix();
}

void flipHut()
{
glPushMatrix();
glScalef(-1.0f, 1.0f, 1.0f); // Mirror across Y-axis
glTranslatef(2.0f, 0.0f, 0.0f); // Adjust position after flip
scaleHut(0.5f, 0.5f);
drawHut();
glPopMatrix();
}

// OBJ012


void drawSimpleBird(float x, float y, float scale, float wingY)
{
glColor3ub(0, 0, 0);

glBegin(GL_LINE_STRIP); // Body curve
glVertex2f(x, y);
glVertex2f(x + scale * 0.4f, y + scale * 0.2f);
glVertex2f(x + scale * 0.8f, y);
glEnd();

glBegin(GL_LINES); // Wings
glVertex2f(x + scale * 0.4f, y + scale * 0.2f);
glVertex2f(x + scale * 0.4f, y + scale * 0.2f + wingY * 0.2f);
glEnd();
}

// OBJ016

void drawBirds()
{
drawSimpleBird(birdX, 12.0f, 0.6f, birdWingY);
drawSimpleBird(birdX - 1.5f, 13.0f, 0.5f, -birdWingY);
drawSimpleBird(birdX - 3.0f, 11.8f, 0.4f, birdWingY);
drawSimpleBird(birdX - 4.2f, 12.6f, 0.3f, -birdWingY);
drawSimpleBird(birdX + 2.0f, 11.0f, 0.5f, birdWingY);
drawSimpleBird(birdX + 4.0f, 13.2f, 0.4f, -birdWingY);
drawSimpleBird(birdX + 6.0f, 12.4f, 0.3f, birdWingY);
drawSimpleBird(birdX - 6.0f, 13.5f, 0.2f, -birdWingY);
drawSimpleBird(birdX - 7.5f, 12.0f, 0.2f, birdWingY);
drawSimpleBird(birdX + 7.0f, 11.8f, 0.25f, -birdWingY);
}

// ANIM006
void updateBirds()
{
birdX += 0.01f; // Always move to the right

birdWingY += 0.01f;
if (birdWingY > 1.0f)
birdWingY = -1.0f;

// Reset bird position to left once it goes off the right edge
if (birdX > 22.0f)
birdX = -20.0f;

glutPostRedisplay(); // Request redraw
}

// OBJ001

void drawSky2()
{
if (isNight2)
{
glColor3f(0.05f, 0.05f, 0.2f); // Dark night sky
}
else
{
glColor3f(0.7f, 0.9f, 1.0f); // Light blue day sky
}

glBegin(GL_QUADS);
glVertex2f(-15.0f, 1.5f);
glVertex2f(15.0f, 1.5f);
glVertex2f(15.0f, 15.0f);
glVertex2f(-15.0f, 15.0f);
glEnd();
}

// OBJ002
void drawSun2()
{
float cx = 12.0f, cy = 12.0f, r = 1.0f;
glColor3f(1.0f, 1.0f, 0.0f);
glBegin(GL_TRIANGLE_FAN);
glVertex2f(cx, cy);
for (int i = 0; i <= 100; i++)
{
float angle = 2 * 3.1416f * i / 100;
glVertex2f(cx + cos(angle) * r, cy + sin(angle) * r);
}
glEnd();
}

// OBJ003
void drawMoon2()
{
float cx = -12.0f, cy = 12.0f, r = 1.0f;

// Main moon circle
glColor3f(1.0f, 1.0f, 0.8f); // Pale yellow
glBegin(GL_TRIANGLE_FAN);
glVertex2f(cx, cy);
for (int i = 0; i <= 100; i++)
{
float angle = 2 * 3.1416f * i / 100;
glVertex2f(cx + cos(angle) * r, cy + sin(angle) * r);
}
glEnd();

// Overlay with dark circle to make it look like a crescent
glColor3f(0.05f, 0.05f, 0.2f); // Sky night color
glBegin(GL_TRIANGLE_FAN);
glVertex2f(cx + 0.4f, cy); // Shift slightly right
for (int i = 0; i <= 100; i++)
{
float angle = 2 * 3.1416f * i / 100;
glVertex2f((cx + 0.4f) + cos(angle) * r, cy + sin(angle) * r);
}
glEnd();
}

void drawStars2()
{
glColor3f(1.0f, 1.0f, 1.0f);
float stars[][2] = {
{-12, 10}, {-10, 12}, {-8, 11}, {-6, 13}, {-4, 12}, {0, 14}, {2, 11}, {4, 13}, {7, 12}, {10, 10}, {12, 13}};

glPointSize(2);
glBegin(GL_POINTS);
for (auto &star : stars)
{
glVertex2f(star[0], star[1]);
}
glEnd();
}

void initRain2()
{
for (int i = 0; i < MAX_RAIN2; i++)
{
rainX2[i] = -15.0f + static_cast<float>(rand()) / RAND_MAX * 30.0f; // X: -15 to 15
rainY2[i] = 5.0f + static_cast<float>(rand()) / RAND_MAX * 10.0f; // Y: 5 to 15
}
}

// OBJ018
void drawRain2()
{
glColor3f(0.5f, 0.7f, 1.0f); // Light blue

glBegin(GL_LINES);
for (int i = 0; i < MAX_RAIN2; i++)
{
glVertex2f(rainX2[i], rainY2[i]);
glVertex2f(rainX2[i] + 0.3f, rainY2[i] - 1.2f);
}
glEnd();
}

void drawBlade()
{
glBegin(GL_QUADS);
glVertex2f(0.0f, 0.0f); // Center (hub)
glVertex2f(0.6f, 0.0f); // Longer to the right
glVertex2f(0.7f, 0.1f); // Taller top
glVertex2f(0.0f, 0.1f); // Taller center top
glEnd();
}

// OBJ013
void drawWindmill(float x, float y)
{
glPushMatrix();
glTranslatef(x, y, 0.0f);
// Windmill Pole (up to y=3.0 relative to base)
glColor3f(0.4f, 0.2f, 0.0f);
glBegin(GL_QUADS);
glVertex2f(-0.08f, 0.0f);
glVertex2f(0.08f, 0.0f);
glVertex2f(0.05f, 3.0f);
glVertex2f(-0.05f, 3.0f);
glEnd();

// Blades at top of pole (translate up by 3.0f)
glPushMatrix();
glTranslatef(0.0f, 3.0f, 0.0f); // Move to top of pole
glRotatef(_angle1, 0.0f, 0.0f, 1.0f); // Rotate blades

for (int i = 0; i < 4; ++i)
{
glColor3f(1.0f, 0.0f, 0.0f);
drawBlade();
glRotatef(90, 0.0f, 0.0f, 1.0f);
}
glPopMatrix();

// Hub Circle at top of pole
glColor3f(0.2f, 0.2f, 0.2f);
float radius = 0.05f;
glBegin(GL_TRIANGLE_FAN);
glVertex2f(0.0f, 3.0f); // Center at top of pole
for (int i = 0; i <= 360; ++i)
{
float angle = i * 3.1416f / 180.0f;
glVertex2f(0.0f + cos(angle) * radius, 3.0f + sin(angle) * radius);
}
glEnd();

glPopMatrix();
}

// OBJ017

void drawBench2(float x, float y, float s)
{

// === SUPPORT ===
glColor3ub(0, 0, 0);
glBegin(GL_POLYGON);
glVertex2f(x + s * -1.8, y + s * 3.2);
glVertex2f(x + s * -1.8, y + s * -1.9);
glVertex2f(x + s * -1.4, y + s * -1.9);
glVertex2f(x + s * -1.4, y + s * 3.2);
glEnd();

glBegin(GL_POLYGON);
glVertex2f(x + s * -2.0, y + s * -2.1);
glVertex2f(x + s * -1.8, y + s * -1.9);
glVertex2f(x + s * -1.4, y + s * -1.9);
glVertex2f(x + s * -1.2, y + s * -2.1);
glEnd();

glBegin(GL_POLYGON);
glVertex2f(x + s * 1.8, y + s * 3.2);
glVertex2f(x + s * 1.8, y + s * -1.9);
glVertex2f(x + s * 1.4, y + s * -1.9);
glVertex2f(x + s * 1.4, y + s * 3.2);
glEnd();

glBegin(GL_POLYGON);
glVertex2f(x + s * 2.0, y + s * -2.1);
glVertex2f(x + s * 1.8, y + s * -1.9);
glVertex2f(x + s * 1.4, y + s * -1.9);
glVertex2f(x + s * 1.2, y + s * -2.1);
glEnd();

// === MID RECTANGLES ===
glColor3ub(0, 0, 0);
float midY[3][2] = {{3.0, 2.5}, {2.2, 1.7}, {1.4, 0.9}};
for (int i = 0; i < 3; i++)
{
glBegin(GL_POLYGON);
glVertex2f(x + s * -3, y + s * midY[i][0]);
glVertex2f(x + s * 3, y + s * midY[i][0]);
glVertex2f(x + s * 3, y + s * midY[i][1]);
glVertex2f(x + s * -3, y + s * midY[i][1]);
glEnd();
}

// === SIT AREA ===
glColor3ub(215, 215, 215);
glBegin(GL_POLYGON);
glVertex2f(x + s * -3, y + s * 0.4);
glVertex2f(x + s * 3, y + s * 0.4);
glVertex2f(x + s * 3.6, y + s * -0.6);
glVertex2f(x + s * -3.6, y + s * -0.6);
glEnd();

glColor3ub(218, 102, 47);
glBegin(GL_POLYGON);
glVertex2f(x + s * -3.6, y + s * -0.6);
glVertex2f(x + s * -3.6, y + s * -1.0);
glVertex2f(x + s * 3.6, y + s * -1.0);
glVertex2f(x + s * 3.6, y + s * -0.6);
glEnd();

// === SUPPORT2 ===
glColor3ub(0, 0, 0);
glBegin(GL_POLYGON);
glVertex2f(x + s * -3.2, y + s * -1.0);
glVertex2f(x + s * -3.2, y + s * -2.3);
glVertex2f(x + s * -2.8, y + s * -2.3);
glVertex2f(x + s * -2.8, y + s * -1.0);
glEnd();

glBegin(GL_POLYGON);
glVertex2f(x + s * -3.2, y + s * -2.3);
glVertex2f(x + s * -2.8, y + s * -2.3);
glVertex2f(x + s * -2.6, y + s * -2.5);
glVertex2f(x + s * -3.4, y + s * -2.5);
glEnd();

glBegin(GL_POLYGON);
glVertex2f(x + s * 2.6, y + s * -1.0);
glVertex2f(x + s * 2.6, y + s * -2.3);
glVertex2f(x + s * 3.0, y + s * -2.3);
glVertex2f(x + s * 3.0, y + s * -1.0);
glEnd();

glBegin(GL_POLYGON);
glVertex2f(x + s * 2.6, y + s * -2.3);
glVertex2f(x + s * 3.0, y + s * -2.3);
glVertex2f(x + s * 3.2, y + s * -2.5);
glVertex2f(x + s * 2.4, y + s * -2.5);
glEnd();
}

//-------------------- Animation functions ------------------

// ANIM002

void updateBoat()
{
if (boatMoving)
{
boat1PosX += boatSpeed;
if (boat1PosX > 25.0f)
{
boat1PosX = -25.0f;
}

boat2PosX -= boatSpeed;
if (boat2PosX < -25.0f)
{
boat2PosX = 25.0f;
}

glutPostRedisplay();
}
}


void handleMouseClick(int button, int state, int x, int y)
{
if (state == GLUT_DOWN)
{
if (button == GLUT_LEFT_BUTTON)
{
isRaining2 = !isRaining2; // Toggle rain on/off
}
else if (button == GLUT_RIGHT_BUTTON)
{
isRaining2 = !isRaining2; // Pause/resume falling
}
}
}
/*void handleKeyboardInput(unsigned char key, int x, int y)
{
if (key == 27)
{ // ESC key ASCII = 27
exit(0); // Exit the program
}
else if (key == 'b')
{
boatMoving = !boatMoving; // Toggle boat
}
else if (key == 'y')
{
isNight2 = false; // Toggle day/night
}
else if (key == 't')
{
isNight2 = true;
}

}
*/
// ANIM005

void updateCloud2()
{
cloud_x += cloud_speed;
if (cloud_x > 30.0f)
{
cloud_x = -20.0f; // Reset position
}

glutPostRedisplay();
}

int currentScenario = 1;  // Track which scene is active


void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        // ----- Scenario switching -----
        case '1':
           case '.':
            currentScenario = 1;
             glutPostRedisplay();
             break;
        case '2':
           case '=':
             currentScenario = 2;
              glutPostRedisplay();
               break;

        case '3':
            case '#':
             currentScenario = 3;
              glutPostRedisplay();
              break;





        // ----- Day/Night (scene 1) -----
        case 'd':
        case 'D':
            isDay = true;
            glutPostRedisplay();
            break;

        case 'n':
        case 'N':
            isDay = false;
            glutPostRedisplay();
            break;

        // ----- Rain (scene 1) -----
        case 'r':
        case 'R':
            isRaining = true;
            glutTimerFunc(0, updateRain, 0);
            break;

        // ----- Train movement -----
        case 's':
        case 'S':
            trainMoving = !trainMoving;
            break;

        // ----- Day/Night (scene 2) -----
        case 'x':
        case 'X':
            isDay1 = true;
            glutPostRedisplay();
            break;

        case 'g':
        case 'G':
            isDay1 = false;
            glutPostRedisplay();
            break;

        // ----- Rain (scene 2) -----
        case 'k':
        case 'K':
            isRaining1 = true;
            glutTimerFunc(0, updateRain1, 0);
            break;

        // ----- Boat movement -----
        case 'b':
            boatMoving = !boatMoving;
            break;

        // ----- Day/Night (scene 3) -----
        case 'y':
            isNight2 = false;
            break;

        case 't':
            isNight2 = true;

        // ----- Exit -----
        case 27: // ESC
            exit(0);
            break;
    }

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        if (button == GLUT_LEFT_BUTTON)
        {
            // Start or toggle rain for scene 1
            isRaining1 = true;
            glutTimerFunc(0, updateRain1, 0);

            // Toggle rain for scene 2
            isRaining2 = !isRaining2;
        }
        else if (button == GLUT_RIGHT_BUTTON)
        {
            // Pause/resume rain for scene 2
            isRaining2 = !isRaining2;
        }
    }
}


// ------------------ drawScence function ---------------

void Village()
{
glClear(GL_COLOR_BUFFER_BIT);
glLoadIdentity();

drawSky2();

if (isNight2)
{
drawMoon2();
drawStars2();
}
else
{
drawSun2();
drawClouds2(); // <-- Add this to show clouds in day
}

drawField();

drawWindmill(-6.0f, 3.5f); // Left side windmill
drawWindmill(-14.3f, 3.4f); // Left side windmill
drawHut(); // Hut 1
drawTreeTransformed(-7.90f, 7.0f, 1.0f, 1.0f); // Tree near Hut 1 (normal size)

translateHut(8.0f, 0.0f); // Hut 2
drawTreeTransformed(2.0f, 3.5f, 0.5f, 0.5f);
drawTreeTransformed(0.5f, 3.5f, 0.4f, 0.4f); // Tree between Hut 1 and 2 (slightly bigger)
drawTreeTransformed(1.2f, 2.2f, 0.2f, 0.2f);
drawWindmill(5.5f, 1.0f); // Right side windmill

flipHut(); // Hut 3
drawTreeTransformed(14.0f, 5.4f, 0.8f, 0.8f);
drawTreeTransformed(14.8f, 2.2f, 0.2f, 0.2f);
drawShurb2();
drawRoad2();

drawBench2(-12.0f, -0.2f, 0.3f);
drawBench2(-4.0f, -0.2f, 0.3f);
drawBench2(5.5f, -0.2f, 0.3f);

drawBench2(10.5f, -5.85f, 0.4f);
drawBench2(-10.5f, -5.85f, 0.4f);

drawRiver();
drawBoat(0.0f, -1.0f); // Static boat
drawBoat(boat1PosX, -3.5f); // Moving boat left → right
drawBoat(boat2PosX, -6.5f); // Moving boat right → left

if (isRaining2)
{
drawRain2();
}
drawBirds();
glutSwapBuffers();
}
// ANIM003
void updateWindmill()
{
_angle1 += 0.2f; // Rotate windmill
if (_angle1 > 360)
_angle1 -= 360;
}



void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (currentScenario == 1) RailwayStation();
    else if (currentScenario == 2) City();
    else if (currentScenario == 3) Village();

    glFlush();
}

// ----------------- Keyboard -----------------
/*void keyboard3(unsigned char key, int x, int y) {
    switch (key) {
        case '1': currentScenario = 1; break;
        case '2': currentScenario = 2; break;
        case '3': currentScenario = 3; break;
        case 27: exit(0); // ESC
    }
    glutPostRedisplay();
}
*/
void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-15.0, 15.0, -15.0, 15.0);
    initRain();
     initRain1(); // Set rain positions
      initRain2(); // <-- Add this here to initialize rain drop positions


    // Set rain positions
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
glutInitWindowSize(1020, 650);
glutInitWindowPosition(90, 50);
glutCreateWindow("Urban & Rural scape");
init();
glutDisplayFunc(display);
//glutKeyboardFunc(keyboard3);
glutIdleFunc([]()
{
updateWindmill();

updateBoat(); // call the boat animation
//updateRain();
updateCloud2();
updateBirds(); });
//glutKeyboardFunc(handleKeyboardInput);
//glutMouseFunc(handleMouseClick);
glutKeyboardFunc(keyboard);
//glutKeyboardFunc(keyboard2);

    glutMouseFunc(mouse1);

sound();
glLoadIdentity();

    glutMouseFunc(mouse);
 glutTimerFunc(16, update, 0);
glutTimerFunc(16, update2, 0);
 glutTimerFunc(16, trainTimer, 0); // Call every ~16ms (60 FPS)
  glutTimerFunc(30, updateRain, 0); // Repeat
   glutTimerFunc(30, updateRain1, 0); // Repeat
glutMainLoop();
return 0;
}


