/*

#include <tool/shader.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <geometry/SphereGeometry.h>
#include <geometry/TorusGeometry.h>
#include <geometry/PlaneGeometry.h>
#include <geometry/BoxGeometry.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <tool/camera.h>


#define STB_IMAGE_IMPLEMENTATION
#include <tool/stb_image.h>



void framebuffer_size_callback(GLFWwindow* , int , int );
void mouse_callback(GLFWwindow* , double , double );
void scroll_callback(GLFWwindow* , double , double );
void processInput(GLFWwindow* );

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    const char* glsl_version = "#version 330";
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader zprogram
    // ------------------------------------

    // create gui context
    // ---------------------------

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // ----------------------------


    glEnable(GL_DEPTH_TEST);
    Shader lightingShader("Sphere_vertex.glsl", "Sphere_fragment.glsl");
    Shader lightCubeShader("Torus_vertex04.glsl", "Torus_fragment04.glsl");

    SphereGeometry sphergeometry(0.1, 10.0, 10.0);
    //TorusGeometry torusgeomentry(0.5, 0.2, 100);
    //PlaneGeometry planegeometry(0.8, 0.8);
    BoxGeometry boxgeometry(1,1);


    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("D:\\programmeProject\\LearnOpenGL\\image\\container2.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("D:\\programmeProject\\LearnOpenGL\\image\\container2_specular.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    lightCubeShader.use();
    lightCubeShader.setInt("material.diffuse2", 0);
    lightCubeShader.setInt("material.specular2", 1);

    // render loop
    // -----------
    float fov = 45.0f;
    ImVec4 clear_color = ImVec4(0.2, 0.31, 0.1, 1.0);
    glm::vec3 view_translate = glm::vec3(0.0, 0.0, -5.0);
    int screen_WIDTH = 800, screen_HEIGHT = 600;
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Hello, world!");
        ImGui::Text(" %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();


        // render
        // ------
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        lightCubeShader.use(); // don't forget to activate/use the shader before setting uniforms!
        // either set it manually like so:
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        


        lightingShader.use();
        fov = camera.GetZoom();
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)screen_WIDTH / (float)screen_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("view", view);


        glBindVertexArray(sphergeometry.vao);
        glm::mat4 Spheremodel = glm::mat4(1.0f);
        float scaleAmount = sin(glfwGetTime());
        glm::vec3 lightPos = glm::vec3(0.9f, 0.9f, 0.0f);
        Spheremodel = glm::translate(Spheremodel, lightPos);
        Spheremodel = glm::rotate(Spheremodel, (float)glfwGetTime(), glm::vec3(1.0f, scaleAmount, scaleAmount));
        lightingShader.setMat4("model", Spheremodel);
        glDrawElements(GL_LINE_LOOP, sphergeometry.indices.size(), GL_UNSIGNED_INT, 0);
        

        lightCubeShader.use();


        // 传递材质属性
        lightCubeShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        lightCubeShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        lightCubeShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        lightCubeShader.setFloat("material.shininess", 32.0f);

        // 设置光照属性
        lightCubeShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        lightCubeShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); 
        lightCubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        lightCubeShader.setVec3("light.position", lightPos.x, lightPos.y, lightPos.z);
        glBindVertexArray(boxgeometry.vao);
        lightCubeShader.setVec3("InputObjectColor", 0.6f, 0.1f, 0.1f);
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = glm::translate(transform, glm::vec3(-0.8f, -0.8f, scaleAmount));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(scaleAmount, 1.0f, 1.0f));
        lightCubeShader.setMat4("model", transform);
        glDrawElements(GL_TRIANGLES, boxgeometry.indices.size(), GL_UNSIGNED_INT, 0);



        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();


    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    sphergeometry.clearInf();
    boxgeometry.clearInf();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, 1920, 1080);
}

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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)    //  todo 存在问题
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// todo  把纹理添加上

*/