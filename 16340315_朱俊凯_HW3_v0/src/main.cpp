#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
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
//改变窗口的大小,一个回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//处理键盘输入的一个函数
void processInput(GLFWwindow *window);

vector<location> getLine(float x,float y,float x1,float y1);
vector<location> getTriangle(float x,float y,float x1,float y1,float x2,float y2);
vector<location> getinsidepoint(float x,float y,float x1,float y1,vector<location>& result,vector<location>& result1);
int setup_Triangle(float x,float y,float x1,float y1,float x2,float y2,unsigned int &VAO,unsigned int &VBO,int shaderProgram);
int setup_Circle(unsigned int &VAO,unsigned int &VBO,int shaderProgram,float circle_r);
void plotCirclePixel(int cx, int cy,int x, int y,vector<location>& points);


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
    float x_list[3]={0.0f,0.3f,-0.5f};
    float y_list[3]={-0.1f,0.7f,0.2f};
    unsigned int VBO,VAO;
    voa_vob_init(VAO,VBO);
    int Triangle_size=setup_Triangle(x_list[0],y_list[0],x_list[1],y_list[1],x_list[2],y_list[2],VAO, VBO, shaderProgram);
    
    unsigned int VBO1,VAO1;
    voa_vob_init(VAO1,VBO1);
    int Circle_size=setup_Circle(VAO1, VBO1, shaderProgram,0.5f);
    
    //使用着色器
    glUseProgram(shaderProgram);
    bool is_circle = false;
    float circle_r=0.3f;
    
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
            ImGui::SliderFloat3("Triangle_x", x_list, -0.9f, 0.9f);
            ImGui::SliderFloat3("Triangle_y", y_list, -0.9f, 0.9f);
            ImGui::Checkbox("If Use Circle", &is_circle);
            ImGui::SliderFloat("circle_r", &circle_r, 0.1f, 0.9f);
            ImGui::End();
        }
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // 渲染. 所有OpenGL的代码放在这儿
        
        if(is_circle){
            Circle_size=setup_Circle(VAO1, VBO1, shaderProgram,circle_r);
            glBindVertexArray(VAO1);
            glDrawArrays(GL_POINTS, 0,Circle_size);
        }else{
            int Triangle_size=setup_Triangle(x_list[0],y_list[0],x_list[1],y_list[1],x_list[2],y_list[2],VAO, VBO, shaderProgram);
            glBindVertexArray(VAO);
            glDrawArrays(GL_POINTS, 0,Triangle_size);
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

vector<location> getLine(float x,float y,float x1,float y1){
    int x_t=(int)(x*400),y_t=(int)(y*300),x1_t=(int)(x1*400),y1_t=(int)(y1*300);
    int n=fabs(x1_t-x_t);
    int m=fabs(y1_t-y_t);
    vector<location> result;
    int x_flag=(x_t<x1_t?1:-1);
    int y_flag=(y_t<y1_t?1:-1);
    if(n>m){                 //斜率k<1
        int last_f=0;
        int t_2dy_div_dx=2*m-n;
        int last_x=0;
        int last_y=0;
        result.push_back(location(x_t,y_t));
        for(int i=0;i<n;i++){
            if(last_f+t_2dy_div_dx<0){
                last_x+=1;
                last_y+=0;              //(1,0)
                location a(x_t+last_x*x_flag,y_t+last_y*y_flag);
                result.push_back(a);
                last_f+=2*m;            //f+=2dy
            }else{
                last_x+=1;
                last_y+=1;              //(1,1)
                location a(x_t+last_x*x_flag,y_t+last_y*y_flag);
                result.push_back(a);
                last_f+=2*m-2*n;            //f+=2dy-2dx
            }
        }
    }else{
        int last_f=0;
        int t_2dx_div_dy=2*n-m;
        int last_x=0;
        int last_y=0;
        result.push_back(location(x_t,y_t));
        for(int i=0;i<m;i++){
            if(last_f+t_2dx_div_dy<0){
                last_x+=0;
                last_y+=1;              //(1,0)
                location a(x_t+last_x*x_flag,y_t+last_y*y_flag);
                result.push_back(a);
                last_f+=2*n;            //f+=2dx
            }else{
                last_x+=1;
                last_y+=1;              //(1,1)
                location a(x_t+last_x*x_flag,y_t+last_y*y_flag);
                result.push_back(a);
                last_f+=2*n-2*m;            //f+=2dx-cdy
            }
        }
    }
    return result;
}
vector<location> getTriangle(float x,float y,float x1,float y1,float x2,float y2){
    vector<location> result=getLine(x, y, x1, y1);
    vector<location> result1=getLine(x, y, x2, y2);
    vector<location> result2=getLine(x1, y1, x2, y2);
    sort(result.begin(),result.end(),comp);
    sort(result1.begin(),result1.end(),comp);
    sort(result2.begin(),result2.end(),comp);
    vector<location> results[]={result2,result1,result};
    float x_list[3]={x,x1,x2};
    float y_list[3]={y,y1,y2};
    for(int i=0;i<2;i++){
        for (int j=i+1;j<3;j++){
            if(y_list[i]>y_list[j]){
                swap(x_list[i],x_list[j]);
                swap(y_list[i],y_list[j]);
                results[i].swap(results[j]);
            }
        }
    }
    vector<location> result3=getinsidepoint(x_list[1],y_list[1],x_list[0],y_list[0],results[1],results[2]);
    vector<location> result4=getinsidepoint(x_list[1],y_list[1],x_list[2],y_list[2],results[1],results[0]);
    result.insert(result.end(), result1.begin(),result1.end());
    result.insert(result.end(), result2.begin(),result2.end());
    result.insert(result.end(), result3.begin(),result3.end());
    result.insert(result.end(), result4.begin(),result4.end());
    return result;
}
vector<location> getinsidepoint(float x,float y,float x1,float y1,vector<location>& result,vector<location>& result1){
    int x_t=(int)(x*400),y_t=(int)(y*300),x1_t=(int)(x1*400),y1_t=(int)(y1*300);
    vector<location> answer;
    bool flag=y>y1?true:false;
    auto it1=result.begin();
    auto it2=result1.begin();
    int line_y=flag?y1_t:y_t;
    int target_y=flag?y_t:y1_t;
    while(line_y!=target_y&&it1!=result.end()&&it2!=result1.end()){
        if(it1->y!=line_y){
            it1++;
        }else if(it2->y!=line_y){
            it2++;
        }else{
            int first_x=min(it1->x,it2->x);
            int last_x=max(it1->x,it2->x);
            for(int i=first_x+1;i<last_x;i++){
                answer.push_back(location(i,line_y));
            }
            line_y+=1;
            it1++;
            it2++;
        }
    }
    return answer;
}
int setup_Triangle(float x,float y,float x1,float y1,float x2,float y2,unsigned int &VAO,unsigned int &VBO,int shaderProgram){
    vector<location> result=getTriangle(x, y, x1, y1, x2, y2);
    float vertices[result.size()*6];
    int index=0;
    float x_per=1/(float)400;
    float y_per=1/(float)300;
    for (std::vector<location>::iterator it = result.begin(); it != result.end(); ++it)
    {
        vertices[index]=(*it).x*x_per;
        vertices[index+1]=(*it).y*y_per;
        vertices[index+2]=0;
        vertices[index+3]=1;
        vertices[index+4]=1;
        vertices[index+5]=0;
        index+=6;
    }
    
    //绑定顶点数组对象
    glBindVertexArray(VAO);
    //把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    //清楚绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return result.size();
}
int setup_Circle(unsigned int &VAO,unsigned int &VBO,int shaderProgram,float circle_r){
    int cx=(int)((0.0f)*400),cy=(int)((0.0f)*300);
    float x_per=1/(float)400;
    float y_per=1/(float)300;
    int r=circle_r*400;
    int x = 0, y = r;
    int d = 1 - r;
    vector<location> points;
    while (x <= y) {
        plotCirclePixel(cx, cy, x, y,points);
        if (d <= 0) {
            d += 2*x + 3;
        } else {
            d += (2*x-2*y) + 5;
            y--;
        }
        x++;
    }
    float vertices[points.size()*6];
    int index=0;
    for (std::vector<location>::iterator it = points.begin(); it != points.end(); ++it)
    {
        vertices[index]=(*it).x*x_per;
        vertices[index+1]=(*it).y*y_per;
        vertices[index+2]=0;
        vertices[index+3]=1;
        vertices[index+4]=1;
        vertices[index+5]=1;
        index+=6;
    }
    //绑定顶点数组对象
    glBindVertexArray(VAO);
    
    //把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    //清楚绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return points.size();
}
void plotCirclePixel(int cx, int cy,int x, int y,vector<location>& points){
    points.push_back(location(cx+x,cy+y));
    points.push_back(location(cx-x,cy+y));
    points.push_back(location(cx+x,cy-y));
    points.push_back(location(cx-x,cy-y));
    points.push_back(location(cx+y,cy+x));
    points.push_back(location(cx-y,cy+x));
    points.push_back(location(cx+y,cy-x));
    points.push_back(location(cx-y,cy-x));
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
