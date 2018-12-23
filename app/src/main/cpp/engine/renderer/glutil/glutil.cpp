#include "glutil.h"


bool CheckGLErrors() {
    bool error = false;
    for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError()) {
        LOGW("OpenGL ERROR:");
        switch (flag) {
            case GL_INVALID_ENUM:
                LOGW("GL_INVALID_ENUM");
                break;
            case GL_INVALID_VALUE:
                LOGW("GL_INVALID_VALUE");
                break;
            case GL_INVALID_OPERATION:
                LOGW("GL_INVALID_OPERATION");
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                LOGW("GL_INVALID_FRAMEBUFFER_OPERATION");
                break;
            case GL_OUT_OF_MEMORY:
                LOGW("GL_OUT_OF_MEMORY");
                break;
            default:
                LOGW("[unknown error code]");
        }
        error = true;
    }
    return error;
}


// creates and returns a shader object compiled from the given source
GLuint CompileShader(GLenum shaderType, const char *source) {
    // allocate shader object name
    GLuint shaderObject = glCreateShader(shaderType);

    // try compiling the source as a shader of the given type
    const GLchar *source_ptr = source;
    glShaderSource(shaderObject, 1, &source_ptr, 0);
    glCompileShader(shaderObject);

    // retrieve compile status
    GLint status;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
        char info[length];
        glGetShaderInfoLog(shaderObject, length, &length, &info[0]);
        LOGW("ERROR compiling shader:\n\n %s \n %s", source, info);

    }

    return shaderObject;
}

// creates and returns a program object linked from vertex and fragment shaders
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader) {
    // allocate program object name
    GLuint programObject = glCreateProgram();

    // attach provided shader objects to this program
    if (vertexShader)
        glAttachShader(programObject, vertexShader);
    if (fragmentShader)
        glAttachShader(programObject, fragmentShader);

    // try linking the program with given attachments
    glLinkProgram(programObject);

    // retrieve link status
    GLint status;
    glGetProgramiv(programObject, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
        char info[length];
        glGetProgramInfoLog(programObject, length, &length, &info[0]);
        LOGW("ERROR linking shader program: \n %s", info);
    }

    return programObject;
}



