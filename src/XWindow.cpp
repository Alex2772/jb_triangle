//
// Created by alex2772 on 3/29/21.
//

#include <iostream>
#include <limits>
#include "XWindow.h"
#include "MyGLEW.h"

GLXContext XWindow::ourContext = nullptr;
Display* XWindow::ourDisplay = nullptr;
Screen* XWindow::ourScreen = nullptr;
int XWindow::ourScreenId = 0;

struct {
    Atom wmProtocols;
    Atom wmDeleteWindow;

    void init() {
        wmProtocols = XInternAtom(XWindow::getDisplay(), "WM_PROTOCOLS", False);
        wmDeleteWindow = XInternAtom(XWindow::getDisplay(), "WM_DELETE_WINDOW", False);
    }

} gAtoms;

XWindow::XWindow(const std::string& title) {


    struct DisplayInstance {
    private:
        static int xerrorhandler(Display* dsp, XErrorEvent* error) {
            if (ourDisplay == dsp) {
                char errorstring[0x100];
                XGetErrorText(dsp, error->error_code, errorstring, sizeof(errorstring));
                printf("X Error: %s\n", errorstring);
            }
            return 0;
        }
    public:
        DisplayInstance() {
            ourDisplay = XOpenDisplay(nullptr);
            XSetErrorHandler(xerrorhandler);
            ourScreen = DefaultScreenOfDisplay(ourDisplay);
            ourScreenId = DefaultScreen(ourDisplay);
            gAtoms.init();
        }

        ~DisplayInstance() {
        }
    };
    static DisplayInstance display;
    static XVisualInfo* vi;
    static XSetWindowAttributes swa;


    if (ourContext == nullptr) {
        GLint att[] = {GLX_X_RENDERABLE, True, // 1
                       GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT, // 3
                       GLX_RENDER_TYPE, GLX_RGBA_BIT, // 5
                       GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR, // 7
                       GLX_RED_SIZE, 8, // 9
                       GLX_GREEN_SIZE, 8, // 11
                       GLX_BLUE_SIZE, 8, // 13
                       GLX_ALPHA_SIZE, 8, // 15
                       GLX_DEPTH_SIZE, 24,
                       GLX_STENCIL_SIZE, 8,
                       GLX_DOUBLEBUFFER, true,
                       GLX_STENCIL_SIZE, 8,
                       GLX_SAMPLE_BUFFERS  , 1,
                       GLX_SAMPLES         , 16,
                       None};

        constexpr size_t attArrayLength = sizeof(att) / sizeof(GLint);
        
        int fbcount;
        GLXFBConfig* fbc = glXChooseFBConfig(ourDisplay, DefaultScreen(ourDisplay), att, &fbcount);

        if (fbc == nullptr || fbcount <= 0) {
            // try to reduce system requirements
            size_t indexToReduce = attArrayLength - 2;
            do {
                std::cout << "Reduced OpenGL requirements: pass " << (attArrayLength - indexToReduce) / 2 - 1 << std::endl;
                att[indexToReduce] = 0;
                indexToReduce -= 2;
                fbc = glXChooseFBConfig(ourDisplay, DefaultScreen(ourDisplay), att, &fbcount);
            } while ((fbc == nullptr || fbcount <= 0) && indexToReduce > 13); // up to GLX_BLUE_SIZE

            if (fbc == nullptr || fbcount <= 0) {
                // try to disable rgba.
                att[5] = 0;
                std::cout << "Disabled RGBA" << std::endl;
                fbc = glXChooseFBConfig(ourDisplay, DefaultScreen(ourDisplay), att, &fbcount);

                if (fbc == nullptr || fbcount <= 0) {
                    // use default attribs
                    std::cout << "Using default attribs" << std::endl;
                    glXChooseFBConfig(ourDisplay, DefaultScreen(ourDisplay), nullptr, &fbcount);
                    if (fbc == nullptr || fbcount <= 0) {
                        // giving up.
                        std::cout << "System hardware is not supported. Giving up." << std::endl;
                        exit(-1);
                    }
                }
            }
        }

        // pick the best FB
        int bestConfigId = -1, worstConfigId = -1, bestNumSamples = -1, worstNumSamples = std::numeric_limits<int>::max();

        int i;
        for (i = 0; i < fbcount; ++i) {
            vi = glXGetVisualFromFBConfig(ourDisplay, fbc[i]);
            if (vi) {
                int samp_buf, samples;
                glXGetFBConfigAttrib(ourDisplay, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
                glXGetFBConfigAttrib(ourDisplay, fbc[i], GLX_SAMPLES, &samples);

                if (bestConfigId < 0 || samp_buf && samples > bestNumSamples)
                    bestConfigId = i, bestNumSamples = samples;
                if (worstConfigId < 0 || !samp_buf || samples < worstNumSamples)
                    worstConfigId = i, worstNumSamples = samples;
            }
            XFree(vi);
        }

        GLXFBConfig bestFbc = fbc[bestConfigId];

        XFree(fbc);

        // Get a visual
        vi = glXGetVisualFromFBConfig(ourDisplay, bestFbc);
        auto cmap = XCreateColormap(ourDisplay, ourScreen->root, vi->visual, AllocNone);
        swa.colormap = cmap;
        swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask
                         | PointerMotionMask | StructureNotifyMask | PropertyChangeMask;
        ourContext = glXCreateContext(ourDisplay, vi, nullptr, true);
    }

    const unsigned width = 800;
    const unsigned height = 600;

    mNativeHandle = XCreateWindow(ourDisplay, ourScreen->root, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual,
                            CWColormap | CWEventMask | CWCursor, &swa);


    XMapWindow(ourDisplay, mNativeHandle);

    XStoreName(ourDisplay, mNativeHandle, title.c_str());
    XChangeProperty(ourDisplay, mNativeHandle, XInternAtom(ourDisplay, "_NET_WM_NAME", false),
                    XInternAtom(ourDisplay, "UTF8_STRING", false), 8, PropModeReplace,
                    reinterpret_cast<const unsigned char*>(title.c_str()), title.length());

    XSetWMProtocols(ourDisplay, mNativeHandle, &gAtoms.wmDeleteWindow, 1);


    glXMakeCurrent(ourDisplay, mNativeHandle, ourContext);

    gl::init();

    onWindowResize(width, height);
/*
    if (!glewExperimental) {
        ALogger::info((const char*) glGetString(GL_VERSION));
        ALogger::info((const char*) glGetString(GL_VENDOR));
        ALogger::info((const char*) glGetString(GL_RENDERER));
        ALogger::info((const char*) glGetString(GL_EXTENSIONS));
        glewExperimental = true;
        if (glewInit() != GLEW_OK) {
            throw AException("glewInit failed");
        }
    }
    */
}

XWindow::~XWindow() {
    XDestroyWindow(ourDisplay, mNativeHandle);
}

void XWindow::loop() {
    XEvent ev;
    for (;;) {
        XNextEvent(ourDisplay, &ev);
        switch (ev.type) {
            case ClientMessage: {
                if (ev.xclient.message_type == gAtoms.wmProtocols &&
                    ev.xclient.data.l[0] == gAtoms.wmDeleteWindow) {
                    // close button clicked
                    return;
                }
                break;
            }
            case ButtonPress: {
                onMousePressed(ev.xbutton.button, ev.xbutton.x, ev.xbutton.y);
                break;
            }
            case ButtonRelease: {
                onMouseReleased(ev.xbutton.button, ev.xbutton.x, ev.xbutton.y);
                break;
            }
            case Expose: {
                onWindowResize(ev.xexpose.width, ev.xexpose.height);
                break;
            }

        }
        onRedraw();
        glXSwapBuffers(ourDisplay, mNativeHandle);
    }
}

void XWindow::onWindowResize(int width, int height) {
    glViewport(0, 0, width, height);
}
