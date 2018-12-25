#pragma once

#include "common.hpp"

struct Geometry
{
    // OpenGL names for array buffer objects, vertex array object
    GLuint vertexBuffer;
    GLuint colourBuffer;
    GLuint vertexArray;
    GLsizei elementCount;
    GLenum mode;

    // initialize object names to zero (OpenGL reserved value)
    Geometry() : vertexBuffer(0), colourBuffer(0), vertexArray(0), elementCount(0)
    {
    }
};

bool CheckGLErrors();

// creates and returns a shader object compiled from the given source
GLuint CompileShader(GLenum shaderType, const char *source);

// creates and returns a program object linked from vertex and fragment shaders
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

// Functions to set up OpenGL buffers for storing geometry data
bool InitializeVAO(Geometry *geometry);

// create buffers and fill with geometry data, returning true if successful
bool LoadGeometry(Geometry *geometry, glm::vec2 *vertices, glm::vec3 *colours, int elementCount);

// deallocate geometry-related objects
void DestroyGeometry(Geometry *geometry);