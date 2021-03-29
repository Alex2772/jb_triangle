#pragma once


#include <X11/X.h>
#include <string>
#include <GL/glx.h>

class XWindow {
private:
    static GLXContext ourContext;
    static Display* ourDisplay;
    static Screen* ourScreen;
    static int ourScreenId;
    Window mNativeHandle;

protected:
    virtual void onCloseButtonClicked() {};

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

