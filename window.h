#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

// class QwtSpinBox;
class QDoubleSpinBox;
class QPushButton;
class GLWidget;

class Window : public QWidget
{
  Q_OBJECT

 public:
  Window();

 protected:
  void keyPressEvent(QKeyEvent *event);

 private:
  GLWidget *glWidget;

  QDoubleSpinBox *q0SpinBox,
                 *q1SpinBox,
                 *q2SpinBox,
                 *q3SpinBox,
                 *q4SpinBox,
                 *RadiusSpinBox,
                 *cam_azimuthSpinBox,
                 *cam_elevationSpinBox,
                 *cam_distanceSpinBox;

    QPushButton *saveGL2PS;
};
#endif
