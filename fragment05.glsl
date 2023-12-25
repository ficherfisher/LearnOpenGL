#version 330 core
out vec4 FragColor;

in vec2 outTexCoord;
in vec3 outNormal;
in vec3 outFragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
//uniform sampler2D texture_normal1;

uniform vec3 viewPos; // 相机位置

// 光源属性
struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform Light light;



void main() {

  // vec4 objectColor = texture(material.texture_specular1, outTexCoord);
  float shininess = 64.0f;

  vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, outTexCoord)); // 环境光

  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(light.position - outFragPos);

  float diff = max(dot(norm, lightDir), 0.0);
  //vec3 diffuse = light.diffuse * diff * material.diffuse; // 漫反射
  vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, outTexCoord)) ;   // todo 满反射贴图texture

  vec3 viewDir = normalize(viewPos - outFragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  // vec3 specular = light.specular * spec * material.specular; // 镜面光
  vec3 specular = light.specular * spec * vec3(texture(texture_specular1, outTexCoord)); // todo 镜面光贴图 texture

  //vec3 result = (ambient + diffuse + specular) * vec3(objectColor);
  vec3 result = ambient + diffuse + specular;

  FragColor = vec4(result, 1.0);
  // mix(texture(texture_diffuse1, outTexCoord), texture(texture_specular1, outTexCoord), 0.5); 
  // vec4(result, 1.0);
}