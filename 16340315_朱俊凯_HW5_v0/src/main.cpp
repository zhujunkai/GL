#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
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

class location{
public:
    int x;
    int y;
    location(int x1,int y1){
        x=x1;
        y=y1;
    }
};

//初始化
GLFWwindow* window_init();
int GL_loaderinit();
int Shaderinit();
void voa_vob_init(unsigned int &VAO,unsigned int &VBO);
void voa_vob_ebo_init(unsigned int &VAO,unsigned int &VBO,unsigned int &EBO);
//改变窗口的大小,一个回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//处理键盘输入的一个函数
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void setup_Cube(unsigned int &VAO,unsigned int &VBO,unsigned int &EBO,float vertices[],unsigned int vertices_size,unsigned int indices[],unsigned int indices_size);
float getmytime(float time_circle_long);


void swap(float &num1,float &num2){
    float temp=num1;
    num1=num2;
    num2=temp;
}
bool comp(const location &a, const location &b){
    return a.y < b.y;
}
//着色器
const char *vertexShaderSource ="#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec3 ourColor;\n"
"uniform mat4 transform;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"uniform vec3 aColor;\n"
"void main()\n"
"{\n"
"   gl_Position =projection* view * transform *  vec4(aPos, 1.0);\n"
"   ourColor=aColor;\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor =  vec4(ourColor,1.0f);\n"
"}\n\0";

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = 400;
float lastY = 300;
bool firstMouse=true;

int main(int, char**)
{
    
    // 初始化windows
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    GLFWwindow* window=window_init();
    if(window==NULL)return 1;
    int shaderProgram=Shaderinit();
    
    //建立三角形
    float vertices[]={
        2.0f,2.0f,2.0f,
        2.0f,-2.0f,2.0f,
        -2.0f,-2.0f,2.0f,
        -2.0f,2.0f,2.0f,
        2.0f,2.0f,-2.0f,
        2.0f,-2.0f,-2.0f,
        -2.0f,-2.0f,-2.0f,
        -2.0f,2.0f,-2.0f
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,
        0, 2, 3, //后
        4, 5, 6,
        4, 6, 7, //前
        0, 1, 5,
        0, 5, 4, //右
        2, 3, 7,
        2, 7, 6, //左
        0, 3, 7,
        0, 7, 4, //上
        1, 2, 6,
        1, 6, 5  //下
    };
    unsigned int EBO,VBO,VAO;
    voa_vob_ebo_init(VAO,VBO,EBO);
    setup_Cube(VAO, VBO, EBO,vertices ,sizeof(vertices), indices,sizeof(indices));
    
    
    //使用着色器
    glUseProgram(shaderProgram);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    glfwSetCursorPosCallback(window, mouse_callback);
    
    
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    
    glm::mat4 projection_ortho =glm::ortho(-6.0f, 6.0f, -4.5f, 4.5f, 0.1f, 100.0f);
    glm::mat4 projection_persp = glm::perspective(glm::radians(45.0f), (float)800/(float)600, 0.1f, 100.0f);
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_persp));
    
    unsigned int aColorLoc = glGetUniformLocation(shaderProgram, "aColor");
    glUniform3f(aColorLoc, 1.0f,0.0f,0.0f);
    
    bool hw1=false,hw2=false,hw3=false,t_projection=false;
    float lrbt[4]={-6.0f, 6.0f, -4.5f, 4.5f};
    float near_far[2]={0.1f, 100.0f};
    float radians=45.0f;
    float w_div_h=(float)800/(float)600;
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
            ImGui::Text("GL HW!");               // Display some text (you can use a
            ImGui::Checkbox("If Use hw1", &hw1);
            ImGui::Checkbox("If Use hw2", &hw2);
            ImGui::Checkbox("If Use hw3", &hw3);
            ImGui::Checkbox("If Use perspective", &t_projection);
            ImGui::SliderFloat4("L_R_B_T", lrbt, -40.0f, 40.0f);
            ImGui::SliderFloat2("Near&Far", near_far, -10.0f, 100.0f);
            ImGui::SliderFloat("Radians", &radians, 10.0f, 80.0f);
            ImGui::SliderFloat("W_Div_H", &w_div_h, 0.2f, 4.0f);
            ImGui::End();
        }
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        // 渲染. 所有OpenGL的代码放在这儿
        if(hw1){
            projection_ortho =glm::ortho(lrbt[0], lrbt[1], lrbt[2], lrbt[3], near_far[0], near_far[1]);
            projection_persp = glm::perspective(glm::radians(radians), w_div_h,  near_far[0], near_far[1]);
        }
        if(t_projection){
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_persp));
        }else{
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_ortho));
        }
        if(hw2){
            float radius = 10.0f;
            float camX = sin(glfwGetTime()) * radius;
            float camZ = cos(glfwGetTime()) * radius;
            glm::mat4 view2;
            view2 = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view2));
        }
        if (hw3) {
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        }
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        if(hw1){
            transform = glm::translate(transform,glm::vec3(-1.5f, 0.5f, -1.5f));
        }
        glUseProgram(shaderProgram);
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        
        glBindVertexArray(VAO);
        glUniform3f(aColorLoc, 1.0f,0.0f,0.0f);
        glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT, 0);
        glUniform3f(aColorLoc, 0.0f,1.0f,0.0f);
        glDrawElements(GL_TRIANGLES, 12,GL_UNSIGNED_INT,0);
        glUniform3f(aColorLoc, 0.0f,0.0f,1.0f);
        glDrawElements(GL_TRIANGLES, 18,GL_UNSIGNED_INT, 0);
        glUniform3f(aColorLoc, 1.0f,0.0f,1.0f);
        glDrawElements(GL_TRIANGLES, 24,GL_UNSIGNED_INT, 0);
        glUniform3f(aColorLoc, 1.0f,1.0f,0.0f);
        glDrawElements(GL_TRIANGLES, 30,GL_UNSIGNED_INT,0);
        glUniform3f(aColorLoc, 0.0f,1.0f,1.0f);
        glDrawElements(GL_TRIANGLES, 36,GL_UNSIGNED_INT, 0);

        // Rendering
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
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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


void setup_Cube(unsigned int &VAO,unsigned int &VBO,unsigned int &EBO,float vertices[],unsigned int vertices_size,unsigned int indices[],unsigned int indices_size){
    //绑定顶点数组对象
    glBindVertexArray(VAO);
    
    //把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_DYNAMIC_DRAW);
    //清楚绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
float getmytime(float time_circle_long){
    return fabs(2*fmod((float)glfwGetTime(),time_circle_long)/time_circle_long-1);
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
int Shaderinit(){
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
void voa_vob_ebo_init(unsigned int &VAO,unsigned int &VBO,unsigned int &EBO){
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //绑定顶点数组对象
    glBindVertexArray(VAO);
    //把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //设置顶点属性指针
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    //清楚绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
