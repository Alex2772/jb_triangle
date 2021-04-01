#pragma once


#include <X11/X.h>
#include <string>
#include <GL/glx.h>
#include "Vec.h"

class XWindow {
private:
    static GLXContext ourContext;
    static Display* ourDisplay;
    static Screen* ourScreen;
    static int ourScreenId;
    Window mNativeHandle;

protected:
    virtual void onMousePressed(unsigned button, int x, int y) {}
    virtual void onMouseReleased(unsigned button, int x, int y) {}
    virtual void onMouseMouse(int x, int y) {}
    virtual void onWindowResize(int width, int height);

    virtual void onRedraw() {};
public:
    explicit XWindow(const std::string& title);

    ~XWindow();

    /**
     * @brief Do event loop until window closed
     */
    void loop();

    static Display* getDisplay() {
        return ourDisplay;
    }
};

