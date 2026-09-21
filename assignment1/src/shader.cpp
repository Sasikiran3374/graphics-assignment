#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

GLuint compileShader(GLenum type,const char* source)
{
    GLuint shader=glCreateShader(type);

    glShaderSource(shader,1,&source,NULL);
    glCompileShader(shader);

    GLint success;

    glGetShaderiv(shader,GL_COMPILE_STATUS,&success);

    if(!success)
    {
        char log[1024];
        glGetShaderInfoLog(shader,1024,NULL,log);
        cout<<"Shader error:\n"<<log<<endl;
    }
    return shader;
}

GLuint createShaderProgram()
{
    ifstream vertexFile("../shaders/vertex.glsl");
    ifstream fragmentFile("../shaders/fragment.glsl");

    stringstream vertexStream;
    stringstream fragmentStream;

    vertexStream<<vertexFile.rdbuf();
    fragmentStream<<fragmentFile.rdbuf();

    string vertexCode=vertexStream.str();
    string fragmentCode=fragmentStream.str();

    const char* vertexSource=vertexCode.c_str();
    const char* fragmentSource=fragmentCode.c_str();

    GLuint vertexShader=compileShader(GL_VERTEX_SHADER,vertexSource);
    GLuint fragmentShader=compileShader(GL_FRAGMENT_SHADER,fragmentSource);

    GLuint program=glCreateProgram();

    glAttachShader(program,vertexShader);
    glAttachShader(program,fragmentShader);

    glLinkProgram(program);

    GLint success;

    glGetProgramiv(program,GL_LINK_STATUS,&success);

    if(!success)
    {
        char log[1024];

        glGetProgramInfoLog(program,1024,NULL,log);

        cout<<"Program error:\n"<<log<<endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}