#pragma once

#include "logging.h"

extern "C" {
    #include <GLES3/gl32.h>
}

#include "../glm/glm.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtc/matrix_transform.hpp"

using namespace glm;

struct Geometry
{
    // OpenGL names for array buffer objects, vertex array object
    GLuint vertexBuffer;
    GLuint indexBuffer;
    GLuint colourBuffer;
    GLuint vertexArray;

    // initialize object names to zero (OpenGL reserved value)
    Geometry() : vertexBuffer(0), indexBuffer(0), colourBuffer(0), vertexArray(0)
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
bool LoadGeometry(Geometry *geometry, vec2 *vertices, u_short *indices, vec3 *colours,
                  int elementCount,
                  int indicesCount);

// deallocate geometry-related objects
void DestroyGeometry(Geometry *geometry);