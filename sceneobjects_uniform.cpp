#include "sceneobjects_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"

#define KEY_W                  87
#define KEY_S                  83
#define KEY_A                  65
#define KEY_D                  68
#define KEY_Q                  81
#define KEY_E                  69

#define KEY_1                  49
#define KEY_2                  50
#define KEY_3                  51
#define KEY_4                  52
#define KEY_5                  53
#define KEY_SPACE              32

using glm::vec3;

sceneobjects_uniform::sceneobjects_uniform() : angle(0.0f) {}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 4);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void sceneobjects_uniform::initScene()
{
    glEnable(GL_DEPTH_TEST);

	camera.MovementSpeed *= 10.0f;

	toothless.load("media/models/toothless.obj");
	bird.load("media/models/bird.obj");
	house.load("media/models/House.obj");
	sphere.load("media/models/sphere.obj");
	island.load("media/models/island.obj");
	rock.load("media/models/rock.obj");
	sea.load("media/models/sea.obj");
	dolphin.load("media/models/dolphin.obj");
	table.load("media/models/Plane.obj");

	shader.load("shader/model_loading.vert", "shader/model_loading.frag");
	skyboxShader.load("shader/skybox.vert", "shader/skybox.frag");

    //compile();

    //std::cout << std::endl;

    //prog.printActiveUniforms();

    ///////////////////// Create the VBO ////////////////////
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// skybox VAO
	unsigned int skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// load textures
	// -------------
	vector<std::string> faces
	{
		"media/textures/skybox/CloudyCrown_Midday_Right.png",
		"media/textures/skybox/CloudyCrown_Midday_Left.png",
		"media/textures/skybox/CloudyCrown_Midday_Up.png",
		"media/textures/skybox/CloudyCrown_Midday_Down.png",
		"media/textures/skybox/CloudyCrown_Midday_Front.png",
		"media/textures/skybox/CloudyCrown_Midday_Back.png",
	};
	
	cubemapTexture = loadCubemap(faces);
}

void sceneobjects_uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void sceneobjects_uniform::update( float t )
{
    deltaTime = t;

	boatPosition.x -= 0.1f;

	if (boatPosition.x < -35.0f)
	{
		boatPosition.x = 35.0f;
	}

	//update your angle here
    view = camera.GetViewMatrix();
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);

    auto model = glm::mat4(1.0f);

    shader.use();

    shader.setMat4("model", model);
    shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	shader.setVec3("lightPosition", lightPosition);
	shader.setVec3("movingLightPosition", movingLightPosition);
	shader.setVec3("ambientColor", ambientColor);
	shader.setVec3("lightColor", lightColor);
	shader.setVec3("movingLightColor", movingLightColor);
	shader.setVec3("directionalLightColor", directionalLightColor);
	shader.setVec3("viewPosition", camera.Position);
	shader.setFloat("shininess", shininess);
	shader.setBool("toggleDiffuse", toggleDiffuse);
	shader.setBool("toggleSpecular", toggleSpecular);
	shader.setBool("toggleToonShading", toggleToonShading);
	shader.setBool("toggleSpotLight", toggleSpotLight);
	shader.setBool("toggleFlatShading", toggleFlatShading);
	shader.setBool("toggleFog", toggleFog);

	shader.setVec3("spotLight.position", camera.Position);
	shader.setVec3("spotLight.direction", camera.Front);
	shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

	shader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("spotLight.constant", 1.0f);
	shader.setFloat("spotLight.linear", 0.09f);
	shader.setFloat("spotLight.quadratic", 0.032f);

	if (toggleAnimation)
	{
		auto rx0 = 0.0f;
		auto rz0 = 20.0f;

		auto x = movingLightPosition.x;
		auto z = movingLightPosition.z;

		auto a = glm::radians(movingLightRotation);

		auto x0 = (x - rx0) * std::cos(a) - (z - rz0) * std::sin(a) + rx0;
		auto z0 = (x - rx0) * std::sin(a) + (z - rz0) * std::cos(a) + rz0;

		movingLightPosition.x = x0;
		movingLightPosition.z = z0;
	}
}

void sceneobjects_uniform::drawMovingLight()
{
	auto model = glm::translate(glm::mat4(1.0f), movingLightPosition);
	model = glm::scale(model, { 0.5f, 0.5f, 0.5f });

	shader.setMat4("model", model);

	sphere.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), lightPosition);

	shader.setMat4("model", model);

	sphere.Draw(shader);
}

void sceneobjects_uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //create the rotation matrix here and update the uniform in the shader 
	auto model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));

	shader.setMat4("model", model);

	house.Draw(shader);

	island.Draw(shader);
	rock.Draw(shader);
	sea.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 10.0f));

	model = glm::scale(model, glm::vec3(5.0f));

	shader.setMat4("model", model);

	table.Draw(shader);

	bird.Draw(shader);

	toothless.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), boatPosition);

	model = glm::scale(model, glm::vec3(0.5f));

	shader.setMat4("model", model);

	dolphin.Draw(shader);

	drawMovingLight();

	glBindVertexArray(0);

	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader.use();
	view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", projection);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void sceneobjects_uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
}

void sceneobjects_uniform::onMouseMove(float xOffset, float yOffset)
{
	camera.ProcessMouseMovement(xOffset, yOffset);
}

void sceneobjects_uniform::keyPress(int key)
{
    switch (key)
    {
    case KEY_W:
		camera.ProcessKeyboard(FORWARD, deltaTime);
        break;
	case KEY_S:
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		break;
	case KEY_A:
		camera.ProcessKeyboard(LEFT, deltaTime);
		break;
	case KEY_D:
		camera.ProcessKeyboard(RIGHT, deltaTime);
		break;
	case KEY_Q:
		camera.ProcessKeyboard(DOWN, deltaTime);
		break;
	case KEY_E:
		camera.ProcessKeyboard(UP, deltaTime);
		break;
    default:
        break;
    }
}

void sceneobjects_uniform::keyDown(int key)
{
	switch (key)
	{
	case KEY_1:
		toggleDiffuse = !toggleDiffuse;
		break;
	case KEY_2:
		toggleSpecular = !toggleSpecular;
		break;
	case KEY_3:
		toggleSpotLight = !toggleSpotLight;
		break;
	case KEY_4:
		toggleToonShading = !toggleToonShading;
		break;
	case KEY_5:
		toggleFlatShading = !toggleFlatShading;
		break;
	case KEY_SPACE:
		toggleAnimation = !toggleAnimation;
		break;
	default:
		break;
	}
}
