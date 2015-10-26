#version 120


// Input vertex data, different for all executions of this shader.
attribute vec3 vertexPosition_modelspace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

// vertex color
varying vec3   f_color;
attribute vec3 v_color;

void main()
{
  // Output position of the vertex, in clip space : MVP * position
  gl_Position =  MVP * vec4(vertexPosition_modelspace, 1);
  f_color = v_color;
}

