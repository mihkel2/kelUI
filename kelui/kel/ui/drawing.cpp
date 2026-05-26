#include "drawing.hpp"

namespace kel {
    std::vector<Vertex> Drawing::vertices;

    void Drawing::rect(float x, float y, float w, float h, Color c) {

        vertices.push_back({x, y, -1.0f, -1.0f, c.r, c.g, c.b, c.a});
        vertices.push_back({x + w, y, -1.0f, -1.0f, c.r, c.g, c.b, c.a});
        vertices.push_back({x, y + h, -1.0f, -1.0f, c.r, c.g, c.b, c.a});

        vertices.push_back({x + w, y, -1.0f, -1.0f, c.r, c.g, c.b, c.a});
        vertices.push_back({x + w, y + h, -1.0f, -1.0f, c.r, c.g, c.b, c.a});
        vertices.push_back({x, y + h, -1.0f, -1.0f, c.r, c.g, c.b, c.a});
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

        vertices.push_back({x, y, u1, v1, color.r, color.g, color.b, color.a});
        vertices.push_back({x + size, y, u2, v1, color.r, color.g, color.b, color.a});
        vertices.push_back({x, y + size, u1, v2, color.r, color.g, color.b, color.a});

        vertices.push_back({x + size, y, u2, v1, color.r, color.g, color.b, color.a});
        vertices.push_back({x + size, y + size, u2, v2, color.r, color.g, color.b, color.a});
        vertices.push_back({x, y + size, u1, v2, color.r, color.g, color.b, color.a});
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
