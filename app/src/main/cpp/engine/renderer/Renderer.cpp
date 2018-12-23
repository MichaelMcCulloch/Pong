
#include "Renderer.h"


//TODO: throw errors and quit.
void Renderer::startUp(ANativeWindow *window) {
    if (init_display(window)) return;
    if (prepareOpenGL()) return;
    if (prepareShaders()) return;
}

void Renderer::shutDown() {
    term_display();
}

/**
 * Initialize an EGL context for the current display.
 */
int Renderer::init_display(ANativeWindow *window) {
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
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
    cntxt = eglCreateContext(disp, config, NULL, NULL);

    if (eglMakeCurrent(disp, surf, surf, cntxt) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
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
        LOGI("OpenGL Info: %s", info);
    }
    // Initialize GL state.
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    return 0;

}

int Renderer::prepareShaders() {
    //prepare the shaders
    char *vertexSource = (char *)
            "attribute vec4 vPosition;   \n"
            "void main()                 \n"
            "{"
            "    gl_Position=vPosition;  \n"
            "}";
    char *fragmentSource = (char *)
            "precision mediump float;                   \n"
            "void main()                                \n"
            "{                                          \n"
            "  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); \n"
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


     // TODO: draw a triangle which follows touch
    if (display == NULL) {
        // No display.
        return;
    }


    glClearColor(((float) x /width), angle,
                 ((float) y/height), 1);
    glClear(GL_COLOR_BUFFER_BIT);

    eglSwapBuffers(display, surface);

}

/**
 * Tear down the EGL context currently associated with the display.
 * TODO: Make use of the message bus to brodcast the trigger.
 */
void Renderer::term_display() {
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






