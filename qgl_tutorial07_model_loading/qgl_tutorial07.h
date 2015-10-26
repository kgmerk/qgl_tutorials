#ifndef QGL_TUTORIAL07_H
#define QGL_TUTORIAL07_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include <QOpenGLTexture>
#include <vector>

class Tutorial07 : public QOpenGLWidget, protected QOpenGLFunctions
{
  public:
    Tutorial07( QWidget *parent=0 );
    ~Tutorial07();

  protected:
    void initializeGL(void);
    void resizeGL( int w, int h );
    void paintGL();
    void keyPressEvent( QKeyEvent *e);
    void timerEvent(QTimerEvent*);

  private:
    QOpenGLShaderProgram shaderProgram;
    QMatrix4x4 MVP;
    QMatrix4x4 view;
    QMatrix4x4 rot;
    QMatrix4x4 model;
    //GLuint mTexture;

    int vertexLocation;
    int matrixLocation;
    int textureLocation;
    int viewMatrixLocation;
    int modelMatrixLocation;
    int vertexUVLocation;
    int vertexNormalLocation;
    int lightLocation;

    GLuint programID;
    GLuint MatrixID;
    GLuint vertexPosition_modelspaceID;
    GLuint vertexUVID;
    GLuint TextureID;
    GLuint vertexbuffer;
    GLuint uvbuffer;
    QOpenGLTexture* mTexture;

    void loadScene(QString filename);
    unsigned int nFaces;
    unsigned int nVertFace;
    //GLfloat* vertexData;
    //GLfloat* texCoord;
    //GLfloat* normals;
    std::vector<GLfloat> vertexData;
    std::vector<GLfloat> texCoord;
    std::vector<GLfloat> normals;
};


#endif // QGL_TUTORIAL07_H
