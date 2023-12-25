#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
in vec3 outNormal;
in vec3 outFragPos;

uniform vec3 viewPos; // 相机位置

// 定义材质结构体
struct Material {
  vec3 ambient; // 环境光 颜色
  //vec3 diffuse; // 漫反射 颜色
  sampler2D diffuse;  //  漫反射贴图
  //vec3 specular; // 高光颜色
  sampler2D specular;
  float shininess; // 高光指数
};
uniform Material material;

// 光源属性
struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform Light light;

uniform vec3 InputObjectColor;

void main() {

  //vec4 objectColor = texture(material.diffuse2, outTexCoord);
  //vec4 objectColor = vec4(InputObjectColor, 1.0f);

  vec3 ambient = light.ambient * vec3(texture(material.diffuse, outTexCoord)); // 环境光

  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(light.position - outFragPos);

  float diff = max(dot(norm, lightDir), 0.0);
  //vec3 diffuse = light.diffuse * diff * material.diffuse; // 漫反射
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, outTexCoord)) ;   // todo 满反射贴图texture

  vec3 viewDir = normalize(viewPos - outFragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  // vec3 specular = light.specular * spec * material.specular; // 镜面光
  vec3 specular = light.specular * spec * vec3(texture(material.specular, outTexCoord)); // todo 镜面光贴图 texture

  //vec3 result = (ambient + diffuse + specular) * vec3(objectColor);
  vec3 result = ambient + diffuse + specular;

  FragColor = vec4(result, 1.0);
}

