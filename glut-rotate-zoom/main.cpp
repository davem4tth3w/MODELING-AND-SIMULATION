#ifdef __APPLE__
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <cmath>

static int slices = 50;
static int stacks = 50;
static double rotationAngle = 0.0;
static double zoomFactor = 1.0;

/* Custom function to draw a cylinder */
void drawCylinder(GLfloat radius, GLfloat height, GLint slices, GLint stacks) {
    GLUquadric *quad = gluNewQuadric();
    gluCylinder(quad, radius, radius, height, slices, stacks);
    gluDeleteQuadric(quad);
}

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t * 90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(0,0,1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Calculate the position of the eye for zooming
    double eyeX = 5 * zoomFactor;
    double eyeY = 5 * zoomFactor;
    double eyeZ = 15 * zoomFactor;

    // Adjust the view by setting the camera position and orientation
    gluLookAt(eyeX, eyeY, eyeZ,  // eye position
              0, 0, 0,   // look-at position
              0, 1, 0);  // up vector

    // Translate the tank body to the center of the screen
    glPushMatrix();
    glTranslated(0, 0, -10);
    glRotated(rotationAngle, 0, 1, 0);

    // Tank Body
    glColor3f(0.5, 0.5, 0.5);
    glutSolidCube(5);

    // Tank Turret
    glPushMatrix();
    glTranslated(0, 2.5, 0);
    glColor3f(0.3, 0.3, 0.3);
    glutSolidCube(2);
    glPopMatrix();

    // Tank Main Gun
    glPushMatrix();
    glTranslated(0, 3.0, 2.0);
    glRotated(-45, 1, 0, 0);
    glColor3f(0.3, 0.3, 0.3);
    drawCylinder(0.3, 3.0, slices, stacks); // Custom cylinder function
    glPopMatrix();

    // Tank Tracks
    glPushMatrix();
    glColor3f(0.1, 0.1, 0.1);
    glScaled(1, 0.2, 1);
    glutSolidTorus(0.5, 2, 10, 10);
    glPopMatrix();

    // Tank Road Wheels with Link
    glColor3f(0.3, 0.3, 0.3);
    for (int i = -2; i <= 2; ++i) {
        for (int j = -1; j <= 1; ++j) {
            glPushMatrix();
            glTranslated(i * 1.5, -3.5, j * 2.5);
            glutSolidTorus(0.3, 0.8, slices, stacks);
            // Draw link
            glColor3f(0.1, 0.1, 0.1);
            glTranslated(0, -0.4, 0);
            glutSolidCube(0.3);
            glPopMatrix();
        }
    }

    // Tank Hatch
    glPushMatrix();
    glColor3f(0.3, 0.3, 0.3);
    glTranslated(0, 4.0, 0);
    glBegin(GL_QUADS);
    glVertex3f(-0.5, 0.0, -0.5); glVertex3f(0.5, 0.0, -0.5); glVertex3f(0.5, 0.0, 0.5); glVertex3f(-0.5, 0.0, 0.5);
    glEnd();
    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'a':
            rotationAngle -= 5.0;
            break;
        case 'd':
            rotationAngle += 5.0;
            break;
        case 'w':
            zoomFactor *= 0.9;
            break;
        case 's':
            zoomFactor /= 0.9;
            break;
        case '+':
            slices++;
            stacks++;
            break;
        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("War Tank");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMainLoop();

    return EXIT_SUCCESS;
}
