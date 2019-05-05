#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include <vector>
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

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

//改变窗口的大小,一个回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//处理键盘输入的一个函数
void processInput(GLFWwindow *window);


//使用GLSL编写出来的顶点着色器和片段着色器的代码
//顶点着色器主要是把输入的数据转化为标准化设备坐标，把它们转换至OpenGL的可视区域内
//片段着色器所做的是计算像素最后的颜色输出，可以理解为我们所画的图元“上色”
//这两个着色器程序都是最简单的程序，没有对输入的数据做任何处理就输出了
const char *vertexShaderSource ="#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";



int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    
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
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    
    
    
    //注册这个函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    
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
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
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
    
    //建立三角形
    float vertices[] = {
        // 位置         // 颜色
        0.0f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // 顶点
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // 底边左点
        0.5f,  -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 底边右点
        0.5f,  0.6f, 0.0f,  1.0f, 0.5f, 0.5f,   // 正方形点
        0.75f,  0.6f, 0.0f,  0.5f, 0.0f, 1.0f,   // 正方形点
        0.75f,  0.85f, 0.0f,  0.0f, 0.5f, 0.5f,   // 正方形点
        0.5f,  0.85f, 0.0f,  0.5f, 1.0f, 0.0f,   // 正方形点
        0.75f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,   // 线段上点
        0.75f,  -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 线段下点
        0.75f,  0.4f, 0.0f,  1.0f, 0.0f, 0.0f,   // 点
        0.75f,  0.3f, 0.0f,  1.0f, 0.0f, 0.0f,   // 点
        0.75f,  0.2f, 0.0f,  1.0f, 0.0f, 0.0f,   // 点
        0.75f,  0.1f, 0.0f,  1.0f, 0.0f, 0.0f,   // 点
    };
    
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,  // first Triangle
        3, 4, 5,   // second Triangle
        3, 5, 6   // third Triangle
    };
    

    
    
    bool is_total = false;
    vector<ImVec4> colors;
    ImVec4 total_color = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
    colors.push_back(ImVec4(1.0f, 0.0f, 0.0f, 1.00f));
    colors.push_back(ImVec4(0.0f, 1.0f, 0.0f, 1.00f));
    colors.push_back(ImVec4(0.0f, 0.0f, 1.0f, 1.00f));

    
    
    
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
            ImGui::Text("Set the Color!");               // Display some text (you can use a
            ImGui::ColorEdit3("Top color", (float*)&colors[0]); // Edit 3 floats representing a color
            ImGui::ColorEdit3("Left color", (float*)&colors[1]); // Edit 3 floats representing a color
            ImGui::ColorEdit3("Right color", (float*)&colors[2]); // Edit 3 floats representing a color
            ImGui::Checkbox("If Change The Total Color", &is_total);
            ImGui::ColorEdit3("Total COLOR", (float*)&total_color, 1);
            ImGui::End();
        }
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // 渲染. 所有OpenGL的代码放在这儿
        if(is_total){
            for (int i = 0; i < 3; i++) {
                vertices[i * 6 + 3] = total_color.x;
                vertices[i * 6 + 4] = total_color.y;
                vertices[i * 6 + 5] = total_color.z;
            }
        }
        else{
            for (int i = 0; i < 3; i++) {
                vertices[i * 6 + 3] = colors[i].x;
                vertices[i * 6 + 4] = colors[i].y;
                vertices[i * 6 + 5] = colors[i].z;
            }
        }
        
        unsigned int VBO,VAO,EBO;
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        //绑定顶点数组对象
        glBindVertexArray(VAO);
        
        //把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        
        //复制我们的索引数组到一个索引缓冲中，供OpenGL使用
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
        //设置顶点属性指针
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
        
        //设置顶点属性指针
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        
        //清楚绑定
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        //使用着色器
        glUseProgram(shaderProgram);
        
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 9,GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_LINES, 7, 2);
        glDrawArrays(GL_POINTS, 9, 4);
        
        
        
        
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
}
