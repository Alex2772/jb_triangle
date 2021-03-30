#include <iostream>
#include <memory>
#include "XWindow.h"
#include "Vbo.h"
#include "Vec.h"
#include "Program.h"

class MyWindow: public XWindow {
private:
    Program mProgram;
    std::unique_ptr<Vbo> mTriangle;

public:
    MyWindow() : XWindow("JetBrains Internship 2021, author: Alex2772"),
        mProgram("attribute vec3 pos;"
                 "void main(void) {gl_Position = vec4(pos, 1);}",
                 "void main(void) {gl_FragColor = vec4(1, 1, 1, 1);}", {"pos"})
    {

    }

protected:
    void onRedraw() override {
        XWindow::onRedraw();

        const auto backgroundColor = 0x25854b_rgb;
        glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glViewport(0, 0, 800, 600);

        glDisable(GL_CULL_FACE);

        if (mTriangle) {
            Vbo::EnableAttribArray v(*mTriangle);
            mProgram.use();
            mTriangle->bind();
            gl::drawArrays(gl::TRIANGLES, 0, 3);
        }

    }

    void onMousePressed(unsigned int button, int x, int y) override {
        XWindow::onMousePressed(button, x, y);

        mTriangle = std::make_unique<Vbo>(0);
        mTriangle->setData({
                                   {0, 1},
                                   {-1, -1},
                                   {1, -1},
                           });
    }
};

int main() {
    MyWindow w;


    w.loop();
    return 0;
}
