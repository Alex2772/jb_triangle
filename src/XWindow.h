#pragma once


#include <X11/X.h>
#include <string>
#include <GL/glx.h>
#include <memory>
#include "Vec.h"

class XDisplayConnection;

class XWindow {
private:
    Window mNativeHandle;
    static std::weak_ptr<XDisplayConnection> ourDisplayConnection;
    std::shared_ptr<XDisplayConnection> mDisplayConnection;

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

};

