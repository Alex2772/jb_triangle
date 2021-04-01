#include <iostream>
#include <memory>
#include <random>
#include <chrono>
#include "XWindow.h"
#include "Vbo.h"
#include "Vec.h"
#include "Program.h"


const ivec2 WINDOW_SIZE = { 800, 600 };

class MyWindow: public XWindow {
private:
    Program mProgram;
    std::unique_ptr<Vbo> mTriangle;
    std::vector<vec2> mVertices;
    bool mDragging = false;
    ivec2 mDragOffset;

public:
    MyWindow() : XWindow("JetBrains Internship 2021, author: Alex2772", WINDOW_SIZE),
        mProgram("attribute vec2 pos;"
                 "uniform vec2 windowSize;"
                 "void main(void) {gl_Position = vec4(pos / windowSize * vec2(2, -2) + vec2(-1, 1), 0, 1);}",
                 "void main(void) {gl_FragColor = vec4(1, 1, 1, 1);}", {"pos"})
    {
        std::default_random_engine e(std::chrono::high_resolution_clock::now().time_since_epoch().count());

        std::uniform_int_distribution<int> xDistr(0, WINDOW_SIZE.x);
        std::uniform_int_distribution<int> yDistr(0, WINDOW_SIZE.y);
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

    void onWindowResize(const ivec2& size) override {
        XWindow::onWindowResize(size);
        mProgram.set("windowSize", size);
    }

    void onMousePressed(unsigned int button, const ivec2& pos) override {
        XWindow::onMousePressed(button, pos);
        if (button != 1)
            return;

        if (!mTriangle) {
            mTriangle = std::make_unique<Vbo>(0);
            mVertices[0] = pos;
            mTriangle->setData(mVertices);
        } else {
            mDragging = true;
            mDragOffset = pos;
        }
    }

    void onMouseReleased(unsigned int button, const ivec2& pos) override {
        XWindow::onMouseReleased(button, pos);
        if (button != 1)
            return;
        mDragging = false;
    }

    void onMouseMove(const ivec2& pos) override {
        XWindow::onMouseMove(pos);

        if (mDragging) {
            auto delta = pos - mDragOffset;
            mDragOffset = pos;
            for (auto& vertex : mVertices) {
                vertex += delta;
            }
            mTriangle->setData(mVertices);
        }
    }
};

int main() {
    MyWindow w;


    w.loop();
    return 0;
}
