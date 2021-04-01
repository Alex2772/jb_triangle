#include <iostream>
#include <memory>
#include <random>
#include <chrono>
#include "XWindow.h"
#include "Vbo.h"
#include "Vec.h"
#include "Program.h"


class MyWindow: public XWindow {
private:
    Program mProgram;
    std::unique_ptr<Vbo> mTriangle;
    std::vector<vec2> mVertices;

public:
    MyWindow() : XWindow("JetBrains Internship 2021, author: Alex2772"),
        mProgram("attribute vec2 pos;"
                 "uniform vec2 windowSize;"
                 "void main(void) {gl_Position = vec4(pos / windowSize * vec2(2, -2) + vec2(-1, 1), 0, 1);}",
                 "void main(void) {gl_FragColor = vec4(1, 1, 1, 1);}", {"pos"})
    {
        std::default_random_engine e;
        e.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());

        std::uniform_real_distribution<float> xDistr(0, 800);
        std::uniform_real_distribution<float> yDistr(0, 600);
        mVertices = {
            {0,  0},
            {xDistr(e), yDistr(e)},
            {xDistr(e), yDistr(e)},
        };
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
        mProgram.set("windowSize", vec2{float(width), float(height)});
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
