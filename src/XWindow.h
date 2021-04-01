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

    ivec2 mWindowSize;

protected:
    virtual void onMousePressed(unsigned int button, const ivec2& pos) {}
    virtual void onMouseReleased(unsigned button, const ivec2& pos) {}
    virtual void onMouseMove(const ivec2& pos) {}
    virtual void onWindowResize(const ivec2& size);

    virtual void onRedraw() {};
public:
    explicit XWindow(const std::string& title, const ivec2& size);

    ~XWindow();

    /**
     * @brief Do event loop until window closed
     */
    void loop();

    const ivec2& getWindowSize() const {
        return mWindowSize;
    }
};

