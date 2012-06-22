#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

//class QtLogo;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
    void SetAzimuth(double angle);
    void SetElevation(double angle);
    void SetDistance(double distance);

    void Setq0(double angle);
    void Setq1(double angle);
    void Setq2(double angle);
    void Setq3(double distance);
    void Setq4(double distance);
    void SetRadius(double distance);

    void SaveGL2PS();

signals:
    void q0Changed(double angle);
    void q1Changed(double angle);
    void q2Changed(double angle);
    void q3Changed(double angle);
    void q4Changed(double angle);

    void azimuthChanged(double distance);
    void elevationChanged(double distance);
    void distanceChanged(double distance);

    void RadiusChanged(double distance);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    int w, h;

    // Camera variables
    double cam_azimuth, cam_elevation, cam_distance;

    // Disc configuration variables
    double q0, q1, q2, q3, q4, r;

    QPoint lastPos;
    QColor qtBlack;
    QColor qtWhite;
};
#endif
