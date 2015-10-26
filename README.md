
This is a translation from the GLFW window system to Qt5.

The first 7 tutorials have already been translated.

I am using QOpenGLWidget because I want a widget that can be easily embedded
in another window.

The translation process requires the creation of a base class derived
from QGLWidget and implementation of the required methods.  The only methods
that really need to change from the other translations are initializeGL() 
and paintGL().  Basically, the code inside the do while loop goes in the
paintGL() method and the gl code prior to that goes in the initializeGL()
method.   Delete any code that references glfw.  Also, there is typically
a little bit of code that should go in the destructor.

I chose to use the QOpenGLShaderProgram to load the shader files but I 
am using straight OpenGL for the rest of it.  IMO, there are more examples
out there of using OpenGL than there are of using the wrapper methods
in QOpenGLShaderProgram.  This also accomplishes my goal of teaching both
a C++ API and a C style API.

Now, I may reconsider this.  Using QOpenGLTexture may convince me to
use the Qt wrappers.  They are much easier!



