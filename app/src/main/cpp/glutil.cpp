#include "glutil.h"


bool CheckGLErrors() {
    bool error = false;
    for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError()) {
        char * errCode;
        switch (flag) {
            case GL_INVALID_ENUM:
                errCode = (char *)"GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                errCode = (char *)"GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                errCode = (char *)"GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                errCode = (char *)"GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                errCode = (char *)"GL_OUT_OF_MEMORY";
                break;
            default:
                errCode = (char *)"[unknown error code]";
        }
        LOGW("OpenGLES Error: %s", errCode);
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

bool InitializeVAO(Geometry *geometry)
{

    const GLuint VERTEX_INDEX = 0;
    const GLuint COLOUR_INDEX = 1;

    //Generate Vertex Buffer Objects
    // create an array buffer object for storing our vertices
    glGenBuffers(1, &geometry->vertexBuffer);

    //create one for storing the indices
    glGenBuffers(1, &geometry->indexBuffer);

    // create another one for storing our colours
    glGenBuffers(1, &geometry->colourBuffer);

    //Set up Vertex Array Object
    // create a vertex array object encapsulating all our vertex attributes
    glGenVertexArrays(1, &geometry->vertexArray);
    glBindVertexArray(geometry->vertexArray);

    // associate the position array with the vertex array object
    glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
    glVertexAttribPointer(
            VERTEX_INDEX, //Attribute index
            2,			  //# of components
            GL_FLOAT,	 //Type of component
            GL_FALSE,	 //Should be normalized?
            sizeof(vec2), //Stride - can use 0 if tightly packed
            0);			  //Offset to first element
    glEnableVertexAttribArray(VERTEX_INDEX);

    // associate the colour array with the vertex array object
    glBindBuffer(GL_ARRAY_BUFFER, geometry->colourBuffer);
    glVertexAttribPointer(
            COLOUR_INDEX, //Attribute index
            3,			  //# of components
            GL_FLOAT,	 //Type of component
            GL_FALSE,	 //Should be normalized?
            sizeof(vec3), //Stride - can use 0 if tightly packed
            0);			  //Offset to first element
    glEnableVertexAttribArray(COLOUR_INDEX);

    // unbind our buffers, resetting to default state
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return !CheckGLErrors();
}

// create buffers and fill with geometry data, returning true if successful
bool LoadGeometry(Geometry *geometry, vec2 *vertices, u_short *indices, vec3 *colours,
                  int elementCount,
                  int indicesCount)
{
    // create an array buffer object for storing our vertices
    glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * elementCount, vertices, GL_STATIC_DRAW);

    // create one for storing the indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u_short) * indicesCount, indices, GL_STATIC_DRAW);

    // create another one for storing our colours
    glBindBuffer(GL_ARRAY_BUFFER, geometry->colourBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * elementCount, colours, GL_STATIC_DRAW);

    //Unbind buffer to reset to default state
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // check for OpenGL errors and return false if error occurred
    return !CheckGLErrors();
}

// deallocate geometry-related objects
void DestroyGeometry(Geometry *geometry)
{
    // unbind and destroy our vertex array object and associated buffers
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &geometry->vertexArray);
    glDeleteBuffers(1, &geometry->vertexBuffer);
    glDeleteBuffers(1, &geometry->colourBuffer);
}