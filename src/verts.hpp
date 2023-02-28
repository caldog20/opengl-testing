#pragma once
#include <GL/glew.h>

#include "global.hpp"

struct Vertex {
    int x;
    int y;

    u32 color;
    u16 clut;
    u16 texpage;

    u16 u;
    u16 v;

    Vertex(u32 position) {
        x = position & 0xffff;
        y = (position >> 16) & 0xffff;
        texpage = 0x8000;
    }

    Vertex(u32 position, u32 color) : color(color) {
        x = position & 0xffff;
        y = (position >> 16) & 0xffff;
        texpage = 0x8000;
    }

    Vertex(u32 position, u32 color, u16 clut, u16 texpage, u32 texcoords) : color(color), texpage(texpage), clut(clut) {
        x = position & 0xffff;
        y = (position >> 16) & 0xffff;
        u = texcoords & 0xff;
        v = (texcoords >> 8) & 0xff;
    }

    Vertex() = default;
};

// Not used right now
struct Texture {
    Texture() = default;

    Texture(int width, int height, GLenum type) : m_width(width), m_height(height), m_type(type) {
        glGenTextures(1, &m_handle);
    }

    void bind() { glBindTexture(m_type, m_handle); }

    void set(GLint format) {
        bind();
        glTexStorage2D(m_type, 1, format, m_width, m_height);
    }

    GLuint handle() { return m_handle; }

    ~Texture() { glDeleteTextures(1, &m_handle); }

    GLuint m_handle{};
    GLenum m_type;
    int m_width;
    int m_height;
};