#version 120

#define highp
#define mediump
#define lowp

// Inputs
attribute highp vec3 vertexPosition_modelspace;
attribute vec2 vertexUV;
attribute vec3 vertexNormal_modelspace;

// Values that stay constant for the whole mesh
uniform highp mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPosition_worldspace;

// Output data
varying vec2 UV;
varying vec3 Position_worldspace;
varying vec3 Normal_cameraspace;
varying vec3 EyeDirection_cameraspace;
varying vec3 LightDirection_cameraspace;

void main(void)
{
   gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

   // Position of the vertex, in worldspace : M * position
   Position_worldspace = vec4(M * vec4(vertexPosition_modelspace, 1)).xyz;

   // Vector that goes from the vertex to the camera, in camera space.
   // In camera space, the camera is at the origin (0, 0, 0).
   vec3 vertexPosition_cameraspace = vec4( V * M * vec4(vertexPosition_modelspace,1)).xyz;
   EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

   // Vector that goes from the vertex to the light, in camera space.
   vec3 LightPosition_cameraspace = vec4(V * M * vec4(LightPosition_worldspace, 1)).xyz;
   LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

   // Normal of the vertex, in camera space
   Normal_cameraspace = vec4(V * M * vec4(vertexNormal_modelspace, 0)).xyz;

   UV = vertexUV;
}
