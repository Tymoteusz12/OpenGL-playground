#include <vector>
namespace variables {

    vector<glm::vec3> vegetation
    {
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3(1.5f, 0.0f, 0.51f),
        glm::vec3(0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3(0.5f, 0.0f, -0.6f)
    };

    float transparentVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };

    struct Vertex {
        glm::vec3 positions;
        glm::vec3 normals;
        glm::vec2 texCoords;
    };

    struct verticesState {
        bool textureExists;
        bool normalsExists;
        vector<Vertex> vertexPointer;
    };

    float* pos = NULL;
	float width = 800.0f;
	float height = 600.0f;
	float viewAngle = 45.0f;
	float deltaTime = 0.0f, currentFrame = 0.0f, lastFrame = 0.0f;
    bool isFlying = false;
    bool speedNotSet = true;

	float floorVertices[] = {
		-50.0f, 0.5f,-50.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.0f,
         50.0f, 0.5f,-50.0f, 1.0f, 0.0f,  0.0f,  1.0f, 0.0f,
         50.0f, 0.5f, 50.0f, 1.0f, 1.0f,  0.0f,  1.0f, 0.0f,
         50.0f, 0.5f, 50.0f, 1.0f, 1.0f,  0.0f,  1.0f, 0.0f,
        -50.0f, 0.5f, 50.0f, 0.0f, 1.0f,  0.0f,  1.0f, 0.0f,
        -50.0f, 0.5f,-50.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.0f,
	};

    float blockVertices[] = {

        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, 
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
    };

    //150x(-150), 150z(-150)

    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 gravityForce = glm::vec3(0.0f, -0.4f, 0.0f);
    glm::vec3 frictionForce = glm::vec3(-0.05f, 0.0f, -0.05f);
    glm::vec3 speedVec = glm::vec3(250.0f);
    glm::vec3 constSpeed = glm::vec3(250.0f);
    glm::vec3 testingCubePosition = glm::vec3(5.0f, 3.0f, 5.0f);

    glm::vec3 spotLightVecProperties[] = {
        glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.3f), glm::vec3(1.0f), glm::vec3(1.0f),
        glm::vec3(2.0f, 5.0f, 3.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.3f), glm::vec3(1.0f), glm::vec3(1.0f),
        glm::vec3(11.5f, 1.0f, -25.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.3f), glm::vec3(1.0f), glm::vec3(1.0f),
        glm::vec3(23.0f, 22.0f, -50.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.3f), glm::vec3(1.0f), glm::vec3(3.0f),
        glm::vec3(-17.0f, 32.0f, -70.0f), glm::vec3(0.0f, -1.0f, -1.0f), glm::vec3(1.0f), glm::vec3(0.3f), glm::vec3(1.0f), glm::vec3(3.0f),
        glm::vec3(33.0f, 52.0f, -70.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.5f, 0.5f), glm::vec3(0.3f), glm::vec3(1.0f), glm::vec3(2.0f)
    };

    float spotLightFloatProperties[] = {
        1.0f, 0.01f, 0.003f, glm::cos(glm::radians(20.0f)), glm::cos(glm::radians(25.0f)),
        1.0f, 0.01f, 0.003f, glm::cos(glm::radians(50.0f)), glm::cos(glm::radians(65.0f)),
        1.0f, 0.0f, 0.0f, glm::cos(glm::radians(30.0f)), glm::cos(glm::radians(50.0f)),
        1.0f, 0.0f, 0.0f, glm::cos(glm::radians(30.0f)), glm::cos(glm::radians(50.0f)),
        1.0f, 0.0f, 0.0f, glm::cos(glm::radians(40.0f)), glm::cos(glm::radians(60.0f)),
        1.0f, 0.0f, 0.0f, glm::cos(glm::radians(40.0f)), glm::cos(glm::radians(60.0f))
    };

    glm::vec3 dirLightVecProperties[] = {
        glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f), glm::vec3(0.3f), glm::vec3(0.5f), glm::vec3(2.0f)
    };

    glm::vec3 steelCubePositions[] = {

        glm::vec3(-25.0f, 2.0f, 0.0f),
        glm::vec3(25.0f, -3.0f, 0.0f),
        glm::vec3(-5.0f, -3.0f, 17.0f),
        glm::vec3(-5.0f, 1.0f, -20.0f),
        glm::vec3(5.0f, -2.0f, 10.0f),
        glm::vec3(15.0f, 0.0f, 20.0f),
        glm::vec3(-16.0f, 0.0f, -9.0f),
        glm::vec3(-5.0f, -5.0f, 25.0f),

    };

    glm::vec3 lightPositions[] = {

        glm::vec3(2.0f, 5.0f, 3.0f), glm::vec3(11.5f, 1.0f, -25.0f), glm::vec3(23.0f, 12.0f, -50.0f),
        glm::vec3(-30.0f, 10.0f, -30.0f), glm::vec3(-17.0f, 12.0f, -70.0f), glm::vec3(33.0f, 32.0f, -70.0f),

    };

    glm::vec3 translation = glm::vec3(23.0f, 2.0f, -50.0f);

    glm::vec3 floorTranslation = glm::vec3(0.0f, -5.0f, 0.0f);

    glm::vec3 scaleOne = glm::vec3(1.0f);
    glm::vec3 scaleTwo = glm::vec3(2.0f);
    glm::vec3 scaleThree = glm::vec3(3.0f);
    glm::vec3 lightScale = glm::vec3(3.0f, 1.0f, 2.0f);
    glm::vec3 wallTranslation = glm::vec3(0.0f, 45.0f, -100.0f);

   
    glm::vec3 correction = glm::vec3(0.5f);

    glm::vec3 testingCube = glm::vec3(1.0f);
    glm::vec3 throwDirection = glm::vec3(1.0f);

    vector <string> faces
    {
        "textures/skybox/skybox/right.jpg",
        "textures/skybox/skybox/left.jpg",
        "textures/skybox/skybox/top.jpg",
        "textures/skybox/skybox/bottom.jpg",
        "textures/skybox/skybox/front.jpg",
        "textures/skybox/skybox/back.jpg"
    };

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
    
}
