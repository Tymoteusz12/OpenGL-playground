#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "stb_image.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "WindowInit.cpp"
#include "Shaders.h"
#include "variables.h"
#include "camera.h"
#include "cubeClass.h"
#include "model.h"
using namespace std;
using namespace variables;

Camera mainCamera(width, height, gravityForce);

unsigned int loadCubemap(vector<string> faces) {

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {

		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}

		else {
			cout << "Cubemap tex failed to load at path: " << faces[i] << endl;
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		isFlying = true;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		isFlying = false;

	mainCamera.MoveCameraFunction(window, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	mainCamera.mouse_callback(window, xpos, ypos);
}

int main(void)
{
	glfwInit();

	CreateWindow* object = new CreateWindow;
	GLFWwindow* window = object->window;
	delete object;

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD \n";
		return -1;
	}

	CreateShader* LightShader = new CreateShader("LightVertexShader.glsl", "LightFragmentShader.glsl");
	CreateShader* BlockShader = new CreateShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	CreateShader* rgbaShader = new CreateShader("rgbaVertex.glsl", "rgbaFragment.glsl");
	CreateShader* skyShader = new CreateShader("cubeVertex.glsl", "cubeFragment.glsl");

	model3D* floorObject = new model3D(&mainCamera, BlockShader, gravityForce, frictionForce);
	model3D* cubeObject = new model3D(&mainCamera, BlockShader, gravityForce, frictionForce);
	model3D* lightObject = new model3D(&mainCamera, LightShader, gravityForce, frictionForce);
	model3D* grassObject = new model3D(&mainCamera, rgbaShader, gravityForce, frictionForce);
	floorObject->setVertexAndBuffer(floorVertices, sizeof(floorVertices), 2);
	floorObject->loadTexture("textures/floor.jpg");

	cubeObject->setVertexAndBuffer(blockVertices, sizeof(blockVertices), 2);
	cubeObject->loadTexture("textures/metal.jpg");
	cubeObject->loadTexture("textures/container.png");
	cubeObject->loadTexture("textures/container_specular.png");

	grassObject->setVertexAndBuffer(transparentVertices, sizeof(transparentVertices), 0);
	grassObject->loadTexture("textures/window.png");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	Model ourModel("C:/Users/Tymek/Documents/BlenderObjFiles/sphere.obj");

	unsigned int cubemapTexture = loadCubemap(faces);

	unsigned int skyboxVBO, skyboxVAO;

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)) {
		float xColor = sin(glfwGetTime());
		float yColor = cos(glfwGetTime());
		spotLightVecProperties[0] = mainCamera.cameraPos;
		spotLightVecProperties[1] = mainCamera.cameraFront;
		spotLightVecProperties[26] = glm::vec3(xColor, yColor, xColor * yColor);
		spotLightVecProperties[32] = glm::vec3(xColor, 0.5f, 0.5f);
		dirLightVecProperties[1] = glm::vec3(xColor, yColor * xColor, 1.0f);

		glm::mat4 projection = mainCamera.CreateProjectionMatix(viewAngle, width, height);
		

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window, deltaTime);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorObject->myTextures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, cubeObject->myTextures[0]);
		glActiveTexture(GL_TEXTURE2);	
		glBindTexture(GL_TEXTURE_2D, cubeObject->myTextures[1]);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, cubeObject->myTextures[2]);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, grassObject->myTextures[0]);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		glm::mat4 view = glm::mat4(glm::mat3(mainCamera.CreateViewMatrix()));
		glDepthFunc(GL_LEQUAL);
		skyShader->useProgram();
		skyShader->setMat4("projection", projection);
		skyShader->setMat4("view", view);
		glBindVertexArray(skyboxVAO);
		skyShader->setInt("skybox", 5);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);

		view = mainCamera.CreateViewMatrix();

		BlockShader->useProgram();
		cubeObject->shaderPointer = BlockShader;
		floorObject->setMaterialProperties(0, 32.0f);
		BlockShader->setVec3("viewPos", mainCamera.cameraPos);
		floorObject->setSpotLightProperties(spotLightVecProperties, spotLightFloatProperties, 6); // works either for floor and cubes
		BlockShader->setMat4("projection", projection);
		BlockShader->setMat4("view", view);
		
		floorObject->setModelProperties(floorTranslation, scaleTwo, NULL); //floor

		floorObject->DrawObject(0, 6);

		floorObject->setMaterialProperties(1, 128.0f);
		floorObject->setDirLightProperties(dirLightVecProperties);
		floorObject->setModelProperties(wallTranslation, scaleOne, glm::vec3(1.0f, 0.0f, 0.0f), 90.0f, NULL); //wall
		floorObject->DrawObject(0, 6);

		cubeObject->setMaterialProperties(1, 64.0f);
		for (int i = 0; i < 8; i++) {
			float angle = 20 * i;
			model = cubeObject->setModelProperties(steelCubePositions[i] * correction, scaleTwo, glm::vec3(1.0f, 0.3f, 0.5f), angle, NULL);
			cubeObject->DrawObject(0, 36);
		}

		cubeObject->setMaterialProperties(2, 3, 32.0f);
		for (int i = 0; i < 8; i++) {
			float angle = 30 * i;
			model = cubeObject->setModelProperties(steelCubePositions[i] * correction + translation, scaleThree, glm::vec3(0.5f, 1.0f, 0.3f), angle, NULL);
			cubeObject->DrawObject(0, 36);
			cubeObject->setModelProperties(glm::vec3(4.0f, 2.0f, 5.0f), scaleOne, &model);
			cubeObject->DrawObject(0, 36);
		}

		LightShader->useProgram();
		LightShader->setMat4("projection", projection);
		LightShader->setMat4("view", view);
		cubeObject->shaderPointer = LightShader;

		for (int i = 0; i < 6; i++) {
			glm::vec3 tempScale = lightScale;
			if (i >= 3)
				tempScale = scaleTwo;
			cubeObject->setModelProperties(lightPositions[i], tempScale, NULL);
			cubeObject->DrawObject(0, 36);
		}

		if (!isFlying) {
			testingCube = mainCamera.cameraPos + testingCubePosition*mainCamera.cameraFront;
			throwDirection = mainCamera.cameraFront;
			speedNotSet = true;
		}

		if (isFlying and speedNotSet) {
			speedVec = constSpeed * throwDirection;
			speedNotSet = false;
		}

		BlockShader->useProgram();
		cubeObject->shaderPointer = BlockShader;
		cubeObject->setMaterialProperties(0, 36.0f);
		cubeObject->moveModel(testingCube, scaleTwo, speedVec, deltaTime, isFlying);
		cubeObject->DrawObject(0, 36);

		

		rgbaShader->useProgram();
		for (GLuint i = 0; i < vegetation.size(); i++) {
			rgbaShader->setInt("texture1", 4);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, vegetation[i]);
			rgbaShader->setMat4("projection", projection);
			rgbaShader->setMat4("view", view);
			rgbaShader->setMat4("model", model);
			grassObject->DrawObject(0, 6);
		}
		glUseProgram(0);

		glBindVertexArray(0);

		BlockShader->useProgram();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, 0.0f));
		BlockShader->setMat4("model", model);
		ourModel.Draw(*BlockShader);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}