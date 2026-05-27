#include "drawing.hpp"

namespace kel {
    std::vector<Vertex> Drawing::vertices;
    float Drawing::screen_w = 1024.0f;
    float Drawing::screen_h = 1024.0f;

    void Drawing::setresolution(float w, float h) {
        screen_w = w;
        screen_h = h;
    }

    static float to_ndc_x(float x, float w) {
        return (x / w) * 2.0f - 1.0f;
    }

    static float to_ndc_y(float y, float h) {
        return (y / h) * 2.0f - 1.0f;
    }

    void Drawing::rect(float x, float y, float w, float h, Color c) {
        float x1 = to_ndc_x(x, screen_w);
        float y1 = to_ndc_y(y, screen_h);
        float x2 = to_ndc_x(x + w, screen_w);
        float y2 = to_ndc_y(y + h, screen_h);

        vertices.push_back({x1, y1, -1.0f, -1.0f, c.r, c.g, c.b, c.a});
        vertices.push_back({x2, y1, -1.0f, -1.0f, c.r, c.g, c.b, c.a});
        vertices.push_back({x1, y2, -1.0f, -1.0f, c.r, c.g, c.b, c.a});

        vertices.push_back({x2, y1, -1.0f, -1.0f, c.r, c.g, c.b, c.a});
        vertices.push_back({x2, y2, -1.0f, -1.0f, c.r, c.g, c.b, c.a});
        vertices.push_back({x1, y2, -1.0f, -1.0f, c.r, c.g, c.b, c.a});
    }

    void Drawing::addglyph(float x, float y, char c, Color color) {
        float size = 16.0f;
        float atlas_size = 16.0f;

        int col = (unsigned char)c % 16;
        int row = (unsigned char)c / 16;

        float u1 = (float)col / atlas_size;
        float v1 = (float)row / atlas_size;
        float u2 = u1 + (1.0f / atlas_size);
        float v2 = v1 + (1.0f / atlas_size);

        float vx1 = to_ndc_x(x, screen_w);
        float vy1 = to_ndc_y(y, screen_h);
        float vx2 = to_ndc_x(x + size, screen_w);
        float vy2 = to_ndc_y(y + size, screen_h);

        vertices.push_back({vx1, vy1, u1, v1, color.r, color.g, color.b, color.a});
        vertices.push_back({vx2, vy1, u2, v1, color.r, color.g, color.b, color.a});
        vertices.push_back({vx1, vy2, u1, v2, color.r, color.g, color.b, color.a});

        vertices.push_back({vx2, vy1, u2, v1, color.r, color.g, color.b, color.a});
        vertices.push_back({vx2, vy2, u2, v2, color.r, color.g, color.b, color.a});
        vertices.push_back({vx1, vy2, u1, v2, color.r, color.g, color.b, color.a});
    }

    void Drawing::text(float x, float y, const std::string& str, Color c) {
        float cursor = x;
        for (char ch : str) {
            addglyph(cursor, y, ch, c);
            cursor += 10.0f;
        }
    }

    void Drawing::line(float x1, float y1, float x2, float y2, Color c) {}

    const std::vector<Vertex>& Drawing::getvertices() {
        return vertices;
    }

    void Drawing::clear() {
        vertices.clear();
    }
}
