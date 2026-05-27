#include "Widgets.hpp"
#include "drawing.hpp"

namespace kel {
    float Widgets::cursor_x = 0;
    float Widgets::cursor_y = 0;
    const float Widgets::padding = 8.0f;

    void Widgets::setcursor(float x, float y) {
        cursor_x = x;
        cursor_y = y;
    }

    bool Widgets::button(const std::string& text, float w, float h) {
      
        Drawing::rect(cursor_x, cursor_y, w, h, {0.2f, 0.2f, 0.25f, 1.0f});
       
        Drawing::rect(cursor_x, cursor_y, w, 2, {0.5f, 0.5f, 0.6f, 1.0f});
       
        Drawing::text(cursor_x + 10, cursor_y + (h * 0.25f), text, {0.9f, 0.9f, 0.9f, 1.0f});

        cursor_y += h + padding;  
        return false; 
    }

    void Widgets::checkbox(const std::string& text, bool* val) {
        float size = 24.0f;


        Drawing::rect(cursor_x, cursor_y, size, size, {0.15f, 0.15f, 0.15f, 1.0f});
      
        if (val && *val)
            Drawing::rect(cursor_x + 4, cursor_y + 4, size - 8, size - 8, {0.2f, 0.8f, 0.2f, 1.0f});

        Drawing::text(cursor_x + size + 8, cursor_y + 4, text, {0.9f, 0.9f, 0.9f, 1.0f});

        cursor_y += size + padding;  
    }

    void Widgets::sliderint(const std::string& text, int* val, int min, int max, float w) {
        float h = 24.0f;

       
        Drawing::rect(cursor_x, cursor_y + h / 2 - 2, w, 4, {0.1f, 0.1f, 0.1f, 1.0f});
       
        if (val && max > min) {
            float progress = static_cast<float>(*val - min) / (max - min);
            Drawing::rect(cursor_x, cursor_y + h / 2 - 2, w * progress, 4, {0.3f, 0.5f, 1.0f, 1.0f});
            Drawing::rect(cursor_x + (progress * w) - 6, cursor_y, 12, h, {0.7f, 0.7f, 0.8f, 1.0f});
        }
        
        Drawing::text(cursor_x, cursor_y - 14, text, {0.8f, 0.8f, 0.8f, 1.0f});

        cursor_y += h + padding + 14; 
    }

    void Widgets::sliderfloat(const std::string& text, float* val, float min, float max, float w) {
        float h = 24.0f;

       
        Drawing::rect(cursor_x, cursor_y + h / 2 - 2, w, 4, {0.1f, 0.1f, 0.1f, 1.0f});
     
        if (val && max > min) {
            float progress = (*val - min) / (max - min);
            Drawing::rect(cursor_x, cursor_y + h / 2 - 2, w * progress, 4, {0.3f, 0.5f, 1.0f, 1.0f});
            Drawing::rect(cursor_x + (progress * w) - 6, cursor_y, 12, h, {0.7f, 0.7f, 0.8f, 1.0f});
        }
      
        Drawing::text(cursor_x, cursor_y - 14, text, {0.8f, 0.8f, 0.8f, 1.0f});

        cursor_y += h + padding + 14;
    }
}