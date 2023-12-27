#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 256) out;

uniform vec2 offset;
uniform vec2 gridSize;

void main() {

    float x = -1.0;

    while (x < 1.0) {
        gl_Position = vec4(x, -1.0, 0.0, 1.0); 
        EmitVertex();

        gl_Position = vec4(x, 1.0, 0.0, 1.0); 
        EmitVertex();
        
        EndPrimitive();
        x += gridSize.x;
    }

    float y = -1.0;

    while (y < 1.0) {
        gl_Position = vec4(-1.0, y, 0.0, 1.0); 
        EmitVertex();

        gl_Position = vec4(1.0, y, 0.0, 1.0); 
        EmitVertex();
        
        EndPrimitive();
        y += gridSize.y;
    }

}