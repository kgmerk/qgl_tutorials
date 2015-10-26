#include "qgl_tutorial05.h"

// Include standard headers
#include <QApplication>
#include <QOpenGLTexture>

static int timer_interval = 100;        // timer interval (millisec)

// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
static const GLfloat g_vertex_buffer_data[] = {
  -1.0f,-1.0f,-1.0f,
  -1.0f,-1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,
   1.0f, 1.0f,-1.0f,
  -1.0f,-1.0f,-1.0f,
  -1.0f, 1.0f,-1.0f,
   1.0f,-1.0f, 1.0f,
  -1.0f,-1.0f,-1.0f,
   1.0f,-1.0f,-1.0f,
   1.0f, 1.0f,-1.0f,
   1.0f,-1.0f,-1.0f,
  -1.0f,-1.0f,-1.0f,
  -1.0f,-1.0f,-1.0f,
  -1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f,-1.0f,
   1.0f,-1.0f, 1.0f,
  -1.0f,-1.0f, 1.0f,
  -1.0f,-1.0f,-1.0f,
  -1.0f, 1.0f, 1.0f,
  -1.0f,-1.0f, 1.0f,
   1.0f,-1.0f, 1.0f,
   1.0f, 1.0f, 1.0f,
   1.0f,-1.0f,-1.0f,
   1.0f, 1.0f,-1.0f,
   1.0f,-1.0f,-1.0f,
   1.0f, 1.0f, 1.0f,
   1.0f,-1.0f, 1.0f,
   1.0f, 1.0f, 1.0f,
   1.0f, 1.0f,-1.0f,
  -1.0f, 1.0f,-1.0f,
   1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f,-1.0f,
  -1.0f, 1.0f, 1.0f,
   1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,
   1.0f,-1.0f, 1.0f
};

// Two UV coordinatesfor each vertex. They were created withe Blender.
static const GLfloat g_uv_buffer_data[] = {
  0.000059f, 1.0f-0.000004f,
  0.000103f, 1.0f-0.336048f,
  0.335973f, 1.0f-0.335903f,
  1.000023f, 1.0f-0.000013f,
  0.667979f, 1.0f-0.335851f,
  0.999958f, 1.0f-0.336064f,
  0.667979f, 1.0f-0.335851f,
  0.336024f, 1.0f-0.671877f,
  0.667969f, 1.0f-0.671889f,
  1.000023f, 1.0f-0.000013f,
  0.668104f, 1.0f-0.000013f,
  0.667979f, 1.0f-0.335851f,
  0.000059f, 1.0f-0.000004f,
  0.335973f, 1.0f-0.335903f,
  0.336098f, 1.0f-0.000071f,
  0.667979f, 1.0f-0.335851f,
  0.335973f, 1.0f-0.335903f,
  0.336024f, 1.0f-0.671877f,
  1.000004f, 1.0f-0.671847f,
  0.999958f, 1.0f-0.336064f,
  0.667979f, 1.0f-0.335851f,
  0.668104f, 1.0f-0.000013f,
  0.335973f, 1.0f-0.335903f,
  0.667979f, 1.0f-0.335851f,
  0.335973f, 1.0f-0.335903f,
  0.668104f, 1.0f-0.000013f,
  0.336098f, 1.0f-0.000071f,
  0.000103f, 1.0f-0.336048f,
  0.000004f, 1.0f-0.671870f,
  0.336024f, 1.0f-0.671877f,
  0.000103f, 1.0f-0.336048f,
  0.336024f, 1.0f-0.671877f,
  0.335973f, 1.0f-0.335903f,
  0.667969f, 1.0f-0.671889f,
  1.000004f, 1.0f-0.671847f,
  0.667979f, 1.0f-0.335851f
};


Tutorial05::Tutorial05( QWidget *parent ) : QOpenGLWidget(parent)
{
  startTimer( timer_interval );
}

Tutorial05::~Tutorial05()
{
  // Cleanup VBO and shader
  makeCurrent();
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &uvbuffer);
  glDeleteProgram(programID);
  delete mTexture;
}

void Tutorial05::initializeGL()
{
  // initialize OpenGL
  initializeOpenGLFunctions();

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);

  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  bool success;
  // load and compile vertex shader
  success = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                  ":/TransformVertexShader.vert");

  // load and compile fragment shader
  success = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                 ":/TextureFragmentShader.frag");

  programID = shaderProgram.programId();

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

  shaderProgram.link();

  // Get a handle for our "MVP" uniform
  MatrixID = glGetUniformLocation(programID, "MVP");

  // Get a handle for our buffers
  vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
  vertexUVID = glGetAttribLocation(programID, "vertexUV");

  // Load the texture Qt methods
  QImage image = QImage(":/uvtemplate.bmp").mirrored();
  mTexture = new QOpenGLTexture(image);
  mTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
  mTexture->setMagnificationFilter(QOpenGLTexture::Linear);
  shaderProgram.setUniformValue("myTextureSampler", 0);

  //enable texturing
  glEnable(GL_TEXTURE_2D);
  //generate textures

  //bind the texture
  mTexture->bind();

  // ... nice trilinear filtering.
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  //glGenerateMipmap(GL_TEXTURE_2D);

  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  glGenBuffers(1, &uvbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

}

void Tutorial05::paintGL(void)
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    glUseProgram(programID);

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, MVP.data());

    // Bind our texture in Texture Unit 0
    mTexture->bind(0);
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(TextureID, 0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
      vertexPosition_modelspaceID,  // The attribute we want to configure
      3,                            // size
      GL_FLOAT,                     // type
      GL_FALSE,                     // normalized?
      0,                            // stride
      (void*)0                      // array buffer offset
    );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
      vertexUVID,                   // The attribute we want to configure
      2,                            // size : U+V => 2
      GL_FLOAT,                     // type
      GL_FALSE,                     // normalized?
      0,                            // stride
      (void*)0                      // array buffer offset
    );

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 12*3); // From index 0 to 12*3 -> 12 triangles

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

}

void Tutorial05::timerEvent(QTimerEvent*)
{
    static float rotval = 4.0;
    MVP.rotate(rotval, 1.0, 0.0, 0.0);
    update();
}

void Tutorial05::resizeGL(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void Tutorial05::keyPressEvent(QKeyEvent *e)
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

  Tutorial05 w;
  w.show();
  return a.exec();
}


