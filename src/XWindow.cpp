//
// Created by alex2772 on 3/29/21.
//

#include <iostream>
#include <limits>
#include "XWindow.h"
#include "MyGLEW.h"

std::weak_ptr<XDisplayConnection> XWindow::ourDisplayConnection;

struct {
    Atom wmProtocols;
    Atom wmDeleteWindow;

    void init(Display* display) {
        wmProtocols = XInternAtom(display, "WM_PROTOCOLS", False);
        wmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);
    }

} gAtoms;

struct XDisplayConnection {
private:

    GLXContext mContext;
    Display* mDisplay;
    Screen* mDefaultScreen;
    int mDefaultScreenId;
    XVisualInfo* mVisualInfo;
    XSetWindowAttributes mSetWindowAttributes;

    static int xerrorhandler(Display* dsp, XErrorEvent* error) {
        //if (mDisplay == dsp)
        {
            char errorstring[0x100];
            XGetErrorText(dsp, error->error_code, errorstring, sizeof(errorstring));
            printf("X Error: %s\n", errorstring);
        }
        return 0;
    }
public:
    XDisplayConnection() {
        mDisplay = XOpenDisplay(nullptr);
        XSetErrorHandler(xerrorhandler);
        mDefaultScreen = DefaultScreenOfDisplay(mDisplay);
        mDefaultScreenId = DefaultScreen(mDisplay);
        gAtoms.init(mDisplay);

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
        GLXFBConfig* fbc = glXChooseFBConfig(mDisplay,
                                             mDefaultScreenId,
                                             att,
                                             &fbcount);

        if (fbc == nullptr || fbcount <= 0) {
            // try to reduce system requirements
            size_t indexToReduce = attArrayLength - 2;
            do {
                std::cout << "Reduced OpenGL requirements: pass "
                          << (attArrayLength - indexToReduce) / 2 - 1
                          << std::endl;
                att[indexToReduce] = 0;
                indexToReduce -= 2;
                fbc = glXChooseFBConfig(mDisplay,
                                        mDefaultScreenId,
                                        att,
                                        &fbcount);
            } while ((fbc == nullptr || fbcount <= 0) && indexToReduce > 13); // up to GLX_BLUE_SIZE

            if (fbc == nullptr || fbcount <= 0) {
                // try to disable rgba.
                att[5] = 0;
                std::cout << "Disabled RGBA" << std::endl;
                fbc = glXChooseFBConfig(mDisplay,
                                        mDefaultScreenId,
                                        att,
                                        &fbcount);

                if (fbc == nullptr || fbcount <= 0) {
                    // use default attribs
                    std::cout << "Using default attribs" << std::endl;
                    glXChooseFBConfig(mDisplay,
                                      mDefaultScreenId,
                                      nullptr,
                                      &fbcount);
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
            mVisualInfo = glXGetVisualFromFBConfig(mDisplay, fbc[i]);
            if (mVisualInfo) {
                int samp_buf, samples;
                glXGetFBConfigAttrib(mDisplay, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
                glXGetFBConfigAttrib(mDisplay, fbc[i], GLX_SAMPLES, &samples);

                if (bestConfigId < 0 || samp_buf && samples > bestNumSamples)
                    bestConfigId = i, bestNumSamples = samples;
                if (worstConfigId < 0 || !samp_buf || samples < worstNumSamples)
                    worstConfigId = i, worstNumSamples = samples;
            }
            XFree(mVisualInfo);
        }

        GLXFBConfig bestFbc = fbc[bestConfigId];

        XFree(fbc);

        // Get a visual
        mVisualInfo = glXGetVisualFromFBConfig(mDisplay, bestFbc);
        auto cmap = XCreateColormap(mDisplay, mDefaultScreen->root, mVisualInfo->visual, AllocNone);
        mSetWindowAttributes.colormap = cmap;
        mSetWindowAttributes.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask
                                          | PointerMotionMask | StructureNotifyMask | PropertyChangeMask;
        mContext = glXCreateContext(mDisplay, mVisualInfo, nullptr, true);
    }

    ~XDisplayConnection() {
        XCloseDisplay(mDisplay);
    }

    GLXContext getContext() const {
        return mContext;
    }

    Display* getDisplay() const {
        return mDisplay;
    }

    Screen* getDefaultScreen() const {
        return mDefaultScreen;
    }

    int getDefaultScreenId() const {
        return mDefaultScreenId;
    }

    XVisualInfo* getVisualInfo() const {
        return mVisualInfo;
    }

    XSetWindowAttributes& getSetWindowAttributes() {
        return mSetWindowAttributes;
    }
};


XWindow::XWindow(const std::string& title, const ivec2& size):
    mWindowSize(size)
{
    if (!(mDisplayConnection = ourDisplayConnection.lock())) {
        ourDisplayConnection = mDisplayConnection = std::make_shared<XDisplayConnection>();
    }

    mNativeHandle = XCreateWindow(mDisplayConnection->getDisplay(),
                                  mDisplayConnection->getDefaultScreen()->root,
                                  0, 0,
                                  size.x, size.y,
                                  0,
                                  mDisplayConnection->getVisualInfo()->depth,
                                  InputOutput,
                                  mDisplayConnection->getVisualInfo()->visual,
                                  CWColormap | CWEventMask | CWCursor,
                                  &mDisplayConnection->getSetWindowAttributes());


    XMapWindow(mDisplayConnection->getDisplay(), mNativeHandle);

    XStoreName(mDisplayConnection->getDisplay(), mNativeHandle, title.c_str());
    XChangeProperty(mDisplayConnection->getDisplay(), mNativeHandle, XInternAtom(mDisplayConnection->getDisplay(), "_NET_WM_NAME", false),
                    XInternAtom(mDisplayConnection->getDisplay(), "UTF8_STRING", false), 8, PropModeReplace,
                    reinterpret_cast<const unsigned char*>(title.c_str()), title.length());

    XSetWMProtocols(mDisplayConnection->getDisplay(), mNativeHandle, &gAtoms.wmDeleteWindow, 1);


    glXMakeCurrent(mDisplayConnection->getDisplay(), mNativeHandle, mDisplayConnection->getContext());

    gl::init();

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
    XDestroyWindow(mDisplayConnection->getDisplay(), mNativeHandle);
}

void XWindow::loop() {
    {
        onWindowResize(mWindowSize);
    }
    XEvent ev;
    for (;;) {
        XNextEvent(mDisplayConnection->getDisplay(), &ev);
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
                onMousePressed(ev.xbutton.button, {ev.xbutton.x, ev.xbutton.y});
                break;
            }
            case ButtonRelease: {
                onMouseReleased(ev.xbutton.button, {ev.xbutton.x, ev.xbutton.y});
                break;
            }
            case Expose: {
                onWindowResize({ev.xexpose.width, ev.xexpose.height});
                break;
            }

        }
        onRedraw();
        glXSwapBuffers(mDisplayConnection->getDisplay(), mNativeHandle);
    }
}

void XWindow::onWindowResize(const ivec2& size) {
    glViewport(0, 0, size.x, size.y);
}
