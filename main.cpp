/*
Amirreza Allahdad
Khuong Vinh Khang Tran
GAM 531
Final Project
April 19, 2021
*/



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

#include <iostream>
#include <windows.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void init();
void renderCube();
void renderLamp(Shader& lightCubeShader, glm::mat4& projection, glm::mat4& view, glm::mat4& model);
unsigned int loadTexture(const char* path);
void texture2Rendering();
void renderPlat(Shader& platShader, glm::vec3& platPosition, glm::vec3& ballPosition, unsigned int diffuseMap, unsigned int specularMap);

// settings
const unsigned int screen_width = 1024;
const unsigned int screen_height = 768;
                                                                                  
unsigned int texture1, texture2;
int width, height, nrChannels;                                                              

bool moveBall = false;
bool movingRight = true;
float mover = 0.0f;
float ballMover = 0.0f;
bool firstTime = true; //To check if it's the first time running the program.



// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));


// lighting
glm::vec3 lightPos(0.0f, 0.5f, 2.5f);

unsigned int VBO, cubeVAO;
unsigned int lightCubeVAO;

//texture sets
unsigned int texture;
const char* textureSet1[6] = { "assets/block1.jpg","assets/block2.jpg", "assets/block3.jpg", "assets/box5.jpg", "assets/glass.png", "assets/danger.png" };



int main()
{
    /// glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window;
    window = glfwCreateWindow(screen_width, screen_height, "Arkanoid - Final Project", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // glad
     // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);


    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("lightingmap.vs", "lightingmap.fs");
    Shader lightCubeShader("lightcube.vs", "lightcube.fs");
    Shader platShader("platShader.vs", "platShader.fs");

    init();
    
    // primitives spawning locations
    glm::vec3 primPositions[] = {
        glm::vec3(-1.65f, 1.09f, 0.0f),
        glm::vec3(-1.36f, 1.09f, 0.0f),
        glm::vec3(-1.07f, 1.09f, 0.0f),
        glm::vec3(-0.78f,  1.09f, 0.0f),
        glm::vec3(-0.49f, 1.09f, 0.0f),
        glm::vec3(-0.2f,  1.09f, 0.0f),
        glm::vec3(0.09f,  1.09f, 0.0f),
        glm::vec3(0.38f,  1.09f, 0.0f),
        glm::vec3(0.67f,  1.09f, 0.0f),
        glm::vec3(0.96f,  1.09f, 0.0f),
        glm::vec3(1.25f,  1.09f, 0.0f),
        glm::vec3(1.54f,  1.09f, 0.0f),

        //Second Row

        glm::vec3(-1.65f, 0.93f, 0.0f),
        glm::vec3(-1.36f, 0.93f, 0.0f),
        glm::vec3(-1.07f, 0.93f, 0.0f),
        glm::vec3(-0.78f,  0.93f, 0.0f),
        glm::vec3(-0.49f, 0.93f, 0.0f),
        glm::vec3(-0.2f,  0.93f, 0.0f),
        glm::vec3(0.09f,  0.93f, 0.0f),
        glm::vec3(0.38f,  0.93f, 0.0f),
        glm::vec3(0.67f,  0.93f, 0.0f),
        glm::vec3(0.96f,  0.93f, 0.0f),
        glm::vec3(1.25f,  0.93f, 0.0f),
        glm::vec3(1.54f,  0.93f, 0.0f),

        //Third Row

        glm::vec3(-1.65f, 0.77f, 0.0f),
        glm::vec3(-1.36f, 0.77f, 0.0f),
        glm::vec3(-1.07f, 0.77f, 0.0f),
        glm::vec3(-0.78f,  0.77f, 0.0f),
        glm::vec3(-0.49f, 0.77f, 0.0f),
        glm::vec3(-0.2f,  0.77f, 0.0f),
        glm::vec3(0.09f,  0.77f, 0.0f),
        glm::vec3(0.38f,  0.77f, 0.0f),
        glm::vec3(0.67f,  0.77f, 0.0f),
        glm::vec3(0.96f,  0.77f, 0.0f),
        glm::vec3(1.25f,  0.77f, 0.0f),
        glm::vec3(1.54f,  0.77f, 0.0f),

        //Fourth Row 

        glm::vec3(-1.65f, 0.61f, 0.0f),
        glm::vec3(-1.36f, 0.61f, 0.0f),
        glm::vec3(-1.07f, 0.61f, 0.0f),
        glm::vec3(-0.78f,  0.61f, 0.0f),
        glm::vec3(-0.49f, 0.61f, 0.0f),
        glm::vec3(-0.2f,  0.61f, 0.0f),
        glm::vec3(0.09f,  0.61f, 0.0f),
        glm::vec3(0.38f,  0.61f, 0.0f),
        glm::vec3(0.67f,  0.61f, 0.0f),
        glm::vec3(0.96f,  0.61f, 0.0f),
        glm::vec3(1.25f,  0.61f, 0.0f),
        glm::vec3(1.54f,  0.61f, 0.0f),

        //Fifth Row

        glm::vec3(-1.65f, 0.45f, 0.0f),
        glm::vec3(-1.36f, 0.45f, 0.0f),
        glm::vec3(-1.07f, 0.45f, 0.0f),
        glm::vec3(-0.78f,  0.45f, 0.0f),
        glm::vec3(-0.49f, 0.45f, 0.0f),
        glm::vec3(-0.2f,  0.45f, 0.0f),
        glm::vec3(0.09f,  0.45f, 0.0f),
        glm::vec3(0.38f,  0.45f, 0.0f),
        glm::vec3(0.67f,  0.45f, 0.0f),
        glm::vec3(0.96f,  0.45f, 0.0f),
        glm::vec3(1.25f,  0.45f, 0.0f),
        glm::vec3(1.54f,  0.45f, 0.0f)
    };

    glm::vec3 platPosition = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 ballPosition = glm::vec3(0.0f, -0.88f, 0.0f);

    // load  (we now use a utility function to keep the code more organized)
    // -----------------------------------------------------------------------------

    texture2Rendering();
    unsigned int diffuseMap1[6] = { loadTexture(textureSet1[0]), loadTexture(textureSet1[1]), loadTexture(textureSet1[2]), loadTexture(textureSet1[4]), loadTexture(textureSet1[3]), loadTexture(textureSet1[5]) };
    unsigned int specularMap1[6] = { loadTexture(textureSet1[0]), loadTexture(textureSet1[1]), loadTexture(textureSet1[2]),loadTexture(textureSet1[4]), loadTexture(textureSet1[3]) , loadTexture(textureSet1[5]) };
   
 

    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);
    
    // render loop
    // -----------
    Sleep(2500);

    while (!glfwWindowShouldClose(window))
    {
        
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        //lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // light properties
        lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        

        lightingShader.setFloat("material.shininess", 64.0f); //that's the shininess, you are looking for :)

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
       
        //Render platform and Ball
        renderPlat(platShader, platPosition, ballPosition, diffuseMap1[4], specularMap1[5]);
        
        //Unbinding the second textures
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, diffuseMap1[3]);

        // Sleep(2500);
        //drawing bricks
        for (unsigned i = 0; i < 60; i++)
        {

            
           if (i < 12)
            {
                // bind diffuse map
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, diffuseMap1[0]);
                
                // bind specular map
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, specularMap1[0]);
            }
            else if (i < 24) {
                // bind diffuse map
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, diffuseMap1[1]);
                // bind specular map
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, specularMap1[1]);
            }
            else if (i < 36) {
               // bind diffuse map
               glActiveTexture(GL_TEXTURE0);
               glBindTexture(GL_TEXTURE_2D, diffuseMap1[2]);
               // bind specular map
               glActiveTexture(GL_TEXTURE1);
               glBindTexture(GL_TEXTURE_2D, specularMap1[2]);
           }
            else if (i < 48) {
               // bind diffuse map
               glActiveTexture(GL_TEXTURE0);
               glBindTexture(GL_TEXTURE_2D, diffuseMap1[0]);
               // bind specular map
               glActiveTexture(GL_TEXTURE1);
               glBindTexture(GL_TEXTURE_2D, specularMap1[0]);
           }
            else if (i < 60) {
               // bind diffuse map
               glActiveTexture(GL_TEXTURE0);
               glBindTexture(GL_TEXTURE_2D, diffuseMap1[1]);
               // bind specular map
               glActiveTexture(GL_TEXTURE1);
               glBindTexture(GL_TEXTURE_2D, specularMap1[1]);
           }
            

            model = glm::mat4(1.0f);
            model = glm::translate(model, primPositions[i]);
            model = glm::scale(model, glm::vec3(0.3f, 0.25f, 0.25f));
            model = glm::scale(model, glm::vec3(0.9f, 0.5f, 0.5f));
            lightingShader.setMat4("model", model);

            renderCube();
            //resetting light properties
            lightingShader.setVec3("material.ambient", 1.0f, 1.0f, 1.0f);  
            lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
            lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
            lightingShader.setVec3("light.specular", 0.0f, 1.0f, 0.0f);
        }
        
        //model = glm::mat4(1.0f);
        //renderLamp(lightCubeShader, projection, view, model);
       
        if (movingRight)
        {
            //Move the ball upwards
            model = glm::mat4(1.7f);

            lightPos = glm::vec3(mover,0.9f, 3.0f);
            lightingShader.setVec3("light.position", lightPos);
            renderLamp(lightCubeShader, projection, view, model);
            mover += 0.0005;
            if (mover > 2.0f)
            {
                movingRight = false;
            }

        }
        else
        {
            lightPos = glm::vec3(mover, 0.9f, 3.0f);
            lightingShader.setVec3("light.position", lightPos);
            renderLamp(lightCubeShader, projection, view, model);
            mover -= 0.0005;
            if (mover < -2.0f)
            {
                movingRight = true;
            }
        }

        // glfw: swap buffers
        glfwSwapBuffers(window);
        // gldw: poll for events
        glfwPollEvents();

    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void renderLamp(Shader& lightCubeShader, glm::mat4& projection, glm::mat4& view, glm::mat4& model)
{
    // also draw the lamp object
    lightCubeShader.use();
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lightCubeShader.setMat4("model", model);

    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void renderCube()
{
    // render the cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void renderPlat(Shader& platShader, glm::vec3& platPosition, glm::vec3& ballPosition, unsigned int diffuseMap, unsigned int specularMap)
{
    platShader.use();
    platShader.setInt("material.diffuse", 0);
    platShader.setInt("material.specular", 1);
    platShader.setInt("texture2", 2);
    
    //lightingShader.setVec3("light.position", lightPos);
    platShader.setVec3("viewPos", camera.Position);

    // light properties
    platShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    platShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    platShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    platShader.setFloat("material.shininess", 64.0f); //that's the shininess, you are looking for :)

    // view/projection transformations
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    platShader.setMat4("projection", projection);
    platShader.setMat4("view", view);

    platShader.setVec3("material.ambient", 1.0f, 0.0f, 0.0f);
    
    //Draw Platform

           // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    model = glm::mat4(1.0f);
    model = glm::translate(model, platPosition);
    model = glm::scale(model, glm::vec3(0.3f, 0.1f, 0.25f));

    platShader.setMat4("model", model);
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    platShader.setVec3("material.ambient", 0.0f, 0.0f, 0.0f);
    //drawing ball
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    if (moveBall)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, ballPosition);
        model = glm::translate(model, glm::vec3(0.0f, ballMover, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        platShader.setMat4("model", model);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        ballMover += 0.0003f;
    }
    else
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, ballPosition);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        platShader.setMat4("model", model);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

    }
                                         
}


void init()
{
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    // first, configure the cube's VAO (and VBO)

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)

    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

   

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        moveBall = true;
   
        
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}




// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void texture2Rendering()
{
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // texture wrapping params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set wrapping to GL_REPEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // filtering params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // load image (smilie.png) and create the texture 
    unsigned char* data1 = stbi_load("assets/smilie.png", &width, &height, &nrChannels, 0);
    if (data1)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data1);
}