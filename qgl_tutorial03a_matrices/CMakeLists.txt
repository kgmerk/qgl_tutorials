project(tutorial03)

cmake_minimum_required(VERSION 2.8.11)

# set this to point to your Qt5 installation
set(CMAKE_PREFIX_PATH "/Users/karl/Qt5/5.1.1/clang_64")

# include the current directory in the include search path
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# automatically run the Qt Meta-Object Compiler (moc)
set(CMAKE_AUTOMOC ON)

# find the required packages
find_package(Qt5Widgets REQUIRED)
find_package(OpenGL REQUIRED)

# define your source and header files
set(SOURCES 
      tutorial03.cpp 
   )

set(HEADERS 
   )

# ceate an executable based on your files
add_executable( tutorial03 ${SOURCES} ${HDRS})

# link to the correct libraries
qt5_use_modules(tutorial03 Widgets Gui OpenGL OpenGLExtensions)
target_link_libraries( tutorial03 ${OPENGL_gl_LIBRARY} 
                                  ${OPENGL_glu_LIBRARY} 
                                  )

