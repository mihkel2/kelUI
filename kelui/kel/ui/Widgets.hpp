#pragma once
#include <string>
#include "drawing.hpp"

namespace kel {
    class Widgets {
    public:

        static void setcursor(float x, float y);


        static bool button(const std::string& text, float w = 200.0f, float h = 40.0f);


        static void checkbox(const std::string& text, bool* val);


        static void sliderint(const std::string& text, int* val, int min, int max, float w = 200.0f);


        static void sliderfloat(const std::string& text, float* val, float min, float max, float w = 200.0f);

    private:
        static float cursor_x;
        static float cursor_y;
        static const float padding;

    };
}
