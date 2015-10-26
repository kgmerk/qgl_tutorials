#include "qgl_tutorial02.h"
#include <QApplication>
#include <QKeyEvent>
#include <QFileInfo>

static const GLfloat g_vertex_buffer_data[] = {
  -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};


Tutorial02::Tutorial02( QWidget *parent ) : QOpenGLWidget(parent)
{
  vertexPosition_modelspaceID = 0;
}

Tutorial02::~Tutorial02()
{
  glDeleteBuffers(1, &vertexbuffer);
}

void Tutorial02::initializeGL(void)
{
  // initialize OpenGL
  initializeOpenGLFunctions();

  // load and compile vertex shader
  bool success;
  success = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/SimpleVertexShader.vert");

  // load and compile fragment shader
  success = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/SimpleFragmentShader.frag");

  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  shaderProgram.link();

  GLuint programID = shaderProgram.programId();

  vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");

  glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
}

void Tutorial02::paintGL(void)
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  shaderProgram.bind();

  GLuint programID = shaderProgram.programId();

  // Use our shader
  glUseProgram(programID);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(
          vertexPosition_modelspaceID, // The attribute we want to configure
          3,                  // size
          GL_FLOAT,           // type
          GL_FALSE,           // normalized?
          0,                  // stride
          (void*)0            // array buffer offset
  );

  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

  glDisableVertexAttribArray(0);
}

void Tutorial02::resizeGL(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void Tutorial02::keyPressEvent(QKeyEvent *e)
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
  QApplication a( argc, argv );

  Tutorial02 w;
  w.show();
  return a.exec();
}

