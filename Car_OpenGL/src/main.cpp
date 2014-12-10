
/****************************************************************************************
Project: OpenGL Car Rendering
Author: Sourabh Bodas
Date: 09-02-2011

Description: This is a test code written for rendering a car using OpenGL, the functions 
used in this program are based on the methods described in various texts to generate Sphere, Torus and
Cylinder. The rendered surfaces are not textured. 

*****************************************************************************************/
#include "all_includes.h"

using namespace std;

int AngleX3d,AngleY3d,PreviousX3d,PreviousY3d;
float Zoom = 0.5;
bool WhetherPressed, WhenWhetherPressedd;
float Pi = 3.1415926535;

bool lights = true;

GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light_position[] = { 6.0f, 4.0f, 5.0f, 0.0f };

GLfloat mat_ambient[]    = { 0.0f, 2.0f, 0.0f, 1.0f };
GLfloat mat_diffuse[]    = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat mat_specular[]   = { 0.8f, 1.0f, 0.8f, 1.0f };
GLfloat high_shininess[] = { 100.0f };

GLfloat global_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };

// Different parameters for solid objects
int SphereSlices = 60;
int SphereCuts = 60;
int ChipsTorus = 110;
int CutsTorus = 100;
int CutsCylinder = 100;
int CylinderSlices = 2;

// Positions of different objects in the rendering scene
double FrontWheelsPos = -35.0;
double RearWheelsPos = 30.0;
double WheelLeftPos = 25.0;
double WheelRightPos = -25.0;
int BumperHeight = 20;
int BootHeight = 35;

// Declaration of vertices to be used in rendering
std::vector < std::vector < double > > VerticesSphere;
std::vector < std::vector < std::vector < double > > > VerticesTorus;
std::vector < std::vector < std::vector < double > > > VerticesCylinder;

GLuint CarCallStack;

/* GLUT callback Handlers */

static void resize(int w, int h)
{

    glPushMatrix();
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(
                    45,        //view_angle //45
                    (float)w/h,
                    1,
                    1000 //1000.0
                    );

    gluLookAt(
    0, 0, 20,
    //Camera_Target[0], Camera_Target[1], Camera_Target[2],
    0,0,0,
    //seen_point[0],seen_point[1],seen_point[2],        //seen point
    0,1,0);        //Up Vector


    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

std::vector <double> CalculateNormal (std::vector <double> a, std::vector <double> b, std::vector <double> c){
    std:: vector <double> result (3,0.0);
	// The normals of the vertices can be computed using the cross product of two vectors, 
	// for which we are considering the normal
    double XVector1 = (b[0] - a[0]);
    double YVector1 = (b[1] - a[1]);
    double ZVector1 = (b[2] - a[2]);
    double XVector2 = (c[0] - b[0]);
    double YVector2 = (c[1] - b[1]);
    double ZVector2 = (c[2] - b[2]);

	// Taking cross product, it is vector product hence we keep the direction as well as magnitude
    double x = (YVector1 * ZVector2) - (ZVector1 * YVector2);
    double y = (ZVector1 * XVector2) - (XVector1 * ZVector2);
    double z = (XVector1 * YVector2) - (YVector1 * XVector2);

	// Calculating the magnitude of the vector calculated above
    float ResMagnitude = (float)(sqrt((x * x) + (y * y) + (z * z)));

//     avoid division by 0
    if (ResMagnitude == 0.0f)
        ResMagnitude = 1.0f;

//     reduce to unit size
    x /= ResMagnitude;
    y /= ResMagnitude;
    z /= ResMagnitude;

    result[0] = x;
    result[1] = y;
    result[2] = z;

    return result;
}

void GenerateCylinderPoints(int Radius, int Height){
    std::vector < std::vector < std::vector < double > > > temp (CylinderSlices+1, std::vector < std::vector < double > > (0, std::vector < double > (3,1.0)));
    VerticesCylinder = temp;
    // First we define the constants
    
    float VerticalCutSize = (2*Pi) / CutsCylinder; // We need to turn 360 degrees
    float HorizontalSliecSize = Height / CylinderSlices; // We need to turn 360 degrees

    std::vector <double> CurrentPoint(3);
    int x = 0;
    for( float i = (-Height/2); i <= (Height/2); i+= HorizontalSliecSize){
        int y = 0;
        for (double j = 0; j <= (2*Pi+0.1); j += VerticalCutSize){
            CurrentPoint[0] = Radius * cos(j);
            CurrentPoint[1] = Radius * sin(j);
            CurrentPoint[2] = i;

            if(y == CutsCylinder){
                CurrentPoint[1] = 0;
            }

            VerticesCylinder[x].push_back(CurrentPoint);
            y++;
        }
    x++;
    }
}

void GenerateTorusPoints(int r0, int r1){
    std::vector < std::vector < std::vector < double > > > temp (ChipsTorus+2, std::vector < std::vector < double > > (0, std::vector < double > (3,1.0)));
    VerticesTorus = temp;
    // First we define the constants
    float Pi = 3.14159265;
    float smallCircleStep = (2*Pi) / ChipsTorus; // We need to turn 360 degrees
    float bigCircleStep = (2*Pi) / CutsTorus; // We need to turn 360 degrees

    std::vector <double> CurrentPoint(3);

    int x = 0;
    for( float j = 0; j <= (2*Pi+0.1); j+= smallCircleStep){
        int y = 0;
        for (float i = 0; i <= (2*Pi+0.1); i+= bigCircleStep){
            CurrentPoint[0] = (r0 + (r1 * cos(i)))*cos(j);
            CurrentPoint[1] = (r0 + (r1 * cos(i)))*sin(j);
            CurrentPoint[2] = r1 * sin(i);

            VerticesTorus[x].push_back(CurrentPoint);
        }
        x++;
    }
}

void GenerateSpherePoints(int Radius){

    // First we define the constants
    float Pi = 3.14159265;
    float VerticalCutSize = (2*Pi) / SphereCuts; // We need to turn 360 degrees
    float HorizontalSliecSize = Pi / SphereSlices; // We need to turn 180 degrees

    // Loop all meridians and parallels with the defined steps
    std::vector <double> CurrentPoint(3);
    CurrentPoint[0] = 0;
    CurrentPoint[1] = sin(-Pi/2)*Radius;
    CurrentPoint[2] = 0;
    VerticesSphere.push_back(CurrentPoint);

    // First we need to add the first point
    for (float j = ((-Pi/2) + HorizontalSliecSize); j < ((Pi/2)); j+= HorizontalSliecSize){
        for( float i = 0; i < (2*Pi); i+= VerticalCutSize){

            CurrentPoint[0] = sin(i)*cos(j)*Radius;
            CurrentPoint[1] = sin(j)*Radius;
            CurrentPoint[2] = cos(j)*cos(i)*Radius;

            // We will have a vector of points for every parallel
            VerticesSphere.push_back(CurrentPoint);
        }
    }

    // The last point
    CurrentPoint[0] = 0;
    CurrentPoint[1] = sin(Pi/2)*Radius;
    CurrentPoint[2] = 0;

    VerticesSphere.push_back(CurrentPoint);

}

void DrawInnerWheel(int Radius){

    float InnerWheelStep = (2*Pi) / 16; // We need to turn 360 degrees
    int t = 0;
    for(float i = 0; i <= (2*Pi+0.1); i += InnerWheelStep){
        if(t%2 == 0){ // We only want to draw at every even step
            glBegin(GL_TRIANGLE_FAN);
                std::vector <double> a (3,0.0);
                std::vector <double> b (3,0.0);
                std::vector <double> c (3,0.0);

                a[0] =  Radius * cos(i);
                a[1] =  Radius * sin(i);
                a[2] = 0;
                c[0] = Radius * cos(i+InnerWheelStep);
                c[1] = Radius * sin(i+InnerWheelStep);

				std::vector <double> normal = CalculateNormal(a,c,b);
                glNormal3d(normal[0],normal[1],normal[2]);

                glVertex3f(a[0],a[1],a[2]);
                glVertex3f(b[0],b[1],b[2]);
                glVertex3f(c[0],c[1],c[2]);
            glEnd();
        }
        t++;
    }
    glEnd();

}

void DrawCylinder(){

    for(int i = 0; i < VerticesCylinder.size()-1; i++){
        glBegin(GL_QUAD_STRIP);
        for(int j = 0; j < VerticesCylinder[i].size()-1; j++){
            std:: vector <double> a = VerticesCylinder[i][j];
            std:: vector <double> b = VerticesCylinder[i+1][j];
            std:: vector <double> c = VerticesCylinder[i][j+1];
            std:: vector <double> d = VerticesCylinder[i+1][j+1];
            std:: vector <double> normal = CalculateNormal(a,b,c);

            // Calculate the normal of the polygon
            glNormal3d(normal[0],normal[1],normal[2]);

            glVertex3f(a[0],a[1],a[2]);
            glVertex3f(b[0],b[1],b[2]);
            glVertex3f(c[0],c[1],c[2]);
            glVertex3f(d[0],d[1],d[2]);

        }
        glEnd();
    }
}

void DrawSphere(){
    double x = 0;
    double y = 0;
    double z = 0;

    // TOP of the sphere
    glBegin(GL_TRIANGLE_FAN);

        for (int i = 0; i < SphereCuts + 1; i++){
            x = VerticesSphere[i][0];
            y = VerticesSphere[i][1];
            z = VerticesSphere[i][2];
            glNormal3d(0.0, 0.0, 1.0);
            glVertex3f(x,y,z);
        }
        // We need to close the circle in the fan so we need to come
        // back to the point we started from on top and bottom
        x = VerticesSphere[1][0];
        y = VerticesSphere[1][1];
        z = VerticesSphere[1][2];
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3f(x,y,z);

    glEnd();

    // BOTTOM of the sphere
    glBegin(GL_TRIANGLE_FAN);

        for (int i = 0;i < SphereCuts + 1; i++){

            x = VerticesSphere[VerticesSphere.size() - 1 - i][0];
            y = VerticesSphere[VerticesSphere.size() - 1 - i][1];
            z = VerticesSphere[VerticesSphere.size() - 1 - i][2];
            glNormal3d(0.0, 0.0, 1.0);
            glVertex3f(x,y,z);
        }
        // We need to close the circle in the fan so we need to come
        // back to the point we started from on top and bottom
        x = VerticesSphere[VerticesSphere.size()-2][0];
        y = VerticesSphere[VerticesSphere.size()-2][1];
        z = VerticesSphere[VerticesSphere.size()-2][2];
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3f(x,y,z);
    glEnd();

    // Now we draw the rest of the sphere
    int index, indeXVector2, index3, index4;
    for ( int i = 0; i < SphereSlices - 2; i++){
        for ( int j = 0; j < SphereCuts; j++){
            glBegin(GL_QUADS);
                index3 = ((i+1) * SphereCuts) + j + 1;
                index = (i * SphereCuts) + j + 1;
                indeXVector2 = (i * SphereCuts) + j + 2;
                index4 = ((i+1) * SphereCuts) + j + 2;

                std:: vector <double> a = VerticesSphere[index3];
                std:: vector <double> b = VerticesSphere[index];
                std:: vector <double> c = VerticesSphere[indeXVector2];
                std:: vector <double> d = VerticesSphere[index4];

                std:: vector <double> normal = CalculateNormal(a,b,c);
                glNormal3d(normal[0],normal[1],normal[2]);

                glVertex3f(a[0],a[1],a[2]);
                glVertex3f(b[0],b[1],b[2]);
                glVertex3f(c[0],c[1],c[2]);
                glVertex3f(d[0],d[1],d[2]);
            glEnd();
        }
    }
}

void DrawTorus(){
    for(int i = 0; i < VerticesTorus.size()-1; i++){
        glBegin(GL_QUAD_STRIP);
        for(int j = 0; j < VerticesTorus[i].size()-1; j++){

            std:: vector <double> a = VerticesTorus[i][j];
            std:: vector <double> b = VerticesTorus[i+1][j];
            std:: vector <double> c = VerticesTorus[i][j+1];
            std:: vector <double> d = VerticesTorus[i+1][j+1];
            std:: vector <double> normal = CalculateNormal(a,b,c);

            // Calculate the normal of the polygon
            glNormal3d(normal[0],normal[1],normal[2]);

            glVertex3f(a[0],a[1],a[2]);
            glVertex3f(b[0],b[1],b[2]);
            glVertex3f(c[0],c[1],c[2]);
            glVertex3f(d[0],d[1],d[2]);
        }
        glEnd();
    }
}

void DrawFrontGlassPanel(){
    // Now we draw the front glass panel
    glBegin(GL_QUADS);
        // defining points
        std::vector <double> a(3,0.0);
        std::vector <double> b(3,0.0);
        std::vector <double> c(3,0.0);
        std::vector <double> d(3,0.0);

        a[0] = FrontWheelsPos;a[1] = BumperHeight;a[2] = WheelRightPos+4;
        b[0] = FrontWheelsPos+15;b[1] = BootHeight-5;b[2] = WheelLeftPos-4;
        c[0] = FrontWheelsPos+15;c[1] = BootHeight-5;c[2] = WheelLeftPos-4;
        d[0] = FrontWheelsPos;d[1] = BumperHeight;d[2] = WheelRightPos+4;
        // compuitng normals
        std::vector <double> normal = CalculateNormal(a,b,c);
        glNormal3d(normal[0],normal[1],normal[2]);

        glVertex3d(a[0],a[1],a[2]);
        glVertex3d(b[0],b[1],b[2]);
        glVertex3d(c[0],c[1],c[2]);
        glVertex3d(d[0],d[1],d[2]);

    glEnd();
}

void DrawLeftPanel(){
    // Now we draw the left side panel
    glBegin(GL_QUADS);
        // defining points
        std::vector <double> a(3,0.0);
        std::vector <double> b(3,0.0);
        std::vector <double> c(3,0.0);
        std::vector <double> d(3,0.0);

        a[0] = FrontWheelsPos+15;a[1] = BootHeight-5;a[2] = WheelLeftPos-4;
        b[0] = RearWheelsPos;b[1] = BootHeight;b[2] = WheelLeftPos-4;
        c[0] = RearWheelsPos;c[1] = 0;c[2] = WheelLeftPos-4;
        d[0] = FrontWheelsPos+15;d[1] = 0;d[2] = WheelLeftPos-4;
        // compuitng normals
        std::vector <double> normal = CalculateNormal(a,b,c);
        glNormal3d(-normal[0],-normal[1],-normal[2]);

        glVertex3d(a[0],a[1],a[2]);
        glVertex3d(b[0],b[1],b[2]);
        glVertex3d(c[0],c[1],c[2]);
        glVertex3d(d[0],d[1],d[2]);
    glEnd();
}

void DrawRightPanel(){
    // Now we draw the right side panel
    glBegin(GL_QUADS);
        // defining points
        std::vector <double> a(3,0.0);
        std::vector <double> b(3,0.0);
        std::vector <double> c(3,0.0);
        std::vector <double> d(3,0.0);

        a[0] = FrontWheelsPos+15;a[1] = BootHeight-5;a[2] = WheelRightPos+4;
        b[0] = RearWheelsPos;b[1] = BootHeight;b[2] = WheelRightPos+4;
        c[0] = RearWheelsPos;c[1] = 0;c[2] = WheelRightPos+4;
        d[0] = FrontWheelsPos+15;d[1] = 0;d[2] = WheelRightPos+4;
        // compuitng normals
        std::vector <double> normal = CalculateNormal(a,b,c);
        glNormal3d(normal[0],normal[1],normal[2]);

        glVertex3d(a[0],a[1],a[2]);
        glVertex3d(b[0],b[1],b[2]);
        glVertex3d(c[0],c[1],c[2]);
        glVertex3d(d[0],d[1],d[2]);
    glEnd();
}

void DrawFrontLeftPanel(){
    // Now we draw the left side panel
    glBegin(GL_QUADS);
        // defining points
        std::vector <double> a(3,0.0);
        std::vector <double> b(3,0.0);
        std::vector <double> c(3,0.0);
        std::vector <double> d(3,0.0);

        a[0] = FrontWheelsPos;a[1] = BumperHeight;a[2] = WheelLeftPos-4;
        b[0] = FrontWheelsPos+15;b[1] = BootHeight-5;b[2] = WheelLeftPos-4;
        c[0] = FrontWheelsPos+15;c[1] = 0;c[2] = WheelLeftPos-4;
        d[0] = FrontWheelsPos;d[1] = 0;d[2] = WheelLeftPos-4;
        // compuitng normals
        std::vector <double> normal = CalculateNormal(a,b,c);
        glNormal3d(-normal[0],-normal[1],-normal[2]);

        glVertex3d(a[0],a[1],a[2]);
        glVertex3d(b[0],b[1],b[2]);
        glVertex3d(c[0],c[1],c[2]);
        glVertex3d(d[0],d[1],d[2]);
    glEnd();
}

void DrawBackPanel(){
    // First we draw the backpanel
    glBegin(GL_QUADS);
        // defining points
        std::vector <double> a(3,0.0);
        std::vector <double> b(3,0.0);
        std::vector <double> c(3,0.0);
        std::vector <double> d(3,0.0);

        a[0] = RearWheelsPos;a[1] = BootHeight;a[2] = WheelRightPos+4;
        b[0] = RearWheelsPos;b[1] = BootHeight;b[2] = WheelLeftPos-4;
        c[0] = RearWheelsPos;c[1] = 0;c[2] = WheelLeftPos-4;
        d[0] = RearWheelsPos;d[1] = 0;d[2] = WheelRightPos+4;
        // compuitng normals
        std::vector <double> normal = CalculateNormal(a,b,c);
        glNormal3d(normal[0],normal[1],normal[2]);

        glVertex3d(a[0],a[1],a[2]);
        glVertex3d(b[0],b[1],b[2]);
        glVertex3d(c[0],c[1],c[2]);
        glVertex3d(d[0],d[1],d[2]);

    glEnd();
}

void DrawFrontPanel(){
    // Now we draw the front panel
    glBegin(GL_QUADS);
        // defining points
        std::vector <double> a(3,0.0);
        std::vector <double> b(3,0.0);
        std::vector <double> c(3,0.0);
        std::vector <double> d(3,0.0);

        a[0] = FrontWheelsPos;a[1] = BumperHeight;a[2] = WheelRightPos+4;
        b[0] = FrontWheelsPos;b[1] = BumperHeight;b[2] = WheelLeftPos-4;
        c[0] = FrontWheelsPos;c[1] = 0;c[2] = WheelLeftPos-4;
        d[0] = FrontWheelsPos;d[1] = 0;d[2] = WheelRightPos+4;
        // compuitng normals
        std::vector <double> normal = CalculateNormal(a,b,c);
        glNormal3d(normal[0],normal[1],normal[2]);

        glVertex3d(a[0],a[1],a[2]);
        glVertex3d(b[0],b[1],b[2]);
        glVertex3d(c[0],c[1],c[2]);
        glVertex3d(d[0],d[1],d[2]);

    glEnd();
}

void DrawFrontRightPanel(){
    // Now we draw the right side panel
    glBegin(GL_QUADS);
        // defining points
        std::vector <double> a(3,0.0);
        std::vector <double> b(3,0.0);
        std::vector <double> c(3,0.0);
        std::vector <double> d(3,0.0);

        a[0] = FrontWheelsPos;a[1] = BumperHeight;a[2] = WheelRightPos+4;
        b[0] = FrontWheelsPos+15;b[1] = BootHeight-5;b[2] = WheelRightPos+4;
        c[0] = FrontWheelsPos+15;c[1] = 0;c[2] = WheelRightPos+4;
        d[0] = FrontWheelsPos;d[1] = 0;d[2] = WheelRightPos+4;
        // compuitng normals
        std::vector <double> normal = CalculateNormal(a,b,c);
        glNormal3d(normal[0],normal[1],normal[2]);

        glVertex3d(a[0],a[1],a[2]);
        glVertex3d(b[0],b[1],b[2]);
        glVertex3d(c[0],c[1],c[2]);
        glVertex3d(d[0],d[1],d[2]);
    glEnd();
}




void DrawBottomPanel(){
    // Now we draw the bottom panel
    glBegin(GL_QUADS);
        // defining points
        std::vector <double> a(3,0.0);
        std::vector <double> b(3,0.0);
        std::vector <double> c(3,0.0);
        std::vector <double> d(3,0.0);

        a[0] = FrontWheelsPos;a[1] = 0;a[2] = WheelRightPos+4;
        b[0] = FrontWheelsPos;b[1] = 0;b[2] = WheelLeftPos-4;
        c[0] = RearWheelsPos;c[1] = 0;c[2] = WheelLeftPos-4;
        d[0] = RearWheelsPos;d[1] = 0;d[2] = WheelRightPos+4;
        // compuitng normals
        std::vector <double> normal = CalculateNormal(a,b,c);
        glNormal3d(normal[0],normal[1],normal[2]);

        glVertex3d(a[0],a[1],a[2]);
        glVertex3d(b[0],b[1],b[2]);
        glVertex3d(c[0],c[1],c[2]);
        glVertex3d(d[0],d[1],d[2]);
    glEnd();
}

void DrawTopPanel(){
    // Now we draw the bottom panel
    glBegin(GL_QUADS);
        // defining points
        std::vector <double> a(3,0.0);
        std::vector <double> b(3,0.0);
        std::vector <double> c(3,0.0);
        std::vector <double> d(3,0.0);

        a[0] = FrontWheelsPos+15;a[1] = BootHeight-5;a[2] = WheelRightPos+4;
        b[0] = FrontWheelsPos;b[1] = BootHeight;b[2] = WheelLeftPos-4;
        c[0] = RearWheelsPos;c[1] = BootHeight;c[2] = WheelLeftPos-4;
        d[0] = RearWheelsPos+15;d[1] = BootHeight-5;d[2] = WheelRightPos+4;
        // compuitng normals
        std::vector <double> normal = CalculateNormal(a,b,c);
        glNormal3d(normal[0],normal[1],normal[2]);

        glVertex3d(a[0],a[1],a[2]);
        glVertex3d(b[0],b[1],b[2]);
        glVertex3d(c[0],c[1],c[2]);
        glVertex3d(d[0],d[1],d[2]);
    glEnd();
}

void DrawHeadLamps()
{
	glColor3d(1,1,0);
	GenerateCylinderPoints(0.2, 0.1);
	DrawCylinder();
}

void DrawShaft(){
    glColor3d(0.1, 0.0, 0.0);
    GenerateCylinderPoints(1,40);
    DrawCylinder();
}



static void display(void)
{

	//const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	//const double a = t*90.0;

	//Front right wheel
    glPushMatrix();
        glScalef(Zoom, Zoom, Zoom);
        glRotatef(AngleY3d/3,1.0,0.0,0.0);
        glRotatef(AngleX3d/3,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3d(1,1,0);
        glTranslatef(FrontWheelsPos, 0.0, WheelRightPos);
        glCallList(CarCallStack);
    glPopMatrix();

	//Front left wheel
    glPushMatrix();
        glScalef(Zoom, Zoom, Zoom);
        glRotatef(AngleY3d/3,1.0,0.0,0.0);
        glRotatef(AngleX3d/3,0.0,0.0,1.0);
        glTranslatef(FrontWheelsPos, 0.0, WheelLeftPos);
        glCallList(CarCallStack);
    glPopMatrix();

	//Rear right wheel
    glPushMatrix();
        glScalef(Zoom, Zoom, Zoom);
        glRotatef(AngleY3d/3,1.0,0.0,0.0);
        glRotatef(AngleX3d/3,0.0,0.0,1.0);
        glTranslatef(RearWheelsPos, 0.0, WheelRightPos);
        glCallList(CarCallStack);
    glPopMatrix();

	//Rear left wheel
	glPushMatrix();
        glScalef(Zoom, Zoom, Zoom);
        glRotatef(AngleY3d/3,1.0,0.0,0.0);
        glRotatef(AngleX3d/3,0.0,0.0,1.0);
        glTranslatef(RearWheelsPos, 0.0, WheelLeftPos);
        glCallList(CarCallStack);
    glPopMatrix();

    glPushMatrix();
        glScalef(Zoom, Zoom, Zoom);
        glRotatef(AngleY3d/3,1.0,0.0,0.0);
        glRotatef(AngleX3d/3,0.0,0.0,1.0);
        glCallList(CarCallStack+1);
    glPopMatrix();

	//Draw hind axis
    glPushMatrix();
        glScalef(Zoom, Zoom, Zoom);
        glRotatef(AngleY3d/3,1.0,0.0,0.0);
        glRotatef(AngleX3d/3,0.0,0.0,1.0);
        glTranslatef(RearWheelsPos, 0.0, 0.0);
        DrawShaft();
    glPopMatrix();

	// Draw front axis
    glPushMatrix();
        glScalef(Zoom, Zoom, Zoom);
        glRotatef(AngleY3d/3,1.0,0.0,0.0);
        glRotatef(AngleX3d/3,0.0,0.0,1.0);
        glTranslatef(FrontWheelsPos, 0.0, 0.0);
        DrawShaft();
    glPopMatrix();

	//Draw head lamp left
	glPushMatrix();
        glScalef(Zoom, Zoom, Zoom);
        glRotatef(AngleY3d/3,0.0,1.0,0.0);
        glRotatef(AngleX3d/3,0.0,1.0,0.0);
        glTranslatef(FrontWheelsPos, 0.0, WheelLeftPos);
    glPopMatrix();

	//Draw head lamp right
	glPushMatrix();
        glScalef(Zoom, Zoom, Zoom);
        glRotatef(AngleY3d/3,0.0,1.0,0.0);
        glRotatef(AngleX3d/3,0.0,1.0,0.0);
        glTranslatef(FrontWheelsPos, 0.0, WheelRightPos);
    glPopMatrix();


    glutSwapBuffers();
}

static void Initialize(void)
{
    CarCallStack = glGenLists (2);
    glNewList(CarCallStack, GL_COMPILE);
        GenerateSpherePoints(1);
        glColor3d(0.8, 0.0, 0.6);
        DrawSphere();

        GenerateTorusPoints(10,2);
        glColor3d(0.0, 0.0, 0.0);
        DrawTorus();

        GenerateCylinderPoints(8,2);
        glColor3d(0.0, 0.0, 0.0);
        DrawCylinder();

        glColor3d(0.0, 0.0, 0.1);
        DrawInnerWheel(10);
    glEndList();

    glNewList(CarCallStack+1, GL_COMPILE);
        glColor3d(0.9,0.2,0.1);
        DrawFrontPanel();
        DrawBackPanel();
        DrawLeftPanel();
        DrawRightPanel();
        DrawBottomPanel();
		DrawFrontLeftPanel();
		DrawFrontRightPanel();
		//DrawTopPanel();

		glColor3d(0.0,0.1,0.1);
		DrawFrontGlassPanel();
    glEndList();
}

static void MouseFunctionality(int button, int state, int x, int y)
{
     if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
      {
            WhetherPressed=1;
            PreviousX3d=x;
            PreviousY3d=y;
      }

     if (button==GLUT_LEFT_BUTTON&& state==GLUT_UP)
            WhetherPressed=0;

     if (button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
     {
            WhenWhetherPressedd=1;
            PreviousX3d=x;
            PreviousY3d=y;
     }

     if (button==GLUT_RIGHT_BUTTON&& state==GLUT_UP)
            WhenWhetherPressedd=0;

     glutPostRedisplay();
}

void MouseMotion(int x,int y)
 {
    if (WhetherPressed)
    {
        AngleX3d=AngleX3d+(x-PreviousX3d);
        AngleY3d=AngleY3d+(y-PreviousY3d);
    }
    if (WhenWhetherPressedd)
    {
        Zoom+=(double)(y-PreviousY3d)/300;
        if(Zoom > 5)
            Zoom=5;
        else
            if(Zoom < 0.2)
                Zoom=0.2;
    }
    PreviousX3d=x;
    PreviousY3d=y;

    glutPostRedisplay();
 }

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {

        case 'q':
            exit(0);
            break;
        case 'l':if(lights)
                 {
                     glDisable(GL_LIGHTING);
                     lights=false;
                 }
                 else
                 {
                    glEnable(GL_LIGHTING);
                    lights=true;
                 }
               break;
    }

    glutPostRedisplay();
}

static void IdleScreen(void)
{
    glutPostRedisplay();
}



/* Program entry point */

int main(int argc, char *argv[])
{
	// main function to render car
    GenerateSpherePoints(1);
    GenerateTorusPoints(6,2);
    GenerateCylinderPoints(4,2);

    glutInit(&argc, argv);
    glutInitWindowSize(1080,960);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutMouseFunc(MouseFunctionality);
    glutMotionFunc(MouseMotion);
    glutKeyboardFunc(key);
    glutIdleFunc(IdleScreen);

    glClearColor(1,1,1,0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	glEnable(GL_NORMALIZE);

//    //so we don't need these...
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	
    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    // set material properties which will be assigned by glColor
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    float specReflection[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
    glMateriali(GL_FRONT, GL_SHININESS, 96);
    Initialize();
    glutMainLoop();

    return 0;
}