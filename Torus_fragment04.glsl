#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
in vec3 outNormal;
in vec3 outFragPos;

uniform vec3 viewPos; // ���λ��

// ������ʽṹ��
struct Material {
  vec3 ambient; // ������ ��ɫ
  //vec3 diffuse; // ������ ��ɫ
  sampler2D diffuse;  //  ��������ͼ
  //vec3 specular; // �߹���ɫ
  sampler2D specular;
  float shininess; // �߹�ָ��
};
uniform Material material;

// ��Դ����
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

  vec3 ambient = light.ambient * vec3(texture(material.diffuse, outTexCoord)); // ������

  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(light.position - outFragPos);

  float diff = max(dot(norm, lightDir), 0.0);
  //vec3 diffuse = light.diffuse * diff * material.diffuse; // ������
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, outTexCoord)) ;   // todo ��������ͼtexture

  vec3 viewDir = normalize(viewPos - outFragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  // vec3 specular = light.specular * spec * material.specular; // �����
  vec3 specular = light.specular * spec * vec3(texture(material.specular, outTexCoord)); // todo �������ͼ texture

  //vec3 result = (ambient + diffuse + specular) * vec3(objectColor);
  vec3 result = ambient + diffuse + specular;

  FragColor = vec4(result, 1.0);
}

