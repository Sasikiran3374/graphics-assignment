#version 330 core

in vec3 ourcolor;
out vec4 fragcolor;

void main(){
    fragcolor=vec4(ourcolor,1.0);
}