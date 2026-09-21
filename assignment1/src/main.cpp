// T.G. SASI KIRAN 112301033
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
const int grid=5;

struct Cell{
    bool filled;
    glm::vec3 color;
    Cell(){
        filled=false;
        color=glm::vec3(1.0f,1.0f,1.0f);
    }
};

Cell cells[grid][grid][grid];

int cube_x=2,cube_y=2,cube_z=2;
glm::vec3 cube_color(1.0f,0.2f,0.2f);

extern GLFWwindow* initializeWindow();
extern void setupCallbacks(GLFWwindow* window);
extern void moveCube(int dx,int dy,int dz);
extern void changeColor();
extern void fillCell();
extern void clearCell();

extern float rotx;
extern float roty;

extern GLuint createShaderProgram();

void addLine(vector<float>& data,glm::vec3 a,glm::vec3 b,glm::vec3 color)
{
    data.push_back(a.x);
    data.push_back(a.y);
    data.push_back(a.z);
    data.push_back(color.r);
    data.push_back(color.g);
    data.push_back(color.b);

    data.push_back(b.x);
    data.push_back(b.y);
    data.push_back(b.z);
    data.push_back(color.r);
    data.push_back(color.g);
    data.push_back(color.b);
}

void addCube(vector<float>& data,float x,float y,float z,glm::vec3 color)
{
    float x1=x+1.0f;
    float y1=y+1.0f;
    float z1=z+1.0f;

    glm::vec3 vertices[8]=
    {
        glm::vec3(x,y,z),
        glm::vec3(x1,y,z),
        glm::vec3(x1,y1,z),
        glm::vec3(x,y1,z),
        glm::vec3(x,y,z1),
        glm::vec3(x1,y,z1),
        glm::vec3(x1,y1,z1),
        glm::vec3(x,y1,z1)
    };

    int indices[36]=
    {
        0,1,2, 0,2,3,
        4,6,5, 4,7,6,
        0,4,5, 0,5,1,
        3,2,6, 3,6,7,
        0,3,7, 0,7,4,
        1,5,6, 1,6,2
    };

    for(int i=0;i<36;i++)
    {
        glm::vec3 p=vertices[indices[i]];

        data.push_back(p.x);
        data.push_back(p.y);
        data.push_back(p.z);

        data.push_back(color.r);
        data.push_back(color.g);
        data.push_back(color.b);
    }
}

void createGrid(vector<float>& data)
{
    glm::vec3 grid_color(0.7f,0.7f,0.7f);

    float start=-2.5f;
    float end=2.5f;

    for(int i=0;i<=grid;i++)
    {
        float p=start+i;

        addLine(data,glm::vec3(start,start,p),glm::vec3(end,start,p),grid_color);
        addLine(data,glm::vec3(p,start,start),glm::vec3(p,start,end),grid_color);

        addLine(data,glm::vec3(start,end,p),glm::vec3(end,end,p),grid_color);
        addLine(data,glm::vec3(p,end,start),glm::vec3(p,end,end),grid_color);

        addLine(data,glm::vec3(start,p,start),glm::vec3(end,p,start),grid_color);
        addLine(data,glm::vec3(p,start,start),glm::vec3(p,end,start),grid_color);

        addLine(data,glm::vec3(start,p,end),glm::vec3(end,p,end),grid_color);
        addLine(data,glm::vec3(p,start,end),glm::vec3(p,end,end),grid_color);

        addLine(data,glm::vec3(start,start,p),glm::vec3(start,end,p),grid_color);
        addLine(data,glm::vec3(start,p,start),glm::vec3(start,p,end),grid_color);

        addLine(data,glm::vec3(end,start,p),glm::vec3(end,end,p),grid_color);
        addLine(data,glm::vec3(end,p,start),glm::vec3(end,p,end),grid_color);
    }
}

void drawData(GLuint vao,GLuint vbo,vector<float>& data,GLenum mode)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,data.size()*sizeof(float),data.data(),GL_DYNAMIC_DRAW);
    glDrawArrays(mode,0,data.size()/6);
}

void createHelix(vector<float>& data)
{
    int points=120;
    float radius=1.2f;
    float height=5.0f;

    for(int i=0;i<points-1;i++)
    {
        float t1=(float)i/(points-1);
        float t2=(float)(i+1)/(points-1);

        float a1=t1*8.0f*3.14159f;
        float a2=t2*8.0f*3.14159f;

        glm::vec3 p1(radius*cos(a1),radius*sin(a1),-height/2.0f+height*t1);
        glm::vec3 p2(radius*cos(a2),radius*sin(a2),-height/2.0f+height*t2);
        glm::vec3 c1(t1,1.0f-t1,0.8f);
        glm::vec3 c2(t2,1.0f-t2,0.8f);

        addLine(data,p1,p2,c1);
    }
}

int main()
{
    GLFWwindow* window=initializeWindow();

    if(!window)
        return -1;

    GLuint program=createShaderProgram();

    GLuint vao,vbo;

    glGenVertexArrays(1,&vao);
    glGenBuffers(1,&vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);

    GLint transformloc=glGetUniformLocation(program,"transform");

    setupCallbacks(window);

    cout<<endl;
    cout<<"CS5024 Assignment 1"<<endl;
    cout<<"-------------------"<<endl;
    cout<<"LEFT / RIGHT : Move X"<<endl;
    cout<<"UP / DOWN    : Move Y"<<endl;
    cout<<"U / B        : Move Z"<<endl;
    cout<<"C            : Change color"<<endl;
    cout<<"F            : Fill cell"<<endl;
    cout<<"W            : Clear cell"<<endl;
    cout<<"L / R        : Rotate left/right"<<endl;
    cout<<"T / D        : Rotate up/down"<<endl;
    cout<<"ESC          : Exit"<<endl;
    cout<<endl;
    cout<<"Starting cube position: (2, 2, 2)"<<endl;

    while(!glfwWindowShouldClose(window))
    {
        int width,height;

        glfwGetFramebufferSize(window,&width,&height);
        glViewport(0,0,width,height);

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0f);
        model=glm::scale(model,glm::vec3(0.7f,0.7f,0.7f));
        model=glm::rotate(model,glm::radians(rotx),glm::vec3(1.0f,0.0f,0.0f));
        model=glm::rotate(model,glm::radians(roty),glm::vec3(0.0f,1.0f,0.0f));

        glm::mat4 view=glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,-9.0f));
        glm::mat4 projection=glm::perspective(glm::radians(45.0f),(float)width/(float)height,0.1f,100.0f);
        glm::mat4 transform=projection*view*model;

        glUseProgram(program);

        glUniformMatrix4fv(transformloc,1,GL_FALSE,glm::value_ptr(transform));

        vector<float> griddata;
        createGrid(griddata);
        glLineWidth(1.5f);
        drawData(vao,vbo,griddata,GL_LINES);

        vector<float> filleddata;

        for(int x=0;x<grid;x++)
        {
            for(int y=0;y<grid;y++)
            {
                for(int z=0;z<grid;z++)
                {
                    if(cells[x][y][z].filled)
                    {
                        float px=-2.5f+x;
                        float py=-2.5f+y;
                        float pz=-2.5f+z;

                        addCube(filleddata,px,py,pz,cells[x][y][z].color);
                    }
                }
            }
        }

        if(!filleddata.empty())
            drawData(vao,vbo,filleddata,GL_TRIANGLES);

        vector<float> selectedcube;

        float px=-2.5f+cube_x;
        float py=-2.5f+cube_y;
        float pz=-2.5f+cube_z;

        addCube(selectedcube,px,py,pz,cube_color);

        drawData(vao,vbo,selectedcube,GL_TRIANGLES);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);
    glDeleteProgram(program);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}