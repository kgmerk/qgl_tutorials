#ifndef QGL_TUTORIAL05_H
#define QGL_TUTORIAL05_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QKeyEvent>

class QOpenGLTexture;

class Tutorial05 : public QOpenGLWidget, protected QOpenGLFunctions
{
  public:
    Tutorial05( QWidget *parent=0 );
    ~Tutorial05();

  protected:
    void initializeGL(void);
    void resizeGL( int w, int h );
    void paintGL();
    void keyPressEvent( QKeyEvent *e);
    void timerEvent(QTimerEvent*);

  private:
    QOpenGLShaderProgram shaderProgram;
    QMatrix4x4 MVP;
    QOpenGLTexture* mTexture;
    GLuint programID;
    GLuint MatrixID;
    GLuint vertexPosition_modelspaceID;
    GLuint vertexUVID;
    GLuint TextureID;
    GLuint vertexbuffer;
    GLuint uvbuffer;
};


#endif // QGL_TUTORIAL05_H
