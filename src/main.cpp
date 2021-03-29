#include <iostream>
#include "XWindow.h"

class MyWindow: public XWindow {
public:
    using XWindow::XWindow;
};

int main() {
    MyWindow w("Gono");
    w.loop();
    return 0;
}
