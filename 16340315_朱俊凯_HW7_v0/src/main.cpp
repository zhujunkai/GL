#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
using namespace std;

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif
#include <GLFW/glfw3.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description){
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
//初始化
GLFWwindow* window_init();
int GL_loaderinit();
int Shaderinit(const char* vertexPath, const char* fragmentPath);
void voa_vob_init(unsigned int &VAO,unsigned int &VBO);
void RenderScene(int &shaderprogram);
//改变窗口的大小,一个回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//处理键盘输入的一个函数
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void setup_plane(unsigned int &VAO,unsigned int &VBO,float vertices[],unsigned int vertices_size);
void setup_Cube(unsigned int &VAO,unsigned int &VBO,float vertices[],unsigned int vertices_size);
float getmytime(float time_circle_long);
void glsetvec3(int shader,char* name,glm::vec3 value);
void glset3f(int shader,char* name,float x,float y,float z);
//着色器
const char *vertexShaderSource =nullptr;
const char *fragmentShaderSource = nullptr;

Camera camera(glm::vec3(1.0f, 2.0f, 10.0f));
unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int SRC_WIDTH = 800, SRC_HEIGHT = 600;
float lastX = 400;
float lastY = 300;
bool firstMouse=true;
bool mouse_start=false;
unsigned int CubeVAO,CubeVBO;
unsigned int PlaneVAO,PlaneVBO;

int main(int, char**)
{
    
    // 初始化windows
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    return 1;
    GLFWwindow* window=window_init();
    if(window==NULL)return 1;
    int shaderProgram=Shaderinit("vs","fs");
    int shaderDepth=Shaderinit("vs2","fs2");
    //建立三角形
    float Cube_vertices[]={
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    voa_vob_init(CubeVAO,CubeVBO);
    setup_Cube(CubeVAO,CubeVBO,Cube_vertices ,sizeof(Cube_vertices));
    float Plane_vertices[]={
        -12.0f,  -0.5f, -12.0f,  0.0f,  1.0f,  0.0f,
        12.0f, -0.5f, -12.0f,  0.0f,  1.0f,  0.0f,
        -12.0f,  -0.5f, 12.0f,  0.0f,  1.0f,  0.0f,
        12.0f, -0.5f, 12.0f,  0.0f,  1.0f,  0.0f,
        12.0f, -0.5f, -12.0f,  0.0f,  1.0f,  0.0f,
        -12.0f, -0.5f, 12.0f,  0.0f,  1.0f,  0.0f
    };
    voa_vob_init(PlaneVAO,PlaneVBO);
    setup_plane(PlaneVAO,PlaneVBO,Plane_vertices ,sizeof(Plane_vertices));
    
    //使用着色器
    glUseProgram(shaderProgram);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glUniform1i(glGetUniformLocation(shaderProgram, "shadowMap"), 0);
    
    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glfwSetCursorPosCallback(window, mouse_callback);
    
    
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    
    glm::mat4 projection_persp = glm::perspective(glm::radians(45.0f), (float)800/(float)600, 0.1f, 100.0f);
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_persp));
    
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos(4.5f, 4.0f, 12.0f);
    float ambient[3]={1.0f, 0.5f, 0.31f};
    float diffuse[3]={1.0f, 0.5f, 0.31f};
    float specular[3]={0.5f, 0.5f, 0.5f};
    float shininess=32.0f;
    bool if_per=false,if_bias=true;
    float near=1.0f,far=50.0f;
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // 键盘输入处理
        processInput(window);
        glfwPollEvents();
        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("My, Setting!");                          // Create a window called "Hello, world!"
            ImGui::SliderFloat3("ambient", ambient, 0.0f, 1.0f);
            ImGui::SliderFloat3("diffuse", diffuse, 0.0f, 1.0f);
            ImGui::SliderFloat3("specular", specular, 0.0f, 1.0f);
            ImGui::SliderFloat("shininess", &shininess, 2.0f, 256.0f);
            ImGui::Checkbox("if_per", &if_per);
            ImGui::Checkbox("if_Shadow Acne", &if_bias);
            ImGui::Text("GL HW!");               // Display some text (you
            ImGui::End();
        }
        glUseProgram(shaderDepth);
        glm::mat4 lightProjection;
        if(if_per){
            lightProjection = glm::perspective(glm::radians(45.0f), (float)800/(float)600, near, far);
        }else{
            lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near, far);
        }
        glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        glUniformMatrix4fv(glGetUniformLocation(shaderDepth,"lightprojection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
        glUniformMatrix4fv(glGetUniformLocation(shaderDepth,"lightview"), 1, GL_FALSE, glm::value_ptr(lightView));
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        RenderScene(shaderDepth);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        // 渲染. 所有OpenGL的代码放在这儿
        int t_shader=shaderProgram;
        glUseProgram(t_shader);
        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
        glsetvec3(t_shader,"light.position", lightPos);
        glsetvec3(t_shader, "viewPos", camera.cameraPos);
        glsetvec3(t_shader, "light.ambient", ambientColor);
        glsetvec3(t_shader, "light.diffuse", diffuseColor);
        glset3f(t_shader,"light.specular", 1.0f, 1.0f, 1.0f);
        glset3f(t_shader,"material.ambient", ambient[0], ambient[1], ambient[2]);
        glset3f(t_shader,"material.diffuse", diffuse[0], diffuse[1], diffuse[2]);
        glset3f(t_shader,"material.specular", specular[0], specular[1], specular[2]);
        glUniform1f(glGetUniformLocation(t_shader,"material.shininess"),shininess);
        glUniform1f(glGetUniformLocation(t_shader,"ifPers"),if_per?1:-1);
        glUniform1f(glGetUniformLocation(t_shader,"ifBias"),if_bias?1:-1);
        glUniform1f(glGetUniformLocation(t_shader,"near_plane"),near);
        glUniform1f(glGetUniformLocation(t_shader,"far_plane"),far);
        glUniformMatrix4fv(glGetUniformLocation(t_shader,"projection"), 1, GL_FALSE, glm::value_ptr(projection_persp));
        glUniformMatrix4fv(glGetUniformLocation(t_shader,"view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glUniformMatrix4fv(glGetUniformLocation(t_shader,"model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"lightprojection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"lightview"), 1, GL_FALSE, glm::value_ptr(lightView));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        RenderScene(shaderProgram);
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.moveForward(0.05f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.moveBack(0.05f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.moveLeft(0.05f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.moveRight(0.05f);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        mouse_start=true;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
        mouse_start=false;
        firstMouse=true;
    }
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(mouse_start){
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
        
        camera.rotate(yoffset*0.2f, xoffset*0.2f);
    }
}

void setup_plane(unsigned int &VAO,unsigned int &VBO,float vertices[],unsigned int vertices_size){
    //绑定顶点数组对象
    glBindVertexArray(VAO);
    
    //把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_DYNAMIC_DRAW);
    //清楚绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void setup_Cube(unsigned int &VAO,unsigned int &VBO,float vertices[],unsigned int vertices_size){
    //绑定顶点数组对象
    glBindVertexArray(VAO);
    
    //把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_DYNAMIC_DRAW);
    //清楚绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
float getmytime(float time_circle_long){
    return fabs(2*fmod((float)glfwGetTime(),time_circle_long)/time_circle_long-1);
}
void glsetvec3(int shader,char* name,glm::vec3 value){
    glUniform3f(glGetUniformLocation(shader,name), value.x,value.y,value.z);
}
void glset3f(int shader,char* name,float x,float y,float z){
    glUniform3f(glGetUniformLocation(shader,name), x,y,z);
}

GLFWwindow* window_init(){
    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
    
    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    
    
    
    //注册这个函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    
    if(GL_loaderinit()==1)return NULL;
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    ImGui::StyleColorsDark();
    
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    return window;
}
int GL_loaderinit(){
    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }
    return 0;
}
int Shaderinit(const char* vertexPath, const char* fragmentPath){
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::stringstream vShaderStream, fShaderStream;
    std::string vertexCode;
    std::string fragmentCode;
    try{
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        // close file handlers
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    // convert stream into string
    vertexShaderSource = vertexCode.c_str();
    fragmentShaderSource = fragmentCode.c_str();
    //顶点着色器
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    //检测着色器是否编译错误
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    //片段着色器
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    //检测着色器是否编译错误
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    //连接着色器
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //检测着色器是否编译错误
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}
void voa_vob_init(unsigned int &VAO,unsigned int &VBO){
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);
    //绑定顶点数组对象
    glBindVertexArray(VAO);
    //把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //设置顶点属性指针
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    //设置顶点属性指针
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    //清楚绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void RenderScene(int &shaderprogram)
{
    // Floor
    glm::mat4 model= glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(PlaneVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    // Cubes
    model == glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.5f, 1.8f, 1.5f));
    model = glm::scale(model, glm::vec3(0.5));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(CubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(CubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 1.0f, -1.0f));
    model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(CubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
