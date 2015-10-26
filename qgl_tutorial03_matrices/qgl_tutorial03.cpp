#include "qgl_tutorial03.h"
#include <iostream>

#include <QApplication>
#include <QOpenGLShaderProgram>

static int timer_interval = 100;        // timer interval (millisec)
static const GLfloat g_vertex_buffer_data[] = {
  -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

static const GLfloat triangle_colors[] = {
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
};

Tutorial03::Tutorial03( QWidget *parent ) : QOpenGLWidget(parent)
{
  startTimer( timer_interval );
}

Tutorial03::~Tutorial03()
{
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &colorbuffer);
}

void Tutorial03::initializeGL(void)
{
  // initialize OpenGL
  initializeOpenGLFunctions();

  bool success;
  // load and compile vertex shader
  success = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/SimpleTransform.vert");

  // load and compile fragment shader
  success = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/SingleColor.frag");

  // link the shader program
  shaderProgram.link();

  GLuint programID = shaderProgram.programId();

  // Get a handle for our buffers
  vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");

  // Get a handle to the colors
  color_location = glGetAttribLocation(programID, "v_color");

  // Get a handle for our "MVP" uniform
  MatrixID = glGetUniformLocation(programID, "MVP");

  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  QMatrix4x4 projection;
  projection.perspective(45.0, 4.0/3.0, 0.1, 100.0);

  // Camera matrix
  QMatrix4x4 view;
  view.lookAt( QVector3D(4,3,3), // Camera is at (4,3,3), in World Space
               QVector3D(0,0,0), // and looks at the origin
               QVector3D(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
             );
  // Model matrix : an identity matrix (model will be at the origin)
  QMatrix4x4 model;
  model.setToIdentity();

  // Our ModelViewProjection : multiplication of our 3 matrices
  MVP =  projection * view * model; // Remember, matrix multiplication is the other way around

  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

//  glGenBuffers(1, &colorbuffer);
//  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
//  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_colors), triangle_colors, GL_STATIC_DRAW);

  glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
}

void Tutorial03::paintGL(void)
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  shaderProgram.bind();
  GLuint programID = shaderProgram.programId();

  glUseProgram(programID);

  // Send our transformation to the currently bound shader,
  // in the "MVP" uniform
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, MVP.data());

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
/*
  // 2nd attribute buffer : color
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glVertexAttribPointer(
    color_location, // The attribute we want to configure
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );
*/
  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

  shaderProgram.release();
}


void Tutorial03::timerEvent(QTimerEvent*)
{
    static float rotval = 4.0;
    MVP.rotate(rotval, 1.0, 0.0, 0.0);
    updateGL();
}

void Tutorial03::resizeGL(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void Tutorial03::keyPressEvent(QKeyEvent *e)
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

  Tutorial03 w;
  w.show();
  return a.exec();
}
