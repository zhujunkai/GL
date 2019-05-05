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

void setup_Cube(unsigned int &VAO,unsigned int &VBO,unsigned int &EBO,float vertices[],unsigned int vertices_size,unsigned int indices[],unsigned int indices_size);
float getmytime(float time_circle_long);
int setup_Ball(unsigned int &VAO,unsigned int &VBO,float cx,float cy,float cz,float color_r,float color_g,float color_b,float circle_r);


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
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"uniform mat4 transform;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position =projection* view * transform *  vec4(aPos, 1.0);\n"
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
    
    // 初始化windows
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    GLFWwindow* window=window_init();
    if(window==NULL)return 1;
    int shaderProgram=Shaderinit();
    
    //建立三角形
    float vertices[]={
        2.0f,2.0f,2.0f,0.0f,0.0f,1.0f,
        2.0f,-2.0f,2.0f,0.0f,0.0f,1.0f,
        -2.0f,-2.0f,2.0f,0.0f,0.0f,1.0f,
        -2.0f,2.0f,2.0f,0.0f,0.0f,1.0f,
        2.0f,2.0f,-2.0f,1.0f,1.0f,0.0f,
        2.0f,-2.0f,-2.0f,1.0f,1.0f,0.0f,
        -2.0f,-2.0f,-2.0f,1.0f,1.0f,0.0f,
        -2.0f,2.0f,-2.0f,1.0f,1.0f,0.0f
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
    
    unsigned int VBO1,VAO1;
    voa_vob_init(VAO1, VBO1);
    int earthsize=setup_Ball(VAO1, VBO1,0,0.0f,0,0,0,1, 0.3f);
    
    unsigned int VBO2,VAO2;
    voa_vob_init(VAO2, VBO2);
    int sunsize=setup_Ball(VAO2, VBO2,0,0,0,1,0,0, 0.6f);
    
    
    //使用着色器
    glUseProgram(shaderProgram);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800/(float)600, 0.1f, 100.0f);
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    bool t_Translation=false,t_Rotation=false,t_Scaling=false,t_earth_run=false;
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
            ImGui::Checkbox("If Use Translation", &t_Translation);
            ImGui::Checkbox("If Use Rotation", &t_Rotation);
            ImGui::Checkbox("If Use Scaling", &t_Scaling);
            ImGui::Checkbox("Earth run", &t_earth_run);
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
        if(!t_earth_run){
            glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            if(t_Translation){
                float my_time = getmytime(2);
                float x_trans=my_time-0.5;
                transform = glm::translate(transform,glm::vec3(x_trans*4, 0.0f, 0.0f));
            }
            if(t_Scaling){
                float my_time = getmytime(3);
                float x_scale=fabs(my_time-0.5f)+0.5;
                transform=glm::scale(transform, glm::vec3(fabs(my_time-0.5f)+0.5, fabs(my_time-0.5f)+0.5, fabs(my_time-0.5f)+0.5));
            }
            if(t_Rotation){
                transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 1.0f));
            }
            glUseProgram(shaderProgram);
            unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36,GL_UNSIGNED_INT, 0);
        }
        else{
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.2f, 1.0f, 0.0f));
            transform = glm::translate(transform,glm::vec3(3.0f, -0.6f, 0.0f));
            unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            glBindVertexArray(VAO1);
            glDrawArrays(GL_TRIANGLES, 0,earthsize);
            
            glm::mat4 transform2 = glm::mat4(1.0f);
            unsigned int transformLoc2 = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(transform2));
            glBindVertexArray(VAO2);
            glDrawArrays(GL_TRIANGLES, 0,sunsize);
        }

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
GLfloat * calculateBallVertex(GLfloat r,int &size,float cx,float cy,float cz,float color_r,float color_g,float color_b)
{
    //    x=x0+r sinθcosφ
    //    y=y0+r sinθsinφ
    //    z=z0+r cosθ
    
    GLfloat radius = 0.8 * r;
    int kRow=100,kCol=100;
    float kHeight=1,kWidth=1;
    
    // GLfloat delta = 2 * M_PI / 100;
    GLfloat *dest = (GLfloat *) malloc(sizeof(GLfloat) * 6 * kRow * kCol * 6);
    
    GLfloat k = kHeight * 1.0 / kWidth;
    
    float step_z = M_PI / kRow ;  //z方向每次步进的角度(上下，纬度)
    float step_xy = 2*M_PI/ kCol;  //x,y平面每次步进的角度（经度）
    float x[2],y[2],z[2];  //用来存坐标
    
    float angle_z = 0; //起始角度
    float angle_xy = 0;
    int i=0, j=0;
    int count =0;
    // 90 -->  -90
    for(i = 0; i < kRow; i++)
    {
        angle_z = i * step_z;  //每次步进step_z
        float first_x=0,first_y=0,first_z=0,first_x1=0,first_y1=0,first_z1=0;
        float last_x=0,last_y=0,last_z=0,last_x1=0,last_y1=0,last_z1=0;
        for(j = 0; j < kCol ; j++)
        {
            // j++;
            angle_xy = j * step_xy;  //每次步进step_xy
            //整个的过程可以想象3D打印机，一层一层的画出来
            x[0] = k * radius * sin(angle_z) * cos(angle_xy);  //第一个小平面的第一个顶点坐标
            y[0] = radius * sin(angle_z) * sin(angle_xy);
            z[0] = radius * cos(angle_z);
            
            x[1] = k * radius * sin(angle_z + step_z) * cos(angle_xy);  //第一个小平面的第二个顶点坐标，下面类似
            y[1] = radius * sin(angle_z + step_z) * sin(angle_xy);
            z[1] = radius * cos(angle_z + step_z);
            if(j==0){
                first_x=x[0];first_y=y[0];first_z=z[0];first_x1=x[1];first_y1=y[1];first_z1=z[1];
                last_x=x[0];last_y=y[0];last_z=z[0];last_x1=x[1];last_y1=y[1];last_z1=z[1];
            }else{
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  0*6+0] = cx+last_x;
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  0*6+1] = cy+last_y;
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  0*6+2] = cz+last_z;
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  1*6+0] = cx+last_x1;
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  1*6+1] = cy+last_y1;
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  1*6+2] = cz+last_z1;
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  2*6+0] = cx+x[0];
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  2*6+1] = cy+y[0];
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  2*6+2] = cz+z[0];
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  3*6+0] = cx+last_x1;
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  3*6+1] = cy+last_y1;
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  3*6+2] = cz+last_z1;
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  4*6+0] = cx+x[0];
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  4*6+1] = cy+y[0];
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  4*6+2] = cz+z[0];
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  5*6+0] = cx+x[1];
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  5*6+1] = cy+y[1];
                dest[i * kCol * 6 * 6 + j * 6 * 6 +  5*6+2] = cz+z[1];
                for(int m=0;m<6;m++){
                    dest[i * kCol * 6 * 6 + j * 6 * 6 +  m*6+3] = color_r;
                    dest[i * kCol * 6 * 6 + j * 6 * 6 +  m*6+4] = color_g;
                    dest[i * kCol * 6 * 6 + j * 6 * 6 +  m*6+5] = color_b;
                }
                last_x=x[0];last_y=y[0];last_z=z[0];last_x1=x[1];last_y1=y[1];last_z1=z[1];
            }
            if(j==kCol-1){
                dest[i * kCol * 6 * 6 + 0*6+0] = cx+x[0];
                dest[i * kCol * 6 * 6 + 0*6+1] = cy+y[0];
                dest[i * kCol * 6 * 6 + 0*6+2] = cz+z[0];
                dest[i * kCol * 6 * 6 + 1*6+0] = cx+x[1];
                dest[i * kCol * 6 * 6 + 1*6+1] = cy+y[1];
                dest[i * kCol * 6 * 6 + 1*6+2] = cz+z[1];
                dest[i * kCol * 6 * 6 + 2*6+0] = cx+first_x;
                dest[i * kCol * 6 * 6 + 2*6+1] = cy+first_y;
                dest[i * kCol * 6 * 6 + 2*6+2] = cz+first_z;
                dest[i * kCol * 6 * 6 + 3*6+0] = cx+x[1];
                dest[i * kCol * 6 * 6 + 3*6+1] = cy+y[1];
                dest[i * kCol * 6 * 6 + 3*6+2] = cz+z[1];
                dest[i * kCol * 6 * 6 + 4*6+0] = cx+first_x;
                dest[i * kCol * 6 * 6 + 4*6+1] = cy+first_y;
                dest[i * kCol * 6 * 6 + 4*6+2] = cz+first_z;
                dest[i * kCol * 6 * 6 + 5*6+0] = cx+first_x1;
                dest[i * kCol * 6 * 6 + 5*6+1] = cy+first_y1;
                dest[i * kCol * 6 * 6 + 5*6+2] = cz+first_z1;
                for(int m=0;m<6;m++){
                    dest[i * kCol * 6 * 6 + m*6+3] = color_r;
                    dest[i * kCol * 6 * 6 + m*6+4] = color_g;
                    dest[i * kCol * 6 * 6 + m*6+5] = color_b;
                }
            }
        } //循环画出这一层的平面，组成一个环
    }  //z轴++，画出剩余层
    size=sizeof(float)*6 * kRow * kCol * 6;
    return dest;
}
int setup_Ball(unsigned int &VAO,unsigned int &VBO,float cx,float cy,float cz,float color_r,float color_g,float color_b,float circle_r){
    int size=0;
    float *vertices=calculateBallVertex(circle_r,size,cx,cy,cz,color_r,color_g,color_b);
    //绑定顶点数组对象
    glBindVertexArray(VAO);
    //把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
    //清楚绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    cout<<size<<endl;
    return size/6;
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
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    //设置顶点属性指针
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    //清楚绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
