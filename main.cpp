//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "sphere.h"
#include "Cylinder1.h"
#include "Sphere2.h"
#include "cube.h"
#include "stb_image.h"

#include <iostream>


using namespace std;


glm::mat4 model_sph1 = glm::mat4(1.0f);


using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void shop(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);

void drawCurveObj(unsigned int& bezierVAO_, Shader& lightingShader, glm::mat4 model, float r, float g, float b, float shine);
long long nCr(int n, int r);
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L);
unsigned int hollowBezier(GLfloat ctrlpoints[], int L, vector <float>& coordinates,
    vector <float>& normals,
    vector <int>& indices,
    vector <float>& vertices);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Point For curves 
vector <float> lampPoints = 
{
-0.4750, 1.8800, 5.1000,
-0.3800, 1.8150, 5.1000,
-0.3400, 1.7100, 5.1000,
-0.3700, 1.6200, 5.1000,
-0.4250, 1.5450, 5.1000,
-0.4950, 1.4500, 5.1000,
-0.5100, 1.3450, 5.1000,
-0.4900, 1.2500, 5.1000,
-0.4550, 1.1950, 5.1000,
-0.4200, 1.1500, 5.1000,
-0.3550, 1.1450, 5.1000,
-0.2700, 1.1400, 5.1000,
-0.1350, 1.1550, 5.1000,
-0.0350, 1.1600, 5.1000,
0.0250, 1.2000, 5.1000,
0.0600, 1.2550, 5.1000,
0.0250, 1.2800, 5.1000,
-0.0350, 1.3500, 5.1000,
-0.0750, 1.4200, 5.1000,
-0.0900, 1.5000, 5.1000,
-0.1150, 1.5550, 5.1000,
-0.1250, 1.6250, 5.1000,
-0.1250, 1.6700, 5.1000,
-0.1100, 1.7150, 5.1000,
-0.0900, 1.7850, 5.1000,
-0.0700, 1.8600, 5.1000,
-0.0500, 1.9150, 5.1000,
-0.0500, 1.9400, 5.1000,
-0.1000, 1.9050, 5.1000,
-0.1450, 1.8700, 5.1000,
-0.1850, 1.8650, 5.1000,
-0.3000, 1.8650, 5.1000,
-0.3500, 1.8700, 5.1000,
-0.3950, 1.8900, 5.1000,
-0.4200, 1.9000, 5.1000,
-0.4300, 1.9050, 5.1000,
-0.4750, 1.9050, 5.1000,

};




vector <float> veg =
{

-0.5900, 1.8600, 5.1000,
-0.4800, 1.8700, 5.1000,
-0.3300, 1.8800, 5.1000,
-0.2700, 1.8400, 5.1000,
-0.1950, 1.6900, 5.1000,
-0.1650, 1.6750, 5.1000,
-0.0150, 1.7550, 5.1000,
0.0400, 1.8300, 5.1000,
0.0950, 1.8800, 5.1000,
0.2150, 1.8900, 5.1000,
0.2450, 1.8900, 5.1000,
0.2750, 1.8400, 5.1000,
0.2750, 1.7050, 5.1000,
0.2750, 1.5750, 5.1000,
0.2300, 1.3700, 5.1000,
0.1500, 1.2500, 5.1000,
0.0500, 1.1900, 5.1000,
-0.0950, 1.1950, 5.1000,
-0.1950, 1.2550, 5.1000,
-0.4250, 1.5150, 5.1000,
-0.5100, 1.7050, 5.1000,
-0.6000, 1.8300, 5.1000,
-0.6100, 1.8450, 5.1000,


};

vector <float> lampCoordinates;
vector <float> lampNormals;
vector <int> lampIndices;
vector <float> lampVertices;

vector <float> vegCoordinates;
vector <float> vegNormals;
vector <int> vegIndices;
vector <float> vegVertices;

class point
{
public:
    point()
    {
        x = 0;
        y = 0;
    }
    int x;
    int y;
} clkpt[2];

const double pi = 3.14159265389;
const int nt = 40;
const int ntheta = 20;

bool loadBezierCurvePoints = true;
bool showHollowBezier = true;
bool lineMode = false;
unsigned int bezierVAO;
unsigned int bezierVAO1;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
bool FAN = false;

// camera
Camera camera(glm::vec3(0.0f, -0.2f, 5.2f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(-3.0f, 6.0,  0.19f),
    glm::vec3(6.0f,  6.0f,  13.0f),
    glm::vec3(5.8f,  11.0f,  5.19f),
    glm::vec3(-2.14f, 10.01f,  6.2f)
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);


// light settings
bool pointLightOn = true;
bool pointLight23On = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool directionalToggle = true;
bool spotToggle = true;
bool vard = true;
bool vars = true;
float ghurano = 0.0f;
float ghurano_z = 0.0f;
float ghurano1 = 0.0f;
float ghurano2 = 0.0f;
unsigned int cubeVAO, cubeVBO, cubeEBO, VAO2, VBO2, VAO3, VBO3;
// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;


glm::mat4 transforamtion(float tx, float ty, float tz, float sx, float sy, float sz) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
    model = translateMatrix * scaleMatrix;
    return model;
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {


         0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f

        
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };


    float radius = 0.5f;
    float height = 1.0f;
    int numSegments = 50;  // Number of segments to approximate the circle

    // Calculate the angle between each segment
    float angleIncrement = (2 * glm::pi<float>()) / numSegments;

 
    std::vector<GLfloat> vertices, unitCircle, cyl;

    for (int i = 0; i <= numSegments; i++) {
        float theta = i * 2.0f * glm::pi<float>() / numSegments;
        float x1 = radius * std::cos(theta);
        float z1 = radius * std::sin(theta);

        float cylx1 = 0.3 * std::cos(theta);
        float cylz1 = 0.3 * std::sin(theta);


        float y1 = 0.0f;
        float y2 = 0.5f;
        float y3 = 0.3f;
        unitCircle.push_back(x1); unitCircle.push_back(0.3f); unitCircle.push_back(z1);
        // Define vertices for the triangle strip
        vertices.push_back(x1); vertices.push_back(y1); vertices.push_back(z1);
        vertices.push_back(x1); vertices.push_back(y2); vertices.push_back(z1);
        cyl.push_back(x1); cyl.push_back(y2); cyl.push_back(z1);
        cyl.push_back(cylx1); cyl.push_back(y1); cyl.push_back(cylz1);

    }




    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, unitCircle.size() * sizeof(GLfloat), unitCircle.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);

    glBindVertexArray(VAO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, cyl.size() * sizeof(GLfloat), cyl.data(), GL_STATIC_DRAW);

    // Configure vertex attribute pointers
    // Assuming each vertex has position data (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);





    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Sphere sphere = Sphere();
    float rotate = 0.0f;

    cout << "PLEASE PRESS THE MENTIONED KEYS FOR FOLLOWING OPERATIONS. "<<endl;
    cout << "1 --> Directional Light On and Off" << endl;
    cout << "2 --> Point Light 1 and 3 On and Off" << endl;
    cout << "3 --> Spot Light On and Off" << endl;
    cout << "4 --> Ambient Light On and Off" << endl;
    cout << "5 --> Diffuse Reflection On and Off" << endl;
    cout << "6 --> Specular Reflection On and Off" << endl;
    cout << "7 --> Point Light 2 and 4 On and Off" << endl;
    cout <<"X --> Rotate according to X axis" << endl;
    cout <<"Y --> Rotate according to Y axis" << endl;
    cout << "Z --> Rotate according to Z axis" << endl;
    cout << endl;
    cout << "FOR BASIC CAMERA:" << endl;
    cout << "H--> eyeX+" << endl;
    cout << "F--> eyeX-" << endl;
    cout << "T--> eyeZ+" << endl;
    cout << "G--> eyeZ-" << endl;
    cout << "Q--> eyeY+" << endl;
    cout << "E--> eyeY-+" << endl;
    cout << endl;
    cout << "FOR CAMERA:" << endl;
    cout << "S--> Up" << endl;
    cout << "R--> Down" << endl;
    cout << "Y--> YAW_L" << endl;
    cout << "U--> YAW_R" << endl;
    cout << "Z--> ROLL_L" << endl;
    cout << "C--> ROLL_R" << endl;
    cout << "W--> PITCH_L" << endl;
    cout << "V--> PITCH_R" << endl;





    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
   

    string diffuseMapPath = "tiles.jpg";
    string specularMapPath = "tiles.jpg";


    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string droadpath = "frontwall.jpg";
    string sroadpath = "frontwall.jpg";
    unsigned int droad = loadTexture(droadpath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int sroad = loadTexture(sroadpath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wall = Cube(droad, sroad, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string droadpath17 = "rightwall.jpg";
    string sroadpath17 = "rightwall.jpg";
    unsigned int droad17 = loadTexture(droadpath17.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int sroad17 = loadTexture(sroadpath17.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube r_wall = Cube(droad17, sroad17, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    
    string droadpath18 = "rightwall.jpg";
    string sroadpath18 = "rightwall.jpg";
    unsigned int droad18 = loadTexture(droadpath18.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int sroad18 = loadTexture(sroadpath18.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube l_wall = Cube(droad18, sroad18, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string droadpath19 = "upor_fr.png";
    string sroadpath19 = "upor_fr.png";
    unsigned int droad19 = loadTexture(droadpath19.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int sroad19 = loadTexture(sroadpath19.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube fr_wall = Cube(droad19, sroad19, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    string droadpath20 = "haha.png";
    string sroadpath20 = "haha.png";
    unsigned int droad20 = loadTexture(droadpath20.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int sroad20 = loadTexture(sroadpath20.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube fr_rg_wall = Cube(droad20, sroad20, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string droadpath21 = "haha.png";
    string sroadpath21 = "haha.png";
    unsigned int droad21 = loadTexture(droadpath21.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int sroad21 = loadTexture(sroadpath21.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube fr_lf_wall = Cube(droad21, sroad21, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string droadpath22 = "wood.png";
    string sroadpath22 = "wood.png";
    unsigned int droad22 = loadTexture(droadpath22.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int sroad22 = loadTexture(sroadpath22.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wood = Cube(droad22, sroad22, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string droadpath1 = "fridge.png";
    string sroadpath1 = "fridge.png";
    unsigned int droad1 = loadTexture(droadpath1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int sroad1 = loadTexture(sroadpath1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube fridge1 = Cube(droad1, sroad1, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string droadpath5 = "chocos.jpg";
    string sroadpath5 = "chocos.jpg";
    unsigned int droad5 = loadTexture(droadpath5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int sroad5 = loadTexture(sroadpath5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube choco = Cube(droad5, sroad5, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string droadpath6 = "chocobar.png";
    string sroadpath6 = "chocobar.png";
    unsigned int droad6 = loadTexture(droadpath6.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int sroad6 = loadTexture(sroadpath6.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube chocobar = Cube(droad6, sroad6, 30.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string droadpath7 = "sarre.jpg";
    string sroadpath7 = "sarre.jpg";
    unsigned int droad7 = loadTexture(droadpath7.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int sroad7 = loadTexture(sroadpath7.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube saree = Cube(droad7, sroad7, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string droadpath70 = "trial.png";
    string sroadpath70 = "trial.png";
    unsigned int droad70 = loadTexture(droadpath70.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int sroad70 = loadTexture(sroadpath70.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube trial = Cube(droad70, sroad70, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);



    
    string diffuseMapPath3 = "cake1.jpg";
    string specularMapPath3 = "cake1.jpg";

    unsigned int diffMap3 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap3 = loadTexture(specularMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2  sphere3 = Sphere2(1.0f, 144, 72, glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffMap3, specMap3, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath2 = "cake.jpg";
    string specularMapPath2 = "cake.jpg";

    unsigned int diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2  sphere2 = Sphere2(1.0f, 144, 72, glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffMap2, specMap2, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath4 = "yes.png";
    string specularMapPath4 = "yes.png";

    unsigned int diffMap4 = loadTexture(diffuseMapPath4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap4 = loadTexture(specularMapPath4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2  sphere41 = Sphere2(1.0f, 144, 72, glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffMap4, specMap4, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath5 = "sarre.jpg";
    string specularMapPath5 = "sarre.jpg";

    unsigned int diffMap5 = loadTexture(diffuseMapPath5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap5 = loadTexture(specularMapPath5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2  sphere42 = Sphere2(1.0f, 144, 72, glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffMap5, specMap5, 0.0f, 0.0f, 1.0f, 1.0f);
    

    //curve vao,vbo,veo creation by hallowBazier
    bezierVAO = hollowBezier(lampPoints.data(), ((unsigned int)lampPoints.size() / 3) - 1, lampCoordinates, lampNormals, lampIndices, lampVertices);
    loadBezierCurvePoints = false;
    showHollowBezier = true;

    bezierVAO1 = hollowBezier(veg.data(), ((unsigned int)veg.size() / 3) - 1, vegCoordinates, vegNormals, vegIndices, vegVertices);
    loadBezierCurvePoints = false;
    showHollowBezier = true;

    //cylinder
    Cylinder1 cylinder(0.9, 0.9, 30, 16, 26, "cupcake.png");
    Cylinder1 cylinder2(0.9, 0.9, 55, 16, 26, "coke.png");
    Cylinder1 cylinder3(0.9, 0.9, 55, 16, 26, "cof.png");
    Cylinder1 cylinder4(0.9, 0.9, 55, 16, 26, "cake2.jpg");
    Cylinder1 cylinder5(0.9, 0.9, 55, 16, 26, "chips.jpg");
    Cylinder1 cylinder6(0.9, 0.9, 55, 16, 26, "fanta.jpg");
    Cylinder1 cylinder7(0.9, 0.9, 55, 16, 26, "7up.jpg");
    Cylinder1 cylinder9(0.9, 0.9, 55, 16, 26, "thala.png");
    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
       // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        //-6.5f, 6.0f, 2.25f
        lightingShader.setVec3("diectionalLight.directiaon", 0.0f, 6.0f, 2.5f);
        lightingShader.setVec3("diectionalLight.ambient", .25, .25, .25);
        lightingShader.setVec3("diectionalLight.diffuse", .75f, .75f, .75f);
        lightingShader.setVec3("diectionalLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setBool("dlighton", vard);


        lightingShader.setVec3("spotlight.position", 3.5, 6.0f, 0.25);
        lightingShader.setVec3("spotlight.direction", 0, -1, 0);
        lightingShader.setVec3("spotlight.ambient", .25, .25, .25);
        lightingShader.setVec3("spotlight.diffuse", .75f, .75f, .75f);
        lightingShader.setVec3("spotlight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotlight.k_c", 1.0f);
        lightingShader.setFloat("spotlight.k_l", 0.09);
        lightingShader.setFloat("spotlight.k_q", 0.04);
        lightingShader.setFloat("cos_theta", glm::cos(glm::radians(5.5f)));
        lightingShader.setBool("spotlighton", vars);

       lightingShader.setVec3("spotlight.position", -6.5f, 6.0f, 2.25f);
        lightingShader.setVec3("spotlight.direction", 0, -1, 0);
        lightingShader.setVec3("spotlight.ambient", .25, .25, .25);
        lightingShader.setVec3("spotlight.diffuse", .75f, .75f, .75f);
        lightingShader.setVec3("spotlight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotlight.k_c", 1.0f);
        lightingShader.setFloat("spotlight.k_l", 0.09);
        lightingShader.setFloat("spotlight.k_q", 0.04);
        lightingShader.setFloat("cos_theta", glm::cos(glm::radians(5.5f)));
        lightingShader.setBool("spotlighton", vars);

        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        //glm::mat4 view = camera.GetViewMatrix();
       // glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);

        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        shop(cubeVAO, lightingShader, model);



        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5 * 7.0f, 0.05f, 0.5 * 8.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5f, 0.4f, 2.0f));
        glm::mat4 modelcarpet = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", modelcarpet);
        // Bind the VAOtthtthhth

        glBindVertexArray(VAO2);
        // Draw the cylinder
        //glDrawArrays(GL_TRIANGLE_FAN, 0, unitCircle.size() / 3);

        // Unbind the VAO
        glBindVertexArray(0);




        glm::mat4 modelForSphere = glm::mat4(1.0f);
        modelForSphere = glm::translate(model, glm::vec3(1.5f, 1.2f, 0.5f));
        //sphere.drawSphere(lightingShader, modelForSphere);

        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
           // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }





        // Curves
        lightingShader.use();

        for (float i = 1.0f; i < 5.0f; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.1f + (i*0.55f), 0.45f, -2.0f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f)); //
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.78f, 0.5f, 1.0f));
            glm::mat4 model_for_half_sphere = translateMatrix * rotateZMatrix * rotateXMatrix * rotateYMatrix * scaleMatrix;

            //drawCurveObj(bezierVAO, lightingShader, model_for_pointlight, 1.0f, 1.0f, 1.0f, 1.0f);
            drawCurveObj(bezierVAO, lightingShader, model_for_half_sphere, 0.67f, 0.66f, 0.68f, 2.0f);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.1f + (i * 0.55f), 2.45f, -2.0f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f)); //
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.78f, 0.5f, 1.0f));
            model_for_half_sphere = translateMatrix * rotateZMatrix * rotateXMatrix * rotateYMatrix * scaleMatrix;

            //drawCurveObj(bezierVAO, lightingShader, model_for_pointlight, 1.0f, 1.0f, 1.0f, 1.0f);
            drawCurveObj(bezierVAO, lightingShader, model_for_half_sphere, 0.67f, 0.66f, 0.68f, 2.0f);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.1f + (i * 0.55f), 1.45f, -2.0f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f)); //
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.78f, 0.5f, 1.0f));
            glm::mat4 model_1 = translateMatrix * rotateZMatrix * rotateXMatrix * rotateYMatrix * scaleMatrix;

            //drawCurveObj(bezierVAO, lightingShader, model_for_pointlight, 1.0f, 1.0f, 1.0f, 1.0f);
            drawCurveObj(bezierVAO1, lightingShader, model_1, 0.67f, 0.66f, 0.68f, 2.0f);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.1f + (i * 0.55f), 3.45f, -2.0f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f)); //
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.78f, 0.5f, 1.0f));
            model_1 = translateMatrix * rotateZMatrix * rotateXMatrix * rotateYMatrix * scaleMatrix;

            //drawCurveObj(bezierVAO, lightingShader, model_for_pointlight, 1.0f, 1.0f, 1.0f, 1.0f);
            drawCurveObj(bezierVAO1, lightingShader, model_1, 0.67f, 0.66f, 0.68f, 2.0f);
        }


        for (float i = 1.0f; i < 5.0f; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.1f , 3.45f, 0.0f + (i * 2.15f)));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f)); //
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 3.0f, 1.0f));
            glm::mat4 model_fo = translateMatrix * rotateZMatrix * rotateXMatrix * rotateYMatrix * scaleMatrix;

            //drawCurveObj(bezierVAO, lightingShader, model_for_pointlight, 1.0f, 1.0f, 1.0f, 1.0f);
            drawCurveObj(bezierVAO, lightingShader, model_fo, 0.67f, 0.66f, 0.68f, 2.0f);


        }











        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        // pass projection matrix to shader (note that in this case it could change every frame)
        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShaderWithTexture.setMat4("projection", projection);

        // camera/view transformation
       // glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShaderWithTexture.setMat4("view", view);

        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);

        glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
       //modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(0.867f, 5.37f, 6.667f));
       //fridge1.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4  alTogether;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        alTogether = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        float baseHeight = 0.5; float width = 0.5;  float length = 0.5;
        model = glm::mat4(1.0f);

        //2nd floor
        model = transforamtion(-4.0f, 6.5f, -2.5f, width * 25, baseHeight * 1.1, length * 30);
        model = alTogether * model;
        cube.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transforamtion(-7.0f, 6.5f, -2.5f, width * 31, baseHeight * 1.1, length * 26);
        model = alTogether * model;
        cube.drawCubeWithTexture(lightingShaderWithTexture, model);

        //floor
        model = transforamtion(-7.0f, -0.5f, -2.5f, width * 30, baseHeight*0.1, length*30);
        model = alTogether * model;
        cube.drawCubeWithTexture(lightingShaderWithTexture, model);

        //front wall
        model = transforamtion(-7.0f, -0.5f, -2.0f, width*30 , baseHeight*15, length*0.1);
        model = alTogether * model;
        wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right wall
        model = transforamtion(7.8f, -0.5f, -2.5f, width * 0.1, baseHeight * 15, length * 30);
        model = alTogether * model;
        r_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left wall
        model = transforamtion(-6.5f, -0.5f, -2.5f, width * 0.1, baseHeight * 15, length * 30);
        model = alTogether * model;
        l_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        //uporer_floor_front_wall
        model = transforamtion(-7.5f, 6.5f, -2.0f, width * 30, baseHeight * 15, length * 0.1);
        model = alTogether * model;
        fr_wall.drawCubeWithTexture(lightingShaderWithTexture, model);
        
        //uporer_floor_right_wall
        model = transforamtion(7.98f, 6.5f, -2.5f, width * 1, baseHeight * 15, length * 30);
        model = alTogether * model;
        fr_rg_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        //uporer_floor_left_wall
        model = transforamtion(-6.98f, 6.5f, -2.5f, width * 1, baseHeight * 15, length * 30);
        model = alTogether * model;
        fr_lf_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        //service
        model = transforamtion(3.98f, -1.0f, 8.0f, width * 3.1, baseHeight * 5.01, length * 7.01);
        model = alTogether * model;
        wood.drawCubeWithTexture(lightingShaderWithTexture, model);

        //trial
       
        model = transforamtion(5.0f, 6.5f, -2.5f, width * 6.1, baseHeight * 15.01, length * 9.01);
        model = alTogether * model;
        trial.drawCubeWithTexture(lightingShaderWithTexture, model);

        //shiri
        float c = 0.5f;
        float d = -1.0f;
        for (float i = 1; i < 6; i = i + 1) {
            for (float j = 1; j < 4; j += 1) {
               
                model = transforamtion(c, d, 10.5f, width * 4.2, baseHeight * 1.05, length * 4.01);
                model = alTogether * model;
                wood.drawCubeWithTexture(lightingShaderWithTexture, model);              
                c = c - 0.5;
                d = d + 0.5;
            }
        }


        //fridge1(side 1)
        model = transforamtion(-6.5,-1.0f,-2.0f, width * 3.00, baseHeight * 9.06, length * 5.001);
        model = alTogether * model;
        fridge1.drawCubeWithTexture(lightingShaderWithTexture, model);

        //fridge1(side 2)
        model = transforamtion(-6.5, 3.53, -2.0f, width * 3.00, baseHeight * 9.06, length * 5.001);
        model = alTogether * model;
       // fridge1.drawCubeWithTexture(lightingShaderWithTexture, model);
       // 
        //taak2 er jinish
        for (float j = 0.2f; j < 4.8; j += 0.5f) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.10f, 0.5f, 3.1f + j));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.80f, 0.50f, 0.2f));

            model = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

            choco.drawCubeWithTexture(lightingShaderWithTexture, model);
        }
        for (float j = 0.2f; j < 4.8; j += 0.2f) {
            
            model = transforamtion(-2.98, 0.61, 3.1f + j, width * 2.0, baseHeight * 1.06, length * 0.2);
            model = alTogether * model;
            choco.drawCubeWithTexture(lightingShaderWithTexture, model);
            
        }

        //shelf 3 er jinish 
        //(taak1)

        for (float j = 0.2f; j <3.0; j += 0.4f) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0f, 2.5f, 3.1f + j));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.80f, 0.50f, 0.2f));

            model = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

            chocobar.drawCubeWithTexture(lightingShaderWithTexture, model);
        }
        for (float i = 1.0f; i < 5.00f; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.32f, 8.3f, 3.1f + i * 1.00f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.9, 2.0f, 0.50f));
            model =  translateMatrix * scaleMatrix;
            saree.drawCubeWithTexture(lightingShaderWithTexture, model);
        }

        for (float i = 1.0f; i < 5.00f; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.66f, 8.3f, 3.1f + i * 1.00f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.9, 2.0f, 0.50f));
            model = translateMatrix * scaleMatrix;
            saree.drawCubeWithTexture(lightingShaderWithTexture, model);

        }
        
     
        
        // fridge1.drawCubeWithTexture(lightingShaderWithTexture, model);

        //sphwrew texrue
       
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        // pass projection matrix to shader (note that in this case it could change every frame)
        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShaderWithTexture.setMat4("projection", projection);

        // camera/view transformation
        //glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShaderWithTexture.setMat4("view", view);

        for (float i = 0.13f; i < 1.5f; i += 0.7f) {
            for (float j = 0.2f; j < 4.8; j += 0.7f) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.98f + i, 1.9f, 3.1f + j));
                rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
                rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.18f, .15f, .2f));

                model_sph1 = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

                sphere2.drawSphereWithTexture(lightingShaderWithTexture, model_sph1);
                
            }
        }

        for (float i = 0.13f; i < 1.5f; i += 0.7f) {
            for (float j = 0.2f; j < 4.8; j += 0.7f) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(1.05f + i, 2.15f, 3.1f + j));
                rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
                rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.22f, 0.09f, 0.22f));

                model_sph1 = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

                sphere3.drawSphereWithTexture(lightingShaderWithTexture, model_sph1);

            }
        }
        for (float i = 0.13f; i < 2.5f; i += 0.7f) {

                translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.50f + i, 9.8f, -1.5));
                rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
                rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.3f, 0.02f));

                model_sph1 = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

                sphere41.drawSphereWithTexture(lightingShaderWithTexture, model_sph1);

                translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.50f + i, 8.8f, -1.5));
                rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
                rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.3f, 0.02f));

                model_sph1 = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

                sphere42.drawSphereWithTexture(lightingShaderWithTexture, model_sph1);

                translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.50f + i, 10.8f, -1.5));
                rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
                rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.3f, 0.02f));

                model_sph1 = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

                sphere42.drawSphereWithTexture(lightingShaderWithTexture, model_sph1);
            
        }

        //cylinder texture
        /* modelMatrixForContainer2 = glm::translate(model, glm::vec3(-0.45f, -0.4f, -2.8f));*/
        for (float i = 0.13f; i < 1.5f; i += 0.7f) {
            for (float j = 0.2f; j < 4.8; j += 0.7f) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.98f + i, 1.69f, 3.1f + j));
                rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
                rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.18f, .01f, .2f));

                model = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

                cylinder.Draw(lightingShaderWithTexture, model);
            }
        }

        for (float i = 0.13f; i < 1.5f; i += 0.7f) {
            for (float j = 0.2f; j < 4.8; j += 0.7f) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.9f + i, 2.85f, 3.1f + j));
                rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
                rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.15f, .01f, .2f));

                model = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;
                cylinder2.Draw(lightingShaderWithTexture, model);

                // cylinder.drawSphere(lightingShaderwithTexure, model);
            }
        }
        for (float i = 0.13f; i < 1.5f; i += 0.7f) {
            for (float j = 0.2f; j < 4.8; j += 0.7f) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(1.05f + i, 3.0f, 3.1f + j));
                rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
                rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.18f, .01f, .2f));

                model = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;
                cylinder3.Draw(lightingShaderWithTexture, model);

                // cylinder.drawSphere(lightingShaderwithTexure, model);
            }
        }
        for (float i = 0.13f; i < 1.5f; i += 0.7f) {
            for (float j = 0.2f; j < 4.8; j += 0.7f) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(1.05f + i, 2.0f, 3.1f + j));
                rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
                rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, .005f, .2f));

                model = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;
                cylinder4.Draw(lightingShaderWithTexture, model);

                // cylinder.drawSphere(lightingShaderwithTexure, model);
            }
        }
        for (float i = 0.13f; i < 1.5f; i += 0.7f) {
            for (float j = 0.2f; j < 4.2; j += 0.7f) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(5.00 + i, 2.0f, 2.1f + j));
                rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
                rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.18f, .01f, .2f));

                model = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

                cylinder5.Draw(lightingShaderWithTexture, model);
            }
        }

        for (float i = 0.13f; i < 1.5f; i += 0.7f) {
            for (float j = 0.2f; j < 4.2; j += 0.7f) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(5.00 + i, 1.0f, 2.1f + j));
                rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
                rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.18f, .01f, .2f));

                model = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

                cylinder6.Draw(lightingShaderWithTexture, model);
            }
        }

        for (float i = 0.13f; i < 4.5f; i += 0.7f) {
            
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.58 + i, 3.3f, -1.6f ));
                rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
                rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.18f, .01f, .2f));

                model = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

                cylinder6.Draw(lightingShaderWithTexture, model);
            
        }
        for (float i = 0.13f; i < 4.5f; i += 0.7f) {

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.58 + i, 2.3f, -1.6f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.18f, .013f, .2f));

            model = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

            cylinder5.Draw(lightingShaderWithTexture, model);

        }
        for (float i = 0.13f; i < 4.5f; i += 0.7f) {

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.58 + i, 1.3f, -1.6f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.18f, .01f, .2f));

            model = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

            cylinder7.Draw(lightingShaderWithTexture, model);

        }

        for (float i = 0.13f; i < 7.0f; i += 1.5f) {

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.58 + i, 8.4f, -2.00f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.65f, .015f, 0.5f));

            model = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

            cylinder9.Draw(lightingShaderWithTexture, model);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.58 + i, 9.4f, -2.00f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.65f, .015f, 0.5f));

            model = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

            cylinder9.Draw(lightingShaderWithTexture, model);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.58 + i, 10.4f, -2.00f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.65f, .015f, 0.5f));

            model = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

            cylinder9.Draw(lightingShaderWithTexture, model);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.58 + i, 11.4f, -2.00f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.65f, .015f, 0.5f));

            model = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

            cylinder9.Draw(lightingShaderWithTexture, model);

        }

        
        
        //dynamic
   
        //rotateAngle_Y = 0.0f;
        
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
           
            if (ghurano1 == 1.0f && ghurano2 == 0.0f)
            {
                //tar mane same state
                rotateAngle_Y = 0.0f;

            }
            else if (ghurano1 == 0.0 && ghurano2 == 1.0)
            {
                //proyojon;
                rotateAngle_Y = 90.0f;
          } 
            ghurano1 = 1.0f; ghurano2 = 0.0f; ghurano += 0.02f;
        }
        else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
           
            if (ghurano1 == 1.0f && ghurano2 == 0.0f)
            {
                rotateAngle_Y = 0.0f;

            }
            else if (ghurano1 == 0.0 && ghurano2 == 1.0)
            {
                rotateAngle_Y = -90.0f;
            }
            ghurano1 = 1.0f; ghurano2 = 0.0f; ghurano -= 0.02f;
        }
        else   if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            if (ghurano1 == 0.0f && ghurano2 == 1.0f)
            {
                rotateAngle_Y = 0.0f;

            }
            else if (ghurano1 == 1.0 && ghurano2 == 0.0)
            {
                rotateAngle_Y = 90.0f;
            }
            ghurano1 = 0.0f; ghurano2 = 1.0f; ghurano_z += 0.02f; //printf("%f", rotateAngle_Y);
            
        }
        else  if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
            if (ghurano1 == 0.0f && ghurano2 == 1.0f)
            {
                rotateAngle_Y = 0.0f;

            }
            else if (ghurano1 == 1.0 && ghurano2 == 0.0)
            {
                rotateAngle_Y = -90.0f;
            }
            ghurano1 = 0.0f; ghurano2 = 1.0f; ghurano_z -= 0.02f;
        }

        
        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.58f + ghurano, 0.0f, 0.0f+ ghurano_z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians (rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 2.00f, 2.5f));
        glm::mat4 shear = glm::mat4(1.0f), shear1 = glm::mat4(1.0f), shear2 = glm::mat4(1.0f), shear3 = glm::mat4(1.0f);
        shear[1][0] = 0.81f;
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix * shear;
        drawCube(cubeVAO, lightingShader, model, 0.62f, 0.522f, 0.89f);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.20f + ghurano, 0.0f, 0.0f + ghurano_z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 2.00f, 2.5f));

        shear1[1][0] = -0.81f;
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix * shear1;
        drawCube(cubeVAO, lightingShader, model, 0.62f, 0.522f, 0.89f);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.58f + ghurano, 0.0f, 0.0f + ghurano_z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.88f, 2.00f, 2.5f));

        shear2[1][0] = -0.81f;
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix * shear2;
        drawCube(cubeVAO, lightingShader, model, 0.62f, 0.522f, 0.89f);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.50f + ghurano, 0.0f, 0.0f + ghurano_z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.88f, 2.00f, 2.5f));

        shear3[1][0] = 0.81f;
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix * shear3;
        drawCube(cubeVAO, lightingShader, model, 0.62f, 0.522f, 0.89f);
        //pichoner chaka
        Sphere sphere2 = Sphere();

        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.70f + ghurano, -0.2f, 0.0f + ghurano_z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f + ghurano));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.05f));

        model_sph1 = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

        sphere2.drawSphere(lightingShader, model_sph1);

        Sphere sphere3 = Sphere();

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.00f + ghurano, -0.2f, 0.0f + ghurano_z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f + ghurano));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.05f));

        model_sph1 = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

        sphere3.drawSphere(lightingShader, model_sph1);

        //shamner chaka
        Sphere sphere4 = Sphere();

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.00f + ghurano, -0.2f, 1.24f + ghurano_z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f + ghurano));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.05f));

        model_sph1 = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

        sphere4.drawSphere(lightingShader, model_sph1);

        Sphere sphere5 = Sphere();

        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.70f + ghurano, -0.2f, 1.24f + ghurano_z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f + ghurano));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.05f));

        model_sph1 = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

        sphere5.drawSphere(lightingShader, model_sph1);




        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void shop(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{
    // Modelling Transformation
    //front wall
   
    

    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model1, model2, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-7.0f, -1.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(30.0f, 15.0f, 1.0f));
    model1 = alTogether*translateMatrix * scaleMatrix;
    
    //ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));
    glm::vec3 cubeColor = glm::vec3(0.859f, 0.82f, 0.918f);// Set the desired color for this cube
    drawCube(cubeVAO, lightingShader, model1, 0.859f, 0.82f, 0.918f);



    //floor
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-7.0f, -1.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(30.0f, 1.0f, 30.0f));
    model2 = alTogether*translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model2, 0.867f, 0.737f, 0.667f);

    //ceiling
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.0f, 6.5f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(25.0f, 1.0f, 30.0f));
    model2 = alTogether * translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    //drawCube(cubeVAO, lightingShader, model2, 0.867f, 0.737f, 0.667f);

    //ceiling
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-7.0f, 6.5f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(31.0f, 1.0f, 26.0f));
    model2 = alTogether * translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
   // drawCube(cubeVAO, lightingShader, model2, 0.867f, 0.737f, 0.667f);
    

    //left wall
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-7.0f, -1.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 15.0f, 30.0f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.62f, 0.522f, 0.749f);

    //right wall
    translateMatrix = glm::translate(identityMatrix, glm::vec3(8.0f, -1.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 15.0f, 30.0f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.62f, 0.522f, 0.749f);


    //service table
    //lower part
    translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0f, -1.0f, 8.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 5.0f, 7.0f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.6f, 0.52f, 0.79f);


    //stair
    float a = -0.5f;
    float b =  0.5f;
    for (float i = 1;i<5 ; i = i+1) {
        for (float j = 1; j < 4; j += 1) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(a, b , 10.5));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 1.0f, 4.0f));

            model = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

            //drawCube(cubeVAO, lightingShader, model, 1.0f, 1.0f, 0.79f);
            a = a - 0.5;
            b = b + 0.5;
        }
    }

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5f, 0.5f, 10.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 1.0f, 4.0f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    //drawCube(cubeVAO, lightingShader, model, 1.0f, 1.0f, 0.79f);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 10.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 1.0f, 4.0f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
   // drawCube(cubeVAO, lightingShader, model, 1.0f, 1.0f, 0.79f);




    //fridge
    /*translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.5f, -1.0f, -2.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 9.0f, 5.0f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.62f, 0.522f, 0.89f);*/

    //shelf1 
    //paya1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, -1.0f, 8.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 9.0f, 0.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //paya2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, -1.0f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 9.0f, 0.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //paya3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, -1.0f, 8.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 9.0f, 0.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //paya4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, -1.0f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 9.0f, 0.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);


    //taak1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.00f, 3.5f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.2f, 0.15f, 10.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);


    //taak2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.00f, 2.5f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.2f, 0.15f, 10.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

   


    //taak3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.00f, 1.5f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.2f, 0.15f, 10.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    
    
    Sphere sphere1 = Sphere();
    for (float i = 0.13f; i < 1.5f; i += 0.7f) {
        for (float j = 0.2f; j < 4.8; j += 0.7f) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.98f + i, 1.9f, 3.1f + j));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.18f, .15f, .2f));

            model_sph1 = alTogether * rotateXMatrix * translateMatrix * scaleMatrix * rotateYMatrix * rotateZMatrix;

            sphere1.drawSphere(lightingShader, model_sph1);
        }
    }

    //taak4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.00f, 0.5f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.2f, 0.15f, 10.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    
    
    

    //shelf2
    //paya1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, -1.0f, 8.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 9.0f, 0.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //paya2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, -1.0f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 9.0f, 0.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //paya3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0f, -1.0f, 8.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 9.0f, 0.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //paya4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0f, -1.0f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 9.0f, 0.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.00f, 3.5f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.2f, 0.15f, 10.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.00f, 2.5f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.2f, 0.15f, 10.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.00f, 1.5f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.2f, 0.15f, 10.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.00f, 0.5f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.2f, 0.15f, 10.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //shelf3
    //paya 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0f, -1.0f, 6.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 9.0f, 0.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f,0.604f,0.42f);

    //paya 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, -1.0f, 2.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 9.0f, 0.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //paya 3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0f, -1.0f, 2.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 9.0f, 0.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //paya 4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, -1.0f, 6.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 9.0f, 0.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.00f, 3.5f, 2.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.2f, 0.15f, 8.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.00f, 2.5f, 2.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.2f, 0.15f, 8.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.00f, 1.5f, 2.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.2f, 0.15f, 8.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.00f, 0.5f, 2.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.2f, 0.15f, 8.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //wall shelf
    //taak1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.00f, 4.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 0.15f, 2.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.00f, 3.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 0.15f, 2.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.00f, 2.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 0.15f, 2.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.00f, 1.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 0.15f, 2.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //paya 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.00f, -1.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 10.00f, 2.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //paya2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.88f, -1.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 10.00f, 2.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);
   


    //wall shelf2
    //taak1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(4.00f, 4.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 0.15f, 2.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(4.00f, 3.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 0.15f, 2.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(4.00f, 2.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 0.15f, 2.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(4.00f, 1.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 0.15f, 2.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //paya1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(4.00f, -1.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 10.00f, 2.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //paya2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.88f, -1.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 10.00f, 2.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);


    //spotlight
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 6.0f, 0.25f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5f, 0.5f, 0.5f));
    glm::mat4 spott = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, spott, 1.0f, 1.0f, .5f);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.5f, 6.0f, 2.25f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5f, 0.5f, 0.5f));
    glm::mat4 spott1 = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, spott1, 5.0f, 5.0f, 5.5f);

    //pointlights
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, 6.0, 0.19f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5f, 0.5f, 0.5f));
    glm::mat4 spott2 = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, spott2, 1.0f, 1.0f, 1.0f);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 6.0f, 13.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5f, 0.5f, 0.5f));
    glm::mat4 spott3 = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, spott3, 1.0f, 1.0f, 1.0f);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.8f, 13.0f, 5.19f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5f, 0.5f, 0.5f));
    glm::mat4 spott4 = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, spott4, 1.0f, 1.0f, 1.0f);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.14f,10.01f, 6.2f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5f, 0.5f, 0.5f));
    glm::mat4 spott5 = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, spott5, 1.0f, 1.0f, 1.0f);


    //second floor

    //ceil
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-7.0f, 14.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(31.0f, 1.0f, 30.0f));
    model2 = alTogether * translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    //drawCube(cubeVAO, lightingShader, model2, 0.0729f, 0.1651f, 0.42f);

    //left wall
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-7.0f, 6.5f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 15.0f, 30.0f));
    model2 = alTogether * translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model2, 0.757f, 0.604f, 0.42f);

    //right wall
    translateMatrix = glm::translate(identityMatrix, glm::vec3(8.0f, 6.5f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 15.0f, 30.0f));
    model2 = alTogether * translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model2, 0.757f, 0.604f, 0.42f);

    //front wall
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-7.0f, 6.5f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(30.0f, 15.0f, 1.0f));
    model2 = alTogether * translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model2, 2.9f, 0.65f, 2.9f);

    //paya1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.00f, 6.5f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 10.00f, 2.5f));
    model2 = alTogether * translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model2, 0.757f, 0.604f, 0.42f);

    //paya2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, 6.5f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 10.00f, 2.5f));
    model2 = alTogether * translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model2, 0.757f, 0.604f, 0.42f);

    //taak1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.00f, 11.5f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.2f, 0.15f, 2.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.00f, 9.5f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.2f, 0.15f, 2.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.00f, 10.5f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.2f, 0.15f, 2.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.00f, 8.5f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.2f, 0.15f, 2.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak5
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.00f, 7.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.2f, 0.15f, 2.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //trial room
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0f, 6.5f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.00f, 15.00f, 9.0f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
   // drawCube(cubeVAO, lightingShader, model, 0.327f, 0.428f, 0.870f);


    //taak2-1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.00f, 8.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(16.0f, 0.15f, 2.3f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak2-2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.00f, 9.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(16.0f, 0.15f, 2.3f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak2-3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.00f, 10.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(16.0f, 0.15f, 2.3f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak2-4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.00f, 11.0f, -2.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(16.0f, 0.15f, 2.3f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);


    //wall decor
    //taak2-1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.00f, 12.0f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 2.00f, 10.0f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.327f, 0.428f, 0.870f);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.1f, 12.0f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.4f, 0.2f, 10.2f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.327f, 0.428f, 0.870f);

    //paya1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.0f, 13.0f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 2.00f, 0.25f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.327f, 0.428f, 0.870f);

    //paya2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.125f, 13.0f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 2.00f, 0.25f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.327f, 0.428f, 0.870f);

    //paya3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.125f, 13.0f, 7.875f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 2.00f, 0.25f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.327f, 0.428f, 0.870f);


    //paya4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.0f, 13.0f, 7.875f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 2.00f, 0.25f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.327f, 0.428f, 0.870f);

    //cosmetics show room
    //nicher part
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 6.5f, 4.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 2.50f, 11.00f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //uporerr part
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 12.5f, 4.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 2.50f, 11.00f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //paya1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 6.5f, 4.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 12.00f, 0.25f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //paya2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 6.5f, 9.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 12.00f, 0.25f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.757f, 0.604f, 0.42f);

    //taak1
   
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 8.5f, 4.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 0.50f, 11.0f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.0729f, 0.1651f, 0.42f);

    //taak2

    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 9.5f, 4.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 0.50f, 11.0f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.0729f, 0.1651f, 0.42f);

    //taak3

    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 10.5f, 4.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 0.50f, 11.0f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.0729f, 0.1651f, 0.42f);

    //taak4

    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 11.5f, 4.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 0.50f, 11.0f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.0729f, 0.1651f, 0.42f);

    //jama kapor

    //paya1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, 6.5f, 8.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 9.0f, 0.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.582f, 0.29f, 0.0f);

    //paya2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, 6.5f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 9.0f, 0.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.582f, 0.29f, 0.0f);

     //handle
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, 11.0f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.5f, 10.0f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.582f, 0.29f, 0.0f);

    //hanger 

    for (float i = 0.0f; i < 5.0f; i++)

    {
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, 11.0f, 3.1f + i * 1.00f));
        glm::mat4 rotation = glm::rotate(identityMatrix, glm::radians(225.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0, 0.5f, 0.5f));
        model = translateMatrix * rotation * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, 0.582f, 0.29f, 0.0f);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, 11.0f, 3.1f + i * 1.00f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0, 0.5f, 0.5f));
        rotation = glm::rotate(identityMatrix, glm::radians(315.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateMatrix * rotation * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, 0.582f, 0.29f, 0.0f);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.4f, 8.5f, 3.1f + i * 1.00f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0, 3.5f, 0.5f));
        model = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, 0.582f, 0.29f, 0.0f);

    }

    //jama kapor

    //paya1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 6.5f, 8.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 9.0f, 0.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.582f, 0.29f, 0.0f);

    //paya2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 6.5f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 9.0f, 0.1f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.582f, 0.29f, 0.0f);

    //handle
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 11.0f, 3.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.5f, 10.0f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.582f, 0.29f, 0.0f);

    //hanger 

    for (float i = 0.0f; i < 5.0f; i++)

    {
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 11.0f, 3.1f + i * 1.00f));
        glm::mat4 rotation = glm::rotate(identityMatrix, glm::radians(225.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0, 0.5f, 0.5f));
        model = translateMatrix * rotation * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, 0.582f, 0.29f, 0.0f);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 11.0f, 3.1f + i * 1.00f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0, 0.5f, 0.5f));
        rotation = glm::rotate(identityMatrix, glm::radians(315.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateMatrix * rotation * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, 0.582f, 0.29f, 0.0f);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.4f, 8.5f, 3.1f + i * 1.00f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0, 3.5f, 0.5f));
        model = translateMatrix * scaleMatrix;
       // drawCube(cubeVAO, lightingShader, model, 0.62f, 0.522f, 0.89f);

    }






    /*
    //back support

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5 * 4.5f, translate_Y, 0.5 * 13.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 2.0f, 0.5f));
    glm::mat4 modelback = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, modelback, 1.0f, 1.0f, 0.749f);
    




    glm::vec3(1.f, 3.70f+4.01f,  0.13f),
    glm::vec3(0.13f,  3.7f+4.01f,  1.0f),
    glm::vec3(5.8f,  3.7f+4.01f,  0.2f),
    glm::vec3(0.13f,  3.7f+4.01f,  6.2f)

 

    //backseat

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5 * 7.0f, 0.5 * 1.0, 0.5 * 13.4f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.993f, 0.25f, 1.6f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-83.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 modelback1 = translateMatrix * rotateXMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, modelback1, 1.0f, 1.0f, .49f);


    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5 * 5.0f, 0.5 * 1.0, 0.5 * 13.4f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.993f, 0.25f, 1.6f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-83.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 modelback2 = translateMatrix * rotateXMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, modelback2, 1.0f, 1.0f, .49f);
    glLineWidth(2);
   
 

    //reception desk

    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f,0.0f,1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 2.8f, 4.2f));
    glm::mat4 bigdesk = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, bigdesk, 0.739f, 0.461f, 0.46f);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.2f,0.0f,1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.2f,2.7f,1.0f));
    glm::mat4 smalldesk = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, smalldesk, 0.729f, 0.451f, 0.42f);


*/

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 0.1;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
        else rotateAngle_Z -= 0.1;
    }
    /*if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001;*/

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 0.1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
   if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }


    ///// camera class changes
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(YAW_L, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        camera.ProcessKeyboard(YAW_R, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.ProcessKeyboard(ROLL_R, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.ProcessKeyboard(ROLL_L, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(PITCH_R, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        camera.ProcessKeyboard(PITCH_L, deltaTime);
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight3.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight3.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    else if (key == GLFW_KEY_7 && action == GLFW_PRESS)
    {
        if (pointLight23On)
        {
            pointlight2.turnOff();
            pointlight4.turnOff();
            pointLight23On = !pointLight23On;
        }
        else
        {
            pointlight2.turnOn();
            pointlight4.turnOn();
            pointLight23On = !pointLight23On;
        }
    }
   

    else if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        if (specularToggle)
        {

            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();

            specularToggle = !specularToggle;
        }
        else
        {

            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();

        
            specularToggle = !specularToggle;
        }
    }

    else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        if (diffuseToggle)
        {

            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {

            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }

    else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        if (ambientToggle)
        {

            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            pointlight4.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {

            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            pointlight4.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }

    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        if (directionalToggle)
        {

            vard = false;
            directionalToggle = !directionalToggle;
        }
        else
        {

            vard = true;
            directionalToggle = !directionalToggle;
        }
        }
    else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        if (spotToggle)
        {
            vars = false;
            spotToggle = !spotToggle;
        }
        else
        {

            vars = true;
            spotToggle = !spotToggle;
        }
        }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}








void drawCurveObj(unsigned int& bezierVAO_, Shader& lightingShader, glm::mat4 model, float r = 1.0f, float g = 1.0f, float b = 1.0f, float shine = 32.0f) {
    lightingShader.use();
    lightingShader.setMat4("model", model);
    lightingShader.setVec3("material.emission", glm::vec3(0, 0, 0));
    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader.setFloat("material.shininess", shine);

    glBindVertexArray(bezierVAO_);
    glDrawElements(GL_TRIANGLES, (unsigned int)lampIndices.size(), GL_UNSIGNED_INT, (void*)0);

    // unbind VAO
    glBindVertexArray(0);
}

long long nCr(int n, int r)
{
    if (r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for (i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

//polynomial interpretation for N points
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L)
{
    double y = 0;
    double x = 0;
    t = t > 1.0 ? 1.0 : t;
    for (int i = 0; i < L + 1; i++)
    {
        long long ncr = nCr(L, i);
        double oneMinusTpow = pow(1 - t, double(L - i));
        double tPow = pow(t, double(i));
        double coef = oneMinusTpow * tPow * ncr;
        x += coef * ctrlpoints[i * 3];
        y += coef * ctrlpoints[(i * 3) + 1];

    }
    xy[0] = float(x);
    xy[1] = float(y);
}

unsigned int hollowBezier(GLfloat ctrlpoints[], int L, vector <float>& coordinates,
    vector <float>& normals,
    vector <int>& indices,
    vector <float>& vertices)
{
    int i, j;
    float x, y, z, r;                //current coordinates
    float theta;
    float nx, ny, nz, lengthInv;    // vertex normal


    const float dtheta = 2 * pi / ntheta;        //angular step size

    float t = 0;
    float dt = 1.0 / nt;
    float xy[2];

    for (i = 0; i <= nt; ++i)              //step through y
    {
        BezierCurve(t, xy, ctrlpoints, L);
        r = xy[0];
        y = xy[1];
        theta = 0;
        t += dt;
        lengthInv = 1.0 / r;

        for (j = 0; j <= ntheta; ++j)
        {
            double cosa = cos(theta);
            double sina = sin(theta);
            z = r * cosa;
            x = r * sina;

            coordinates.push_back(x);
            coordinates.push_back(y);
            coordinates.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            // center point of the circle (0,y,0)
            nx = (x - 0) * lengthInv;
            ny = (y - y) * lengthInv;
            nz = (z - 0) * lengthInv;

            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            theta += dtheta;
        }
    }

    // generate index list of triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1

    int k1, k2;
    for (int i = 0; i < nt; ++i)
    {
        k1 = i * (ntheta + 1);     // beginning of current stack
        k2 = k1 + ntheta + 1;      // beginning of next stack

        for (int j = 0; j < ntheta; ++j, ++k1, ++k2)
        {
            // k1 => k2 => k1+1
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);

            // k1+1 => k2 => k2+1
            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }

    size_t count = coordinates.size();
    for (int i = 0; i < count; i += 3)
    {
        vertices.push_back(coordinates[i]);
        vertices.push_back(coordinates[i + 1]);
        vertices.push_back(coordinates[i + 2]);

        vertices.push_back(normals[i]);
        vertices.push_back(normals[i + 1]);
        vertices.push_back(normals[i + 2]);
    }

    unsigned int bezierVAO;
    glGenVertexArrays(1, &bezierVAO);
    glBindVertexArray(bezierVAO);

    // create VBO to copy vertex data to VBO
    unsigned int bezierVBO;
    glGenBuffers(1, &bezierVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
        vertices.data(),   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage

    // create EBO to copy index data
    unsigned int bezierEBO;
    glGenBuffers(1, &bezierEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);   // for index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        (unsigned int)indices.size() * sizeof(unsigned int),             // data size, # of bytes
        indices.data(),               // ptr to index data
        GL_STATIC_DRAW);                   // usage

    // activate attrib arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // set attrib arrays with stride and offset
    int stride = 24;     // should be 24 bytes
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

    // unbind VAO, VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return bezierVAO;
}
