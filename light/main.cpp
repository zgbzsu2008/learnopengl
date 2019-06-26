#include <iostream>
#include <learnopengl/shader.hpp>
#include <learnopengl/texture.hpp>
#include <learnopengl/vertex.hpp>
#include <learnopengl/camera.hpp>
#include <learnopengl/shape.hpp>
#include <learnopengl/text.hpp>
#include <learnopengl/shaderpool.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void error_callback(int error, const char* description);

void processInput(GLFWwindow* window);

constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

std::map<int, bool> keys;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

learnopengl::Camera camera(glm::vec3(0.0, 0.0f, 3.0f));

glm::vec3 lightPos(1.2f, 1.5f, 2.0f);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // uncomment this statement to fix compilation on OS X
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetErrorCallback(error_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // anti-aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    learnopengl::ShaderPool pool;
    pool.add({ "mesh.vert", "mesh.frag" }).add({ "text.vert", "text.frag" });

    unsigned int texture1 = learnopengl::textureFromFile("container2.png");
    unsigned int texture2 = learnopengl::textureFromFile("container2_specular.png");

    learnopengl::Shape shape(33, false);
    learnopengl::Text text("SIMYOU.TTF");
    {
        text.setPixelSize(0, 48);
        text.genTextTexture(0, 128);
        text.genTextTexture(u"学习OpenGL3.3版");
        // text.genTextTexture(u'\u4e00', u'\u9fa5');
        // text.genTextTexture(learnopengl::base_chs);
    }

    glm::vec3 cubePositions[] = { { 0.0f, 0.0f, 0.0f },    { 2.0f, 5.0f, -15.f },  { -1.5f, -2.2f, -2.5f },
                                  { -3.8f, -2.0f, -12.f }, { 2.4f, -0.4f, -3.5f }, { -1.7f, 3.0f, -7.5f },
                                  { 1.3f, -2.0f, -2.5f },  { 1.5f, 2.0f, -2.5f },  { 1.5f, 0.2f, -1.5f },
                                  { -1.3f, 1.0f, -1.5f } };

    pool.add({ "light.vert", "light.frag" }).add({ "cube.vert", "cube.frag" });
    learnopengl::Vertex v(8);
    {
        float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, 
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, 
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, 
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, 
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, 
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
             
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
        };
        v.bind(vertices, 288).setVertexAttrib(3, 3, 2);
    }

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float fovy = glm::radians(camera.Zoom);
        float aspect = 1.0f * SCR_WIDTH / SCR_HEIGHT;
        glm::mat4 projection = glm::perspective(fovy, aspect, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);

            auto& shader = pool.get("light").activate();
            shader.bind("projection", projection).bind("view", view);
            shader.bind("material.diffuse", 0).bind("material.specular", 1);
            shader.bind("material.shininess", 64.0f);

            shader.bind("light.direction", camera.Front);
            shader.bind("viewPos", camera.Position);
            shader.bind("light.position", camera.Position);

            glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
            shader.bind("light.ambient", lightColor * glm::vec3(0.2f));
            shader.bind("light.diffuse", lightColor * glm::vec3(0.5f));
            shader.bind("light.specular", lightColor * glm::vec3(1.0f));

            shader.bind("light.constant", 1.0f);
            shader.bind("light.linear", 0.09f);
            shader.bind("light.quadratic", 0.032f);
            shader.bind("light.cutOff", glm::cos(glm::radians(12.5f)));
            shader.bind("light.outerCutOff", glm::cos(glm::radians(17.5f)));

            for (unsigned int i = 0; i < 10; i++) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 10.0f * i;
                model = glm::rotate(model, glm::radians(angle * (float)glfwGetTime()), glm::vec3(1.0f, 0.3f, 0.5f));
                shader.bind("model", model);
                v.render();
            }
        }
        {
            pool.get("cube").activate().bind("projection", projection).bind("view", view);
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos);
            model = glm::scale(model, glm::vec3(0.1f));
            pool.get("cube").bind("model", model);
            v.render();
        }
        {
            pool.get("mesh").activate().bind("projection", projection).bind("view", view);
            pool.get("text").activate().bind("projection", projection).bind("view", view);
            shape.render(pool.get("mesh"), GL_LINES).render(pool.get("text"), text);
        }
        {

            model = glm::mat4(1.0f);
            pool.get("text").activate().bind("projection", projection).bind("view", view).bind("model", model);
            text.render(pool.get("text"), u"学习OpenGL3.3版", glm::vec3(1.0f, 1.0f, 2.0f), 2.0f,
                        glm::vec3(1.0f, 0.5f, 1.0f));
            // ortho projection
            text.render(pool.get("text"), u"学习OpenGL3.3版", glm::vec2(10.0f, 10.0f), 1.0f);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    using namespace learnopengl;
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
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
}

void error_callback(int error, const char* description)
{
    std::cout << "Error: " << error << ": " << description << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    std::cout << "Mouse move: " << xpos << ", " << ypos << std::endl;
    if (firstMouse) {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos);

    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    std::cout << "Mouse scroll: " << xoffset << ", " << yoffset << std::endl;
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        keys[key] = true;
        std::cout << "Press   key code: " << key << std::endl;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
        std::cout << "Release key code: " << key << std::endl;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                std::cout << "Press mouse button left" << std::endl;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                std::cout << "Press mouse button middle" << std::endl;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                std::cout << "Press mouse button right" << std::endl;
                break;
            default:
                break;
        }
    }
}