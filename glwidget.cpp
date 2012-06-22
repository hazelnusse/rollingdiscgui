#include "glwidget.h"

#include <QtGui>
#include <QtOpenGL>

#include <cmath>
#include <gl2ps.h>
#include <GL/freeglut.h>
#include <GL/glu.h>

// TODO:
// label q0, q1, q2
// figure out orientation
// include figure in latex code

void DrawCircleXZ(double r, int num_segments)
{
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < num_segments; ++i) {
        double theta = 2.0 * M_PI * double(i) / num_segments;//get the current angle
        glVertex3d(r * std::cos(theta), 0.0, r * std::sin(theta));//output vertex
    }
    glEnd();
}

void printstring(const char *string, float angle)
{
  unsigned int i;
  const char *fonts[] =
    { "Times-Roman", "Times-Bold", "Times-Italic", "Times-BoldItalic",
      "Helvetica", "Helvetica-Bold", "Helvetica-Oblique", "Helvetica-BoldOblique",
      "Courier", "Courier-Bold", "Courier-Oblique", "Courier-BoldOblique",
      "Symbol", "ZapfDingbats" };

  /* call gl2psText before the glut function since glutBitmapCharacter
     changes the raster position... */
  gl2psTextOpt(string, fonts[4], 12, GL2PS_TEXT_C, angle);

  for (i = 0; i < strlen(string); i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
}

double RadToDeg(double angle)
{
    static double conversion = 45.0/std::atan(1.0);
    return angle*conversion;
}

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    qtBlack = QColor::fromRgb(0, 0, 0);
    qtWhite = QColor::fromRgb(255, 255, 255);
}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLWidget::SetAzimuth(double angle)
{
    if (angle != cam_azimuth) {
        cam_azimuth = angle;
        emit azimuthChanged(angle);
        updateGL();
    }
}

void GLWidget::SetElevation(double angle)
{
    if (angle != cam_elevation) {
        cam_elevation = angle;
        emit elevationChanged(angle);
        updateGL();
    }

}

void GLWidget::SetDistance(double distance)
{
    if (distance != cam_distance) {
        cam_distance = distance;
        emit distanceChanged(distance);
        updateGL();
    }

}

void GLWidget::Setq0(double angle)
{
    if (angle != q0) {
        q0 = angle;
        emit q0Changed(angle);
        updateGL();
    }
}

void GLWidget::Setq1(double angle)
{
    if (angle != q1) {
        q1 = angle;
        emit q1Changed(angle);
        updateGL();
    }
}

void GLWidget::Setq2(double angle)
{
    if (angle != q2) {
        q2 = angle;
        emit q2Changed(angle);
        updateGL();
    }
}

void GLWidget::Setq3(double distance)
{
    if (distance != q3) {
        q3 = distance;
        emit q3Changed(distance);
        updateGL();
    }
}

void GLWidget::Setq4(double distance)
{
    if (distance != q4) {
        q4 = distance;
        emit q4Changed(distance);
        updateGL();
    }
}

void GLWidget::SetRadius(double distance)
{
    if (r != distance) {
        r = distance;
        emit RadiusChanged(distance);
        updateGL();
    }
}

void GLWidget::SaveGL2PS()
{
    FILE *fp = fopen("test.pgf", "wb");
    int state = GL2PS_OVERFLOW, buffsize = 0;
    GLint viewport[4] = {0, 0, w, h};
    while (state == GL2PS_OVERFLOW) {
      buffsize += 1024*1024;
      gl2psBeginPage("test", "gl2psTest",
                     viewport,
                     GL2PS_PGF, GL2PS_BSP_SORT, GL2PS_DRAW_BACKGROUND | GL2PS_OCCLUSION_CULL | GL2PS_BEST_ROOT,
                     GL_RGBA, 0,
                     NULL,
                     0, 0, 0,
                     buffsize, fp,
                     "test_latex");
      updateGL();
      state = gl2psEndPage();
    }
    fclose(fp);
}

void GLWidget::initializeGL()
{
    qglClearColor(qtWhite);

    // glEnable(GL_LINE_SMOOTH);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth(1.5);

    glShadeModel(GL_FLAT);

    GLfloat lmodel_ambient[] = {1.0, 1.0, 1.0, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void GLWidget::paintGL()
{
    double t0 = std::tan(q0),
           s0_2 = std::sin(q0/2.0), c0_2 = std::cos(q0/2.0),
           c1 = std::cos(q1);

    GLfloat lw;
    glGetFloatv(GL_LINE_WIDTH, &lw);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0.0, 0.0, 0.0);        // Draw black

    glRotated(-90.0, 0.0, 1.0, 0.0); // rotate so y is up, z left
    glRotated(90.0, 1.0, 0.0, 0.0);  // rotate so y is left, z down
    
    glTranslated(cam_distance, 0.0, 0.0); // Go to disc contact point

    // Rotate to align with inertial axes
    glRotated(-RadToDeg(cam_elevation), 0.0, 1.0, 0.0);
    glRotated(-RadToDeg(cam_azimuth), 0.0, 0.0, 1.0);

    // Translate back to inertial origin
    glTranslated(-q3, -q4, 0.0);

    // Draw lines parallel to N.x, N.y, N.z
    glLineWidth(2.0*lw);
    glBegin(GL_LINES);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(1.1*q3, 0.0, 0.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 1.1*q4, 0.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, 0.5*r);
    glEnd();
    glLineWidth(lw);

    // Labels for inertial axes
    glRasterPos3d(1.16*q3, 0.0, 0.0);
    printstring("$\\mathbf{n}_x$", 0.0);
    glRasterPos3d(0.0, 1.2*q4, 0.0);
    printstring("$\\mathbf{n}_y$", 0.0);
    glRasterPos3d(0.0, 0.0, .6*r);
    printstring("$\\mathbf{n}_z$", 0.0);

    // Draw a circle with body fixed x and z axes in the plane of the circle,
    // the body fixed y perpendicular to the plane of the circle.
    glPushMatrix();
    glRasterPos3d(q3/2.0, q4+0.004, 0.0);
    printstring("$q_3$", 0.0);
    glRasterPos3d(q3+0.003, q4/2.0, 0.0);
    printstring("$q_4$", 0.0);

    // Line from contact point to x and y axes
    glBegin(GL_LINES);
    glVertex3d(q3, q4, 0.0);
    glVertex3d(q3, 0.0, 0.0);
    glVertex3d(q3, q4, 0.0);
    glVertex3d(0.0, q4, 0.0);
    glEnd();

    // Translate to disc contact point
    glTranslated(q3, q4, 0.0);
    glRotated(RadToDeg(q0), 0.0, 0.0, 1.0);

    // Draw line through contactpoint, aligned with disc path
    if (t0 != 0.0) {
      double b = q3 - q4/t0;
      double d;
      if (b < 0.0) {
          d = q3 / std::cos(q0);
      } else {
          d = q4 / std::sin(q0);
      }
      // Label q0
      glPushMatrix();
        glTranslated(-d, 0.0, 0.0);
        glRotated(-RadToDeg(q0), 0.0, 0.0, 1.0);
        glRasterPos3d(1.3*r*c0_2, 1.3*r*s0_2, 0.0);
        printstring("$q_0$", 0.0);
      glPopMatrix();

      glBegin(GL_LINES);
        // Line from contact point to x/y axes
        glVertex3d(.5*r, 0.0, 0.0);
        glVertex3d(-d, 0.0, 0.0);
        // Line from directly above contact point, even with disc center to
        // some height above disc
        glVertex3d(0.0, 0.0, -r*c1);
        glVertex3d(0.0, 0.0, -2.75*r*c1);
      glEnd();
    }

    glPushMatrix();
      glRotated(RadToDeg(q1)/2.0, 1.0, 0.0, 0.0);
      glRasterPos3d(0.0, 0.0, -2.2*r);
      printstring("$q_1$", 0.0);
    glPopMatrix();

    glRotated(RadToDeg(q1), 1.0, 0.0, 0.0);
    glTranslated(0.0, 0.0, -r);

    // Draw dimensioning lines in disc plane
    glBegin(GL_LINES);
      glVertex3d(0.0, 0.0,  0.9*r);
      glVertex3d(0.0, 0.0, -0.9*r);
      glVertex3d(0.0, 0.0, -1.1*r);
      glVertex3d(0.0, 0.0, -1.75*r);
    glEnd();

    // Draw the q2 label
    glPushMatrix();
      glRotated(RadToDeg(2.0*M_PI + q2)/2.0 + 9.0, 0.0, 1.0, 0.0);
      glRasterPos3d(0.0, 0.0, 0.5*r);
      printstring("$q_2$", 0.0);
    glPopMatrix();

    // Rotate to draw the disc
    glRotated(RadToDeg(q2), 0.0, 1.0, 0.0);

    // Disc fixed -z axes
    glBegin(GL_LINES);
      glVertex3d(0.0, 0.0, 0.0);
      glVertex3d(0.0, 0.0, -r*.9);
    glEnd();

//    glLineWidth(2.0*lw);
    glPushMatrix();
      glRotated(-90.0, 1.0, 0.0, 0.0);
      GLUquadricObj *q = gluNewQuadric();
      GLfloat mat[] = {0.9, 0.9, 0.9, 1.0};
      glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
      gluDisk(q, 0.0, r, 100, 3);
      mat[0] = mat[1] = mat[2] = 0.0;
      glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
    glPopMatrix();
    DrawCircleXZ(r, 100);
//    glLineWidth(lw);

    glPopMatrix();
}

void GLWidget::resizeGL(int width, int height)
{
    w = width;
    h = height;
    // int side = qMin(width, height);
    // glViewport((width - side) / 2, (height - side) / 2, side, side);
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.5*q3, 1.5*q3, -1.5*q4, 1.5*q4, -1.5, 1.5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
