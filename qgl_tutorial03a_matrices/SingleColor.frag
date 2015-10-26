#version 120

// Ouput data

varying vec3 f_color;
void main()
{
  // Output color = red, full opacity
  gl_FragColor = vec4(f_color.x, f_color.y, f_color.z, 1.0);
}

