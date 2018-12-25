
#include "Renderer.h"


//TODO: throw errors and quit.
void Renderer::startUp(ANativeWindow *window, MessageBus* m) {
    LOGI("Renderer: startUp");
    BusNode::startUp(m);

    if (initDisplay(window)) return;
    if (prepareOpenGL()) return;
    if (prepareShaders()) return;
}

void Renderer::shutDown() {
    LOGI("Renderer: shutDown");
    termDisplay();
}

/**
 * Initialize an EGL context for the current display.
 */
int Renderer::initDisplay(ANativeWindow *window) {
    LOGI("Renderer: initDisplay");
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, 16,
            EGL_NONE
    };
    EGLint w, h, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surf;
    EGLContext cntxt;

    EGLDisplay disp = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(disp, 0, 0);

    /* Here, the application chooses the configuration it desires.
     * find the best match if possible, otherwise use the very first one
     */
    eglChooseConfig(disp, attribs, nullptr, 0, &numConfigs);
    std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[numConfigs]);
    assert(supportedConfigs);
    eglChooseConfig(disp, attribs, supportedConfigs.get(), numConfigs, &numConfigs);
    assert(numConfigs);
    auto i = 0;
    for (; i < numConfigs; i++) {
        auto &cfg = supportedConfigs[i];
        EGLint r, g, b, d;
        if (eglGetConfigAttrib(disp, cfg, EGL_RED_SIZE, &r) &&
            eglGetConfigAttrib(disp, cfg, EGL_GREEN_SIZE, &g) &&
            eglGetConfigAttrib(disp, cfg, EGL_BLUE_SIZE, &b) &&
            eglGetConfigAttrib(disp, cfg, EGL_DEPTH_SIZE, &d) &&
            r == 8 && g == 8 && b == 8 && d == 0) {

            config = supportedConfigs[i];
            break;
        }
    }
    if (i == numConfigs) {
        config = supportedConfigs[0];
    }

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(disp, config, EGL_NATIVE_VISUAL_ID, &format);
    surf = eglCreateWindowSurface(disp, config, window, NULL);
    EGLint attribList[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE }; // OpenGL 2.0

    cntxt = eglCreateContext(disp, config, NULL, attribList);


    if (eglMakeCurrent(disp, surf, surf, cntxt) == EGL_FALSE) {
        LOGW("Renderer: Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(disp, surf, EGL_WIDTH, &w);
    eglQuerySurface(disp, surf, EGL_HEIGHT, &h);

    display = disp;
    context = cntxt;
    surface = surf;
    width = w;
    height = h;

    return 0;
}


int Renderer::prepareOpenGL() {
    // Check openGL on the system
    auto opengl_info = {GL_VENDOR, GL_RENDERER, GL_VERSION, GL_EXTENSIONS};
    for (auto name : opengl_info) {
        auto info = glGetString(name);
        LOGI("Renderer: OpenGL Info: %s", info);
    }
    // Initialize GL state.
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    return 0;

}

int Renderer::prepareShaders() {
    //prepare the shaders
    char *vertexSource = (char *)
            "#version 320 es                        \n"
            "in vec2 vPosition;                     \n"
            "in vec3 vColor;                        \n"
            "out vec3 fCol;                         \n"
            "void main(){                           \n"
            "    gl_Position=vec4(vPosition, 0.0, 1.0);   \n"
            "    fCol=vColor;                       \n"
            "}";
    char *fragmentSource = (char *)
            "#version 320 es                            \n"
            "precision mediump float;                   \n"
            "in vec3 fCol;                          \n"
            "out vec4 color;                            \n"
            "void main() {                              \n"
            "  color = vec4(fCol, 1.0);                        \n"
            "}                                          \n";


    auto vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
    auto fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
    auto program = LinkProgram(vertex, fragment);

    if (CheckGLErrors()) {
        return -1;
    }
    shader = program;


    return 0;
}

/**
 * Just the current frame in the display.
 */
void Renderer::drawFrame(int x, int y, float angle) {
    LOGV("Renderer: drawFrame");

     // TODO: draw a triangle which follows touch
    if (display == NULL) {
        // No display.
        return;
    }

    float xOff = (float)x / (float)width;
    float yOff = (float)y / (float)height;
    glm::vec2 fingerIsAt = glm::vec2((xOff * 2) -1, -(yOff*2) +1);

    //Create Triangle
    std::vector<glm::vec2> pts = {
            glm::vec2(fingerIsAt.x - .5, fingerIsAt.y - sqrt(3) / 6),
            glm::vec2(fingerIsAt.x + .5, fingerIsAt.y - sqrt(3) / 6),
            glm::vec2(fingerIsAt.x + 0, fingerIsAt.y + sqrt(3) / 6)
    };


    std::vector<glm::vec3> col = {
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(0, 0, 1)
    };


    Geometry triangle;
    if (!InitializeVAO(&triangle)){
        LOGW("Renderer: failed to initialize VAO");
        return;
    }
    if (!LoadGeometry(&triangle, pts.data(), col.data(), 3)){
        LOGW("Renderer: failed to load geometry");
    };

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader);
    glBindVertexArray(triangle.vertexArray);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
    glUseProgram(0);

    eglSwapBuffers(display, surface);

    CheckGLErrors();

}

/**
 * Tear down the EGL context currently associated with the display.
 * TODO: Make use of the message bus to broadcast the trigger.
 */
void Renderer::termDisplay() {
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
        }
        eglTerminate(display);
    }

    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
}


void Renderer::onNotify(Message message) {
    LOGV("Renderer: received %s", (char *)message.getEvent());
    BusNode::onNotify(message);
}






