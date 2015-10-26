#include "qgl_tutorial01.h"

#include <QApplication>

void Tutorial01::initializeGL(void)
{
  // initialize OpenGL
  initializeOpenGLFunctions();

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
}

// just clear the screen to start with
void Tutorial01::paintGL(void)
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void Tutorial01::resizeGL(int w, int h)
{
  // just do this to supress warnings.
  // Later the resize method becomes important
  Q_UNUSED(w);
  Q_UNUSED(h);
}

// capture a keypress event in the QGLWindow
void Tutorial01::keyPressEvent(QKeyEvent *e)
{
  switch (e->key())
  {
    case Qt::Key_Escape:  /*  Escape Key */
     exit(0);
      break;
    default:
      break;
  }
  e->accept();  // Don't pass any key events to parent
}

/*  Main Loop
*  Open window with initial window size, title bar,
*  and handle input events.
*/
int main( int argc, char **argv )
{
  QApplication::setColorSpec( QApplication::CustomColor );
  QApplication a( argc, argv );

  Tutorial01 w;
  w.show();
  return a.exec();
}

