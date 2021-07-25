#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "Material.h"
#include "Model.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Skybox.h"
#include "Camera.h"


class RenderManager {
 public:  // variables
  GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0,
         uniformEyePosition = 0, uniformSpecularIntensity = 0,
         uniformShininess = 0, uniformDirectionalLightTransform = 0,
         uniformOmniLightPos = 0, uniformFarPlane = 0;

  // TODO: these variables should be components or objects
  Texture rockTexture;
  Texture asteroidTexture;

  Material shinyMaterial;
  Material dullMaterial;

  Model xwing;
  Model asteroid;
  Model asteroid2;

  DirectionalLight mainLight;
  PointLight pointLights[kMaxPointLights];
  SpotLight spotLights[kMaxSpotLights];

  Skybox skybox;

  unsigned int pointLightCount = 0;
  unsigned int spotLightCount = 0;

  // TODO: this will be part of the asteroid entity
  GLfloat asteroidAngle = 0.0f;

  std::vector<Mesh*> meshList;

  // potentially allows multiple rendering shaders
  std::vector<Shader> shaderList;
  Shader directionalShadowShader;
  Shader omniShadowShader;

  const char* vShader;
  const char* fShader;

  GLfloat buffer_width, buffer_height;
  glm::mat4 projection;

 private:

 public: // Methods
  RenderManager();
  void Initialize(GLfloat buffer_width, GLfloat buffer_height);

  // Phong shading. Average normals of each face of shared vertices
  void calcAverageNormals(unsigned int* indices, unsigned int indiceCount,
      GLfloat* vertices, unsigned int verticeCount, unsigned int vLength,
      unsigned int normalOffset);
  void CreateObjects(); // TODO: move to entity manager
  void CreateShaders();
  void RenderScene(float delta_time);
  void DirectionalShadowMapPass(float delta_time);
  void OmniShadowMapPass(PointLight* light, float delta_time);
  void RenderPass(glm::mat4 view_matrix, Camera& camera, float delta_time);

 private:
};
