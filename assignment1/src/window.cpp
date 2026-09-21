// T.G. SASI KIRAN 112301033
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

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

extern Cell cells[grid][grid][grid];

extern int cube_x;
extern int cube_y;
extern int cube_z;

extern glm::vec3 cube_color;

float rotx=25.0f;
float roty=-35.0f;

void moveCube(int dx,int dy,int dz)
{
    int nx=cube_x+dx;
    int ny=cube_y+dy;
    int nz=cube_z+dz;

    if(nx>=0 && nx<grid && ny>=0 && ny<grid && nz>=0 && nz<grid)
    {
        cube_x=nx;
        cube_y=ny;
        cube_z=nz;

        cout<<"Cube position: ("<<cube_x<<", "<<cube_y<<", "<<cube_z<<")"<<endl;
    }
}

void changeColor()
{
    float r,g,b;

    cout<<"Enter RGB values (0 to 1): ";
    cin>>r>>g>>b;

    if(cin.fail())
    {
        cin.clear();
        cin.ignore(10000,'\n');
        cout<<"Invalid input."<<endl;
        return;
    }

    if(r<0.0f || r>1.0f || g<0.0f || g>1.0f || b<0.0f || b>1.0f)
    {
        cout<<"RGB values must be between 0 and 1."<<endl;
        return;
    }

    cube_color=glm::vec3(r,g,b);

    cout<<"Cube color changed."<<endl;
}

void fillCell()
{
    cells[cube_x][cube_y][cube_z].filled=true;
    cells[cube_x][cube_y][cube_z].color=cube_color;
    cout<<"Filled cell: ("<<cube_x<<", "<<cube_y<<", "<<cube_z<<")"<<endl;
}

void clearCell()
{
    cells[cube_x][cube_y][cube_z].filled=false;
    cout<<"Cleared cell: ("<<cube_x<<", "<<cube_y<<", "<<cube_z<<")"<<endl;
}

void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods)
{
    if(action!=GLFW_PRESS)
        return;

    if(key==GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window,true);

    else if(key==GLFW_KEY_LEFT)
        moveCube(-1,0,0);

    else if(key==GLFW_KEY_RIGHT)
        moveCube(1,0,0);

    else if(key==GLFW_KEY_UP)
        moveCube(0,1,0);

    else if(key==GLFW_KEY_DOWN)
        moveCube(0,-1,0);

    else if(key==GLFW_KEY_U)
        moveCube(0,0,1);

    else if(key==GLFW_KEY_B)
        moveCube(0,0,-1);

    else if(key==GLFW_KEY_C)
        changeColor();

    else if(key==GLFW_KEY_F)
        fillCell();

    else if(key==GLFW_KEY_W)
        clearCell();

    else if(key==GLFW_KEY_L)
        roty-=10.0f;

    else if(key==GLFW_KEY_R)
        roty+=10.0f;

    else if(key==GLFW_KEY_T)
        rotx-=10.0f;

    else if(key==GLFW_KEY_D)
        rotx+=10.0f;
}

GLFWwindow* initializeWindow()
{
    if(!glfwInit())
    {
        cout<<"GLFW initialization failed."<<endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif

    GLFWwindow* window=glfwCreateWindow(1000,1000,"CS5024 Assignment 1",NULL,NULL);

    if(!window)
    {
        cout<<"Window creation failed."<<endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    glewExperimental=GL_TRUE;

    if(glewInit()!=GLEW_OK)
    {
        cout<<"GLEW initialization failed."<<endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    glClearColor(0.05f,0.05f,0.08f,1.0f);

    return window;
}

void setupCallbacks(GLFWwindow* window)
{
    glfwSetKeyCallback(window,keyCallback);
}