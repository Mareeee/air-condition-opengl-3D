#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Header/vertices.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../Header/util.h"
#include "../Header/FreeType.h"
#include "../Header/model.hpp"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f, pitch = 0.0f;
float lastX = 400, lastY = 300;
float deltaTime = 0.0f, lastFrame = 0.0f;
int screenWidth = 1920, screenHeight = 1080;

float verticalVelocity = 0.0f, currentSpeed = 0.0f;
const float walkSpeed = 2.5f, runSpeed = 5.0f;
const float acceleration = 10.0f, deceleration = 10.0f;
const float gravity = -9.81f;
const float jumpForce = 5.0f;
bool isGrounded = true;

bool isHovered = false;
bool isOn = false;
glm::vec3 buttonPos = glm::vec3(0.775f, 0.225f, -1.275f);

float пречагаAngle = 0.0f;
const float пречагаOpenAngle = -15.0f;
const float пречагаClosedAngle = 0.0f;
const float пречагаRotationSpeed = 10.0f;

float waterLevel = 0.0f;
const float maxWaterLevel = 0.2f;
const float fillSpeed = 0.02f;

float dropY = -1.5f;
bool dropActive = false;
const float dropSpeed = 2.0f;
glm::vec3 dropStartPos = glm::vec3(-2.0f, 0.15f, 0.0f);

float basinRadius = 0.3f;
bool isBasinHovered = false;
bool isBasinPickedUp = false;
glm::vec3 basinPos = glm::vec3(-2.0f, -1.5f, 0.0f);

bool depthTestEnabled = true;
bool faceCullingEnabled = false;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && isHovered) {
        isOn = !isOn;
    }

    if (isBasinHovered && !isBasinPickedUp) {
        isBasinPickedUp = true;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

    glm::vec3 v = buttonPos - cameraPos;
    float distanceAlongRay = glm::dot(v, cameraFront);

    glm::vec3 closestPointOnRay = cameraPos + (distanceAlongRay * cameraFront);
    float distToCenter = glm::length(buttonPos - closestPointOnRay);

    if (distToCenter < 0.05 && distanceAlongRay > 0) {
        isHovered = true;
    }
    else {
        isHovered = false;
    }

    glm::vec3 vBasin = basinPos - cameraPos;
    float distBasinAlongRay = glm::dot(vBasin, cameraFront);

    glm::vec3 closestPointBasin = cameraPos + (distBasinAlongRay * cameraFront);
    float distToBasinCenter = glm::length(basinPos - closestPointBasin);

    if (distToBasinCenter < basinRadius && distBasinAlongRay > 0 && waterLevel >= maxWaterLevel) {
        isBasinHovered = true;
    }
    else {
        isBasinHovered = false;
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (isOn) {
        waterLevel < maxWaterLevel ? waterLevel += fillSpeed * deltaTime : isOn = false;

        if (пречагаAngle > пречагаOpenAngle)
            пречагаAngle -= пречагаRotationSpeed * deltaTime;
        if (!dropActive) {
            dropY = 0.0f;
            dropActive = true;
        }
        dropY -= dropSpeed * deltaTime;

        if (dropY < -1.6f) {
            dropActive = false;
        }
    }
    else {
        if (пречагаAngle < пречагаClosedAngle)
            пречагаAngle += пречагаRotationSpeed * deltaTime;
        dropActive = false;
    }

    bool isRunning = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    float targetSpeed = isRunning ? runSpeed : walkSpeed;

    bool isMoving = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;

    if (!isMoving) targetSpeed = 0.0f;

    if (currentSpeed < targetSpeed) {
        currentSpeed += acceleration * deltaTime;
        if (currentSpeed > targetSpeed) currentSpeed = targetSpeed;
    }
    else if (currentSpeed > targetSpeed) {
        currentSpeed -= deceleration * deltaTime;
        if (currentSpeed < targetSpeed) currentSpeed = targetSpeed;
    }

    glm::vec3 direction(0.0f);
    glm::vec3 forward = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
    glm::vec3 right = glm::normalize(glm::cross(forward, cameraUp));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) direction += forward;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) direction -= forward;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) direction -= right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) direction += right;

    if (glm::length(direction) > 0.0f) {
        direction = glm::normalize(direction);
        cameraPos += direction * currentSpeed * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS /*&& isGrounded*/ && !isBasinPickedUp) {
        verticalVelocity = jumpForce;
        isGrounded = false;
    }

    if (!isGrounded) {
        verticalVelocity += gravity * deltaTime;
        cameraPos.y += verticalVelocity * deltaTime;
    }

    if (cameraPos.y <= 0.0f) {
        cameraPos.y = 0.0f;
        verticalVelocity = 0.0f;
        isGrounded = true;
    }

    if (isBasinPickedUp) {
        basinPos = cameraPos + cameraFront * 0.8f + glm::vec3(0.0f, -0.3f, 0.0f);

        glm::vec3 toAC = glm::normalize(glm::vec3(-2.0f, 0.0f, 0.0f) - cameraPos);
        float angleCos = glm::dot(cameraFront, toAC); 
        
        if (waterLevel > 0.0f) {
            if (angleCos < -0.5f && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                waterLevel = 0.0f;
            }
        } else {
            if (angleCos > 0.99f && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                basinPos = glm::vec3(-2.0f, -1.5f, 0.0f);
                isBasinPickedUp = false;
            }
        }
    }
}

void formVAOs(float* vertices, size_t size, unsigned int& VAO) {
    unsigned int stride = (3 + 3) * sizeof(float);
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

int formRoundVAOs(unsigned int& VAO, bool isSphere) {
    std::vector<float> verts = isSphere ? generateSphere(1.0f, 16) : generateCylinder(0.3f, 0.2f, 32);

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return verts.size();
}

void form2DVAOs(float* vertices, size_t size, unsigned int& VAO) {
    unsigned int VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3) * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void drawCuboid(Shader unifiedShader, unsigned int VAOcuboid, bool јелПречага, float colorX, float colorY, float colorZ) {
    unifiedShader.setBool("uUseTexture", false);
    unifiedShader.setVec3("uMaterial.kD", colorX, colorY, colorZ);
    unifiedShader.setVec3("uMaterial.kA", 0.2f, 0.2f, 0.2f);
    unifiedShader.setVec3("uMaterial.kS", 0.72811f, 0.626959f, 0.626959f);
    unifiedShader.setFloat("uMaterial.shine", 0.6 * 120);

    depthTestEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    faceCullingEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

    glm::mat4 model = glm::mat4(1.0f);
    if (јелПречага) {
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.7f));
        model = glm::rotate(model, glm::radians(-пречагаAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.7f));
    }
    unifiedShader.setMat4("uM", model);
    glBindVertexArray(VAOcuboid);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawRoundObjects(Shader unifiedShader, unsigned int VAOround, int verts, bool isSphere) {
    unifiedShader.setBool("uUseTexture", false);
    glm::mat4 model = glm::mat4(1.0f);

    depthTestEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    faceCullingEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

    if (isSphere && dropActive) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        unifiedShader.setVec3("uMaterial.kD", 0.0f, 0.7f, 1.0f);
        unifiedShader.setFloat("uAlpha", 0.25f);
        model = glm::translate(model, dropStartPos);
        model = glm::translate(model, glm::vec3(0.0f, dropY, 0.0f));
        model = glm::scale(model, glm::vec3(0.03f));
        unifiedShader.setMat4("uM", model);
        glBindVertexArray(VAOround);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)verts / 6);
        glDisable(GL_BLEND);
        unifiedShader.setFloat("uAlpha", 1.0f);
    }
    if (!isSphere) {
        unifiedShader.setVec3("uMaterial.kD", isBasinHovered ? glm::vec3(0.4f, 0.6f, 0.4f) : glm::vec3(0.6f, 0.6f, 0.6f));
        model = glm::translate(model, basinPos);
        unifiedShader.setMat4("uM", model);
        glBindVertexArray(VAOround);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)verts / 6);
    }
    if (!isSphere && waterLevel > 0.01f) {
        glEnable(GL_BLEND);
        unifiedShader.setVec3("uMaterial.kD", 0.0f, 0.4f, 0.8f);
        glm::mat4 modelWater= model;
        modelWater = glm::translate(modelWater, glm::vec3(0.0f, 0.001f, 0.0f));
        modelWater = glm::scale(modelWater, glm::vec3(0.99f, waterLevel / maxWaterLevel, 0.99f));
        unifiedShader.setMat4("uM", modelWater);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)verts / 6);
        glDisable(GL_BLEND);
    }
}

void drawModel(Shader unifiedShader, Model realModel, bool isAC) {
    unifiedShader.setBool("uUseTexture", true);
    glm::mat4 model = glm::mat4(1.0f);

    depthTestEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    faceCullingEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

    if (isAC) {
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
        model = glm::scale(model, glm::vec3(0.03f));
    }
    else {
        model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.03f));
    }

    unifiedShader.setMat4("uM", model);
    unifiedShader.setVec3("uMaterial.kA", 0.2f, 0.2f, 0.2f);
    unifiedShader.setFloat("uMaterial.shine", 32.0f);
    realModel.Draw(unifiedShader);
}

void drawWECEŠKOLJKA(Shader unifiedShader, Model realModel) {
    unifiedShader.setBool("uUseTexture", true);
    glm::mat4 model = glm::mat4(1.0f);

    depthTestEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    faceCullingEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

    model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.5f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.03f));

    unifiedShader.setMat4("uM", model);
    unifiedShader.setVec3("uMaterial.kA", 0.2f, 0.2f, 0.2f);
    unifiedShader.setFloat("uMaterial.shine", 32.0f);
    realModel.Draw(unifiedShader);
}

void drawFollowingModel(Shader unifiedShader, Model realModel) {
    unifiedShader.setBool("uUseTexture", true);

    depthTestEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    faceCullingEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, cameraPos);

    model = glm::rotate(model, glm::radians(-yaw - 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));

    model = glm::translate(model, glm::vec3(0.3f, -0.325f, -0.8f));

    model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, glm::vec3(0.04f));

    unifiedShader.setMat4("uM", model);
    unifiedShader.setVec3("uMaterial.kA", 0.2f, 0.2f, 0.2f);
    unifiedShader.setFloat("uMaterial.shine", 32.0f);

    realModel.Draw(unifiedShader);
}

void drawCrosshair(Shader unifiedShader, unsigned int VAOcrosshair) {
    glDisable(GL_DEPTH_TEST);

    glm::mat4 identity = glm::mat4(1.0f);
    unifiedShader.setMat4("uV", identity);
    unifiedShader.setMat4("uP", identity);
    unifiedShader.setMat4("uM", identity);

    unifiedShader.setBool("uUseTexture", false);
    unifiedShader.setVec3("uMaterial.kD", 1.0f, 1.0f, 1.0f);

    glBindVertexArray(VAOcrosshair);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glEnable(GL_DEPTH_TEST);
}

void settings(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        depthTestEnabled = true;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        depthTestEnabled = false;
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        faceCullingEnabled = true;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        faceCullingEnabled = false;
    }
}

int main(void) {
    if (!glfwInit()) return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    screenWidth = mode->width;
    screenHeight = mode->height;
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Klima", NULL, NULL);
    glfwSetWindowMonitor(window, monitor, 0, 0, screenWidth, screenHeight, 75);
    if (window == NULL) { glfwTerminate(); return 2; }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) return 3;
    glViewport(0, 0, screenWidth, screenHeight);

    Model ac("Resources/ac.obj");
    Model spinner("Resources/spinner.obj");
    Model remote("Resources/remote.obj");
    Model toilet("Resources/toilet.obj");

    std::map<std::string, float*> verticies = getVerticesMap();
    std::map<std::string, size_t> sizes = getVerticesSizes();

    unsigned int VAOcuboid, VAOпречага, VAOcylinder, VAOsphere, VAOcrosshair;
    int vertsSphere = formRoundVAOs(VAOsphere, true);
    int vertsCylinder = formRoundVAOs(VAOcylinder, false);
    formVAOs(verticies["verticesButton"], sizes["verticesButton"], VAOcuboid);
    formVAOs(verticies["verticesПречага"], sizes["verticesПречага"], VAOпречага);
    form2DVAOs(verticies["verticesCrosshair"], sizes["verticesCrosshair"], VAOcrosshair);

    unsigned int textShader = createShader("Shaders/text.vert", "Shaders/text.frag");
    initTextRendering("Resources/arial.ttf", 48, textShader);

    Shader unifiedShader("Shaders/basic.vert", "Shaders/basic.frag");
    unifiedShader.use();
    unifiedShader.setVec3("uLightPos", 20.0, 2.0, 20.0);
    unifiedShader.setVec3("uLightColor", 1, 1, 1);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.4f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        unifiedShader.use();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        settings(window);
        unifiedShader.setMat4("uP", projection);
        unifiedShader.setMat4("uV", view);
        unifiedShader.setVec3("uViewPos", cameraPos);

        drawCuboid(unifiedShader, VAOcuboid, false, isOn ? 0.6f : 0.25f, isOn ? 0.04f : 0.0f, isOn ? 0.04 : 0.0f);
        drawCuboid(unifiedShader, VAOпречага, true, 1.0f, 1.0f, 1.0f);
        drawModel(unifiedShader, ac, true);
        drawModel(unifiedShader, spinner, false);
        drawWECEŠKOLJKA(unifiedShader, toilet);

        if (!isBasinPickedUp) {
            drawFollowingModel(unifiedShader, remote);
        }

        drawRoundObjects(unifiedShader, VAOcylinder, vertsCylinder, false);
        drawRoundObjects(unifiedShader, VAOsphere, vertsSphere, true);

        drawCrosshair(unifiedShader, VAOcrosshair);

        RenderText(textShader, "Marko Cvijanovic - SV75/2022", 10.0f, screenHeight - 50.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.7f, screenWidth, screenHeight);

        glfwSwapBuffers(window);
        glfwPollEvents();
        while (glfwGetTime() - currentFrame < 1 / 75.0) {}
    }

    glDeleteVertexArrays(1, &VAOcuboid);
    glDeleteVertexArrays(1, &VAOcylinder);
    glDeleteVertexArrays(1, &VAOsphere);
    glDeleteVertexArrays(1, &VAOcrosshair);
    glfwTerminate();
    return 0;
}