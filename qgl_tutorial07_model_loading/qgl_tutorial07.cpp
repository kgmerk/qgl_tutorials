#include "qgl_tutorial07.h"

// Include standard headers
#include <QApplication>
#include <QOpenGLTexture>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static int timer_interval = 100;        // timer interval (millisec)

Tutorial07::Tutorial07( QWidget *parent ) : QOpenGLWidget(parent)
{
  startTimer( timer_interval );

  // Import the model from a file
  // Note that this could really be loaded from anywhere.
  // The vertices, textures, and normals are stored in this class
  // but could be loaded elsewhere easily.
  loadScene("./suzanne.obj");
}

Tutorial07::~Tutorial07()
{
  // the OpenGL context must be current before deleting the texture
  makeCurrent();
  delete mTexture;
}

// Load a model from a file using the Open Asset Import library.
void Tutorial07::loadScene(QString filename)
{
    // Clear out old data
    if(!vertexData.empty())
        vertexData.clear();

    if(!texCoord.empty())
        texCoord.clear();

    if(!normals.empty())
        normals.clear();

    // Load the obj file
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename.toStdString(),
                  aiProcess_CalcTangentSpace |
                  aiProcess_Triangulate |
                  aiProcess_JoinIdenticalVertices |
                  aiProcess_SortByPType);

    if(!scene)
    {
        std::cout << "Scene import failed." << std::endl;
        return;
    }

    // Allocate memory
    aiMesh* myMesh = scene->mMeshes[0];
    nFaces = myMesh->mNumFaces;
    nVertFace = myMesh->mFaces[0].mNumIndices; // Number of vertices per face

    // Go through each face and save data to arrays for use in opengl
    for(int i = 0; i < nFaces; i++)
    {
        aiFace currFace = myMesh->mFaces[i];
        for(int j = 0; j < nVertFace; j++)
        {
            // Get the vertex data for the current face
            int n = currFace.mIndices[j];
            vertexData.push_back( myMesh->mVertices[n].x );
            vertexData.push_back( myMesh->mVertices[n].y );
            vertexData.push_back( myMesh->mVertices[n].z );

            // Get the vertex normal data
            normals.push_back( myMesh->mNormals[n].x );
            normals.push_back( myMesh->mNormals[n].y );
            normals.push_back( myMesh->mNormals[n].z );

            // Get the texture coordinates for the current face
            texCoord.push_back( myMesh->mTextureCoords[0][n].x );
            texCoord.push_back( myMesh->mTextureCoords[0][n].y );
        }
    }
}

void Tutorial07::initializeGL()
{
  // initialize OpenGL
  initializeOpenGLFunctions();

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);

  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  // Cull triangles which normal is not towards the camera
  glEnable(GL_CULL_FACE);

  bool success;
  // load and compile vertex shader
  success = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                  ":/VertShader.vsh");

  // load and compile fragment shader
  success = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                 ":/FragShader.fsh");

  shaderProgram.link();

  // Get shader variable locations
  vertexLocation = shaderProgram.attributeLocation("vertexPosition_modelspace");
  matrixLocation = shaderProgram.uniformLocation("MVP");
  modelMatrixLocation = shaderProgram.uniformLocation("M");
  vertexUVLocation = shaderProgram.attributeLocation("vertexUV");
  vertexNormalLocation = shaderProgram.attributeLocation("vertexNormal_modelspace");
  textureLocation = shaderProgram.uniformLocation("myTextureSampler");
  lightLocation = shaderProgram.uniformLocation("LightPosition_worldspace");

  // Load the texture Qt methods
  QImage image = QImage(":/uvmap.DDS");
  mTexture = new QOpenGLTexture(image.mirrored());
  mTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
  mTexture->setMagnificationFilter(QOpenGLTexture::Linear);

  //enable texturing
  glEnable(GL_TEXTURE_2D);

  // bind the texture
  mTexture->bind();

}

void Tutorial07::paintGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(vertexData.empty() || texCoord.empty() || normals.empty())
    return;

  // Set up projection and view matrices
  QMatrix4x4 projection;
  projection.perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);

  QVector3D cameraPos(0,3,5);
  QMatrix4x4 view;
  view.lookAt(cameraPos,
      QVector3D(0, 0, 0), // looks at the origin
      QVector3D(0, 1, 0)); // Head is up (set to 0, -1, 0 to look upside down)

  // Get the combined projection modelview matrix
  QMatrix4x4 pmvMatrix = projection * view * rot * model;

  shaderProgram.bind();

  glBindTexture(GL_TEXTURE_2D, 1);

  QVector3D lightPos(4, 4, 4); // Light source

  // Set values that are uniform for all vertices
  shaderProgram.setUniformValue(matrixLocation, pmvMatrix);
  shaderProgram.setUniformValue("M", model);
  shaderProgram.setUniformValue("V", view);
  shaderProgram.setUniformValue(lightLocation,lightPos);

  // Enable the attribute arrays so we can set values
  shaderProgram.enableAttributeArray(vertexLocation);
  shaderProgram.enableAttributeArray(vertexUVLocation);
  shaderProgram.enableAttributeArray(vertexNormalLocation);

  shaderProgram.setAttributeArray(vertexLocation, &vertexData[0], 3);
  shaderProgram.setAttributeArray(vertexUVLocation, &texCoord[0], 2);
  shaderProgram.setAttributeArray(vertexNormalLocation, &normals[0], 3);

  // Draw the triangle mesh
  glDrawArrays(GL_TRIANGLES, 0, nFaces*3);

  // Disable the arrays and release the shaders
  shaderProgram.disableAttributeArray(vertexLocation);
  shaderProgram.disableAttributeArray(vertexUVLocation);
  shaderProgram.disableAttributeArray(vertexNormalLocation);
  shaderProgram.release();
}

void Tutorial07::timerEvent(QTimerEvent*)
{
    static float rotval = 4.0;
    rot.rotate(rotval, 1.0, 0.0, 0.0);
    update();
}

void Tutorial07::resizeGL(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void Tutorial07::keyPressEvent(QKeyEvent *e)
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

  Tutorial07 w;
  w.show();
  return a.exec();
}


