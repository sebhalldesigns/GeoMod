#version 330 core
layout (location = 0) in vec2 aPos;

uniform float offset;
uniform float gridSize;
uniform int horizontal;

void main() {

    int instance = gl_InstanceID;

    vec2 pos = aPos;
    pos.y += gridSize * instance;
    pos.y += offset;
    
    if (horizontal > 0) {
        
        gl_Position = vec4(pos, 0.0, 1.0);
    } else {
        gl_Position = vec4(pos.yx, 0.0, 1.0); 
    }

}