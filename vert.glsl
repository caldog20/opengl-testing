#version 330 core

// inPos: The vertex position.
// inColor: The colour in BGR888. Top 8 bits are garbage and are trimmed by the vertex shader to conserve CPU time
// inClut: The CLUT (palette) for textured primitives
// inTexpage: The texpage. We use bit 15 for indicating an untextured primitive (1 = untextured). This
// lets us batch untextured and textured primitives together. Bit 15 is unused by hardware, so this is a possible optimization
// inUV: The UVs (texture coordinates) for textured primitives

layout (location = 0) in ivec2 inPos;
layout (location = 1) in uint inColor;
layout (location = 2) in int inClut;
layout (location = 3) in int inTexpage;
layout (location = 4) in vec2 inUV;

out vec4 vertexColor;
out vec2 texCoords;
flat out ivec2 clutBase;
flat out ivec2 texpageBase;
flat out int texMode;

// We always apply a 0.5 offset in addition to the drawing offsets, to cover up OpenGL inaccuracies
uniform vec2 u_vertexOffsets = vec2(+0.5, -0.5);

void main() {
    // Normalize coords to [0, 2]
    float x = float(inPos.x);
    float y = float(inPos.y);
    float xx = (x + 0.5) / 512.0;
    float yy = (y + -0.5) / 256;

    // Normalize to [-1, 1]
    xx -= 1.0;
    yy -= 1.0;

    float red = float(inColor & 0xffu);
    float green = float((inColor >> 8u) & 0xffu);
    float blue = float((inColor >> 16u) & 0xffu);
    vec3 color = vec3(red, green, blue);

    gl_Position = vec4(xx, yy, 1.0, 1.0);
    vertexColor = vec4(color / 255.0, 1.0);
//    texMode = 4;
    if ((inTexpage & 0x8000) != 0) { // Untextured primitive
        texMode = 4;
    } else {
        texMode = (inTexpage >> 7) & 3;
        texCoords = inUV;
        texpageBase = ivec2((inTexpage & 0xf) * 64, ((inTexpage >> 4) & 0x1) * 256);
        clutBase = ivec2((inClut & 0x3f) * 16, inClut >> 6);
    }
}


//// GLSL Vertex Shader
//#version 330 core
//layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 color;
//out vec3 vertexColor;
//
//void main() {
//
//    gl_Position = vec4(position, 1.0);
//    gl_Position.x = (gl_Position.x / 512.f) - 1.f;
//    gl_Position.y = -(gl_Position.y / 256.f - 1.f);
//
//
//    vertexColor = color;
//    vertexColor.r /=  255.f;
//    vertexColor.g /=  255.f;
//    vertexColor.b /=  255.f;
//}