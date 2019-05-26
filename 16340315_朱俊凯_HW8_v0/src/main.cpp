#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

#define MAXSIZE 100

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
    double x;
    double y;
    location(double x1,double y1){
        x=x1;
        y=y1;
    }
};

//初始化
GLFWwindow* window_init();
int GL_loaderinit();
int Shaderinit();
void voa_vob_init(unsigned int &VAO,unsigned int &VBO);
//改变窗口的大小,一个回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//处理键盘输入的一个函数
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void onMouseButton(GLFWwindow* window, int button, int action , int mods) ;

int setup_Circle(unsigned int &VAO,unsigned int &VBO,int shaderProgram,float circle_r);
void plotCirclePixel(int cx, int cy,int x, int y,vector<location>& points);
int set_BezierLine(unsigned int &VAO,unsigned int &VBO,int shaderProgram);
int set_BezierPointsline(unsigned int &VAO,unsigned int &VBO,int shaderProgram);

unsigned long cnr(int n, int r);



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

int mouse_x=0,mouse_y=0;
double SCR_WIDTH=800,SCR_HEIGHT=600;
vector<location> Bezier_points;
bool Point_changed=false;


int main(int, char**)
{
    
    // 初始化windows
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    return 1;
    GLFWwindow* window=window_init();
    if(window==NULL)return 1;
    int shaderProgram=Shaderinit();
    
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window,onMouseButton);
    
    unsigned int VBO,VAO;
    voa_vob_init(VAO,VBO);
    int BezierLine_size=set_BezierLine(VAO, VBO, shaderProgram);
    
    
    unsigned int VBO1,VAO1;
    voa_vob_init(VAO1,VBO1);
    int BezierPointsline_size=0;
    
    //使用着色器
    glUseProgram(shaderProgram);
    
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
            ImGui::End();
        }
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        // 渲染. 所有OpenGL的代码放在这儿
        if(Bezier_points.size()>1){
            BezierPointsline_size=set_BezierPointsline(VAO1, VBO1, shaderProgram);
            glBindVertexArray(VAO1);
            glDrawArrays(GL_LINES, 0,BezierPointsline_size);
        }
        if(Point_changed){
            BezierLine_size=set_BezierLine(VAO, VBO, shaderProgram);
            Point_changed=false;
        }
        float time_counter=fmod(glfwGetTime(),2.0f)/2;
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0,(int)BezierLine_size*time_counter);
        
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
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    mouse_x=xpos;
    mouse_y=ypos;
    return;
}
void onMouseButton( GLFWwindow* window,int button, int action, int mods ) {
    if(action==1){
        if( button==GLFW_MOUSE_BUTTON_RIGHT )
        {
            if(Bezier_points.size()>0)Bezier_points.pop_back();
            Point_changed=true;
        }
        else if( button==GLFW_MOUSE_BUTTON_LEFT)
        {
            double pos_x=(mouse_x/SCR_WIDTH)*2-1;
            double pos_y=1-(mouse_y/SCR_HEIGHT)*2;
            if(fabs(pos_x)>1||fabs(pos_y)>1)return;
            Bezier_points.push_back(location(pos_x,pos_y));
            Point_changed=true;
        }
    }
    return;
}
int set_BezierLine(unsigned int &VAO,unsigned int &VBO,int shaderProgram){
    float step=0.0002;
    vector<location> points;
    int Bezierpoints_size=Bezier_points.size();
    int Bezierpoints_rank=Bezierpoints_size-1;
    for(float t = 0.0; t <= 1.0; t += step){
        float point_x=0,point_y=0,p=0;
        for(int i=0;i<Bezierpoints_size;i++){
            p=cnr(Bezierpoints_rank, i)*pow(1-t, Bezierpoints_rank-i)*pow(t,i);
            point_x+=p*Bezier_points[i].x;
            point_y+=p*Bezier_points[i].y;
        }
        points.push_back(location(point_x,point_y));
    }
    float vertices[points.size()*6];
    int index=0;
    for (std::vector<location>::iterator it = points.begin(); it != points.end(); ++it)
    {
        vertices[index]=(*it).x;
        vertices[index+1]=(*it).y;
        vertices[index+2]=0;
        vertices[index+3]=1;
        vertices[index+4]=1;
        vertices[index+5]=0;
        index+=6;
    }
    glBindVertexArray(VAO);
    
    //把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    //清楚绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return points.size();
}
int set_BezierPointsline(unsigned int &VAO,unsigned int &VBO,int shaderProgram){
    float vertices[(Bezier_points.size()*2)*6];
    int index=0;
    for (std::vector<location>::iterator it = Bezier_points.begin(); it != Bezier_points.end(); ++it)
    {
        vertices[index]=(*it).x;
        vertices[index+1]=(*it).y;
        vertices[index+2]=0;
        vertices[index+3]=0.2f;
        vertices[index+4]=0.2f;
        vertices[index+5]=0.2f;
        index+=6;
        if(it!=Bezier_points.begin()){
            vertices[index]=(*it).x;
            vertices[index+1]=(*it).y;
            vertices[index+2]=0;
            vertices[index+3]=0.2f;
            vertices[index+4]=0.2f;
            vertices[index+5]=0.2f;
            index+=6;
        }
    }
    glBindVertexArray(VAO);
    
    //把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    //清楚绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return (Bezier_points.size()*2-2);
}
unsigned long cnr(int n, int r)
{
    unsigned long c[MAXSIZE];
    int i, j;
    for (i = 0; i <= r; i++)
    c[i] = 1;
    for (i = 1; i <= n - r; i++)
    for (j = 1; j <= r; j++)
    c[j] += c[j - 1];
    return c[r];
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

