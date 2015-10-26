#ifndef QGL_TUTORIAL01_H
#define QGL_TUTORIAL01_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QKeyEvent>

class Tutorial01 : public QOpenGLWidget, protected QOpenGLFunctions
{
  public:
    Tutorial01( QWidget *parent=0 ) : QOpenGLWidget(parent) {}

  protected:
    void initializeGL(void);
    void resizeGL( int w, int h );
    void paintGL();
    void keyPressEvent( QKeyEvent *e);
};

#endif // QGL_TUTORIAL01_H
