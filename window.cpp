#include <QtGui>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <cmath>

#include "glwidget.h"
#include "window.h"

Window::Window()
{
    double PI_OVER_4 = std::atan(1.0);
    double PI_OVER_2 = 2.0*PI_OVER_4;
    double PI = 2.0*PI_OVER_2;

    // Yaw
    q0SpinBox = new QDoubleSpinBox(this);// createSpinBox(0, PI_OVER_2);
    q0SpinBox->setMaximum(PI_OVER_2);
    q0SpinBox->setSingleStep(PI_OVER_2 / 50.0);
    q0SpinBox->setSuffix(" rad");
    // Lean
    q1SpinBox = new QDoubleSpinBox(this);// createSpinBox(0, PI_OVER_2);
    q1SpinBox->setMinimum(-PI_OVER_2);
    q1SpinBox->setMaximum(PI_OVER_2);
    q1SpinBox->setSingleStep(PI_OVER_2 / 50.0);
    q1SpinBox->setSuffix(" rad");
    // Spin
    q2SpinBox = new QDoubleSpinBox(this);
    q2SpinBox->setMinimum(0.0);
    q2SpinBox->setMaximum(PI_OVER_2);
    q2SpinBox->setSingleStep(PI_OVER_2 / 50.0);
    q2SpinBox->setSuffix(" rad");
    // x
    q3SpinBox = new QDoubleSpinBox(this);
    q3SpinBox->setMinimum(0.0);
    q3SpinBox->setMaximum(0.5);
    q3SpinBox->setSingleStep(0.01);
    q3SpinBox->setSuffix(" m");
    // y
    q4SpinBox = new QDoubleSpinBox(this);
    q4SpinBox->setMinimum(0.0);
    q4SpinBox->setMaximum(0.5);
    q4SpinBox->setSingleStep(0.01);
    q4SpinBox->setSuffix(" m");
    // Disc radius
    RadiusSpinBox = new QDoubleSpinBox(this); // createSpinBox(0.0, 0.5);
    RadiusSpinBox->setMinimum(0.0);
    RadiusSpinBox->setMaximum(0.05);
    RadiusSpinBox->setSingleStep(0.01);
    RadiusSpinBox->setSuffix(" m");
    // Camera azimuth
    cam_azimuthSpinBox = new QDoubleSpinBox(this); // createSpinBox(0.0, 0.5);
    cam_azimuthSpinBox->setMinimum(-PI);
    cam_azimuthSpinBox->setMaximum(PI);
    cam_azimuthSpinBox->setSingleStep(PI_OVER_2/50.0);
    cam_azimuthSpinBox->setSuffix(" rad");
    // Camera elevation
    cam_elevationSpinBox = new QDoubleSpinBox(this); // createSpinBox(0.0, PI_OVER_2);
    cam_elevationSpinBox->setMinimum(0.0);
    cam_elevationSpinBox->setMaximum(PI_OVER_2);
    cam_elevationSpinBox->setSingleStep(PI_OVER_2/50.0);
    cam_elevationSpinBox->setSuffix(" rad");
    // Camera distance
    cam_distanceSpinBox = new QDoubleSpinBox(this);  // createSpinBox(0.1, 1.25);
    cam_distanceSpinBox->setMinimum(0.1);
    cam_distanceSpinBox->setMaximum(1.25);
    cam_distanceSpinBox->setSingleStep((1.25-0.1)/50.0);
    cam_distanceSpinBox->setSuffix(" m");

    // Set initial values of all quantities
    q0SpinBox->setValue(0.48);
    q1SpinBox->setValue(0.50);
    q2SpinBox->setValue(PI/3.0);
    q3SpinBox->setValue(0.04);
    q4SpinBox->setValue(0.02);
    RadiusSpinBox->setValue(0.01);
    cam_azimuthSpinBox->setValue(-1.85);
    cam_elevationSpinBox->setValue(0.27);
    cam_distanceSpinBox->setValue(0.75);

    // Create the GLWidget and initialize with correct values
    glWidget = new GLWidget;
    glWidget->Setq0(q0SpinBox->value());
    glWidget->Setq1(q1SpinBox->value());
    glWidget->Setq2(q2SpinBox->value());
    glWidget->Setq3(q3SpinBox->value());
    glWidget->Setq4(q4SpinBox->value());
    glWidget->SetRadius(RadiusSpinBox->value());
    glWidget->SetAzimuth(cam_azimuthSpinBox->value());
    glWidget->SetElevation(cam_elevationSpinBox->value());
    glWidget->SetDistance(cam_distanceSpinBox->value());

    saveGL2PS = new QPushButton("Save GL2PS");

    connect(q0SpinBox, SIGNAL(valueChanged(double)), glWidget, SLOT(Setq0(double)));
    connect(glWidget, SIGNAL(q0Changed(double)), q0SpinBox, SLOT(setValue(double)));

    connect(q1SpinBox, SIGNAL(valueChanged(double)), glWidget, SLOT(Setq1(double)));
    connect(glWidget, SIGNAL(q1Changed(double)), q1SpinBox, SLOT(setValue(double)));

    connect(q2SpinBox, SIGNAL(valueChanged(double)), glWidget, SLOT(Setq2(double)));
    connect(glWidget, SIGNAL(q2Changed(double)), q2SpinBox, SLOT(setValue(double)));

    connect(q3SpinBox, SIGNAL(valueChanged(double)), glWidget, SLOT(Setq3(double)));
    connect(glWidget, SIGNAL(q3Changed(double)), q3SpinBox, SLOT(setValue(double)));

    connect(q4SpinBox, SIGNAL(valueChanged(double)), glWidget, SLOT(Setq4(double)));
    connect(glWidget, SIGNAL(q4Changed(double)), q4SpinBox, SLOT(setValue(double)));

    connect(RadiusSpinBox, SIGNAL(valueChanged(double)), glWidget, SLOT(SetRadius(double)));
    connect(glWidget, SIGNAL(RadiusChanged(double)), RadiusSpinBox, SLOT(setValue(double)));

    connect(cam_azimuthSpinBox, SIGNAL(valueChanged(double)), glWidget, SLOT(SetAzimuth(double)));
    connect(glWidget, SIGNAL(azimuthChanged(double)), cam_azimuthSpinBox, SLOT(setValue(double)));

    connect(cam_elevationSpinBox, SIGNAL(valueChanged(double)), glWidget, SLOT(SetElevation(double)));
    connect(glWidget, SIGNAL(elevationChanged(double)), cam_elevationSpinBox, SLOT(setValue(double)));

    connect(cam_distanceSpinBox, SIGNAL(valueChanged(double)), glWidget, SLOT(SetDistance(double)));
    connect(glWidget, SIGNAL(distanceChanged(double)), cam_distanceSpinBox, SLOT(setValue(double)));

    connect(saveGL2PS, SIGNAL(clicked()), glWidget, SLOT(SaveGL2PS()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(glWidget);
    QFormLayout *spinBoxForms = new QFormLayout;
    spinBoxForms->addRow("Yaw", q0SpinBox);
    spinBoxForms->addRow("Lean", q1SpinBox);
    spinBoxForms->addRow("Spin", q2SpinBox);
    spinBoxForms->addRow("x", q3SpinBox);
    spinBoxForms->addRow("y", q4SpinBox);
    spinBoxForms->addRow("Radius:", RadiusSpinBox);
    spinBoxForms->addRow("Camera Azimuth:", cam_azimuthSpinBox);
    spinBoxForms->addRow("Camera Elevation:", cam_elevationSpinBox);
    spinBoxForms->addRow("Camera Distance:", cam_distanceSpinBox);
    mainLayout->addLayout(spinBoxForms);
    mainLayout->addWidget(saveGL2PS);
    setLayout(mainLayout);
    setWindowTitle(tr("Rolling disc"));
}

void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}
