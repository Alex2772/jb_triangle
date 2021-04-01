#include <iostream>
#include <memory>
#include "XWindow.h"
#include "Vbo.h"
#include "Vec.h"
#include "Program.h"

class MyWindow: public XWindow {
private:
    Program mProgram;
    ivec2 mWindowSize;
    std::unique_ptr<Vbo> mTriangle;
    std::vector<vec2> mVertices =
            {
                    {0,  1},
                    {-100, -100},
                    {100,  -100},
            };

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

        glDisable(GL_CULL_FACE);

        if (mTriangle) {
            Vbo::EnableAttribArray v(*mTriangle);
            mProgram.use();
            mTriangle->bind();
            gl::drawArrays(gl::TRIANGLES, 0, 3);
        }

    }

    void onWindowResize(int width, int height) override {
        XWindow::onWindowResize(width, height);
        mWindowSize = {width, height};
    }

    void onMousePressed(unsigned int button, int x, int y) override {
        XWindow::onMousePressed(button, x, y);

        if (!mTriangle) {
            mTriangle = std::make_unique<Vbo>(0);
            mVertices[0] = vec2{float(x), float(y)};
            mTriangle->setData(mVertices);
        }
    }
};

int main() {
    MyWindow w;


    w.loop();
    return 0;
}
