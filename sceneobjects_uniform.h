#ifndef SCENEOBJECTS_UNIFORM_H
#define SCENEOBJECTS_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include <learnopengl/model.h>
#include <learnopengl/camera.h>

class sceneobjects_uniform : public Scene
{
private:
    GLuint vaoHandle;
    GLSLProgram prog;
    float angle;

    Model house;
    Model sphere;
    Model island;
    Model rock;
    Model sea;
    Model dolphin;
    Model table;
    Model bird;
    Model toothless;
    
    Camera camera{ glm::vec3(0.0f, 10.0, 100.0f) };

    Shader shader;
    Shader skyboxShader;

    float deltaTime = 0.0f;

	glm::vec3 lightPosition{ -20.0f, 20.0f, 20.0f };

	glm::vec3 movingLightPosition{ 3.0f, 5.0f, 15.0f };

	glm::vec3 movingLightColor{ 1.0f, 0.0f, 0.0f };

	glm::vec3 ambientColor{ 0.1f, 0.1f, 0.1f };
	glm::vec3 lightColor{ 1.0f, 1.0f, 1.0f };
	glm::vec3 directionalLightColor{ 1.0f, 1.0f, 1.0f };

	float movingLightRotation = 1.0f;

	float shininess = 128.0f;

	unsigned int skyboxVAO;

    unsigned int cubemapTexture;
    
    glm::vec3 boatPosition = glm::vec3(35.0f, -3.0f, 30.0f);

    bool toggleDiffuse = false;
    bool toggleSpecular = false;
    bool toggleToonShading = false;
	bool toggleSpotLight = false;
	bool toggleFlatShading = false;
	bool toggleFog = false;
    bool toggleAnimation = false;
private:
    void compile();

    void drawMovingLight();
public:
    sceneobjects_uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);

    void onMouseMove(float xOffset, float yOffset) override;

	void keyPress(int key) override;

    void keyDown(int key) override;
};

#endif // SCENEBASIC_UNIFORM_H
