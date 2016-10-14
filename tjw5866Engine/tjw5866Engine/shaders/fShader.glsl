#version 430

const vec3 lightPos = vec3(0.0,0.0,1.0);
const vec3 ambientColor = vec3(0.2, 0.2, 0.2);
const vec3 diffuseColor = vec3(0.65, 0.65, 0.65);
const vec3 specColor = vec3(0.8, 0.8, 0.8);
const float shininess = 16.0;
const float screenGamma = 2.2; // Assume the monitor is calibrated to the sRGB color space
in vec3 fragPos;
in vec2 fragUV;
in vec3 camPos;
uniform sampler2D myTexture;
varying vec3 normalInterp;

void main(void)
{
  vec3 normal = normalize(normalInterp);
  vec3 lightDir = normalize(camPos - fragPos);
  vec3 viewDir = normalize(lightPos - fragPos);
  vec3 halfDir = normalize(lightDir + viewDir);
  float specular = 0.0;

  float lambertian = max(dot(lightDir, normal), 0.0);
  float specAngle = max(dot(halfDir, normal), 0);
  specular = pow(specAngle, shininess);

  vec3 colorLinear = ambientColor + lambertian * diffuseColor + specular * specColor;
  // apply gamma correction (assume ambientColor, diffuseColor and specColor
  // have been linearized, i.e. have no gamma correction in them)
  vec3 colorGammaCorrected = pow(colorLinear, vec3(1.0/screenGamma));

  vec4 color = texture(myTexture, fragUV);

  gl_FragColor = vec4(colorGammaCorrected * color.rgb, color.a);
}