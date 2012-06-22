VPATH += ../shared
INCLUDEPATH += ../shared

HEADERS       = glwidget.h \
                window.h
SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp
QT           += opengl

# install
sources.files = $$SOURCES $$HEADERS rolling_disc_gui.pro

unix: LIBS += -lgl2ps -lglut -lGLU
