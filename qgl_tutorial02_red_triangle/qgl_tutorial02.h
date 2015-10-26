#ifndef QGL_TUTORIAL02_H
#define QGL_TUTORIAL02_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

class Tutorial02 : public QOpenGLWidget, protected QOpenGLFunctions
{
  public:
    Tutorial02( QWidget *parent=0 );
    ~Tutorial02();

  protected:
    void initializeGL(void);
    void resizeGL( int w, int h );
    void paintGL();
    void keyPressEvent( QKeyEvent *e);

  private:
    QOpenGLShaderProgram shaderProgram;
    GLuint vertexPosition_modelspaceID;
    GLuint vertexbuffer;
};


#endif // QGL_TUTORIAL02_H

