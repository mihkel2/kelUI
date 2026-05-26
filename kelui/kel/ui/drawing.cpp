#include "drawing.hpp"

namespace kel {
    std::vector<Vertex> Drawing::vertices;

    void Drawing::rect(float x, float y, float w, float h) {

        vertices.push_back({x, y, c.r, c.g, c.b, c.a});
        vertices.push_back({x + w, y, c.r, c.g, c.b, c.a});
        vertices.push_back({x, y + h, c.r, c.g, c.b, c.a});

        vertices.push_back({x + w, y, c.r, c.g, c.b, c.a});
        vertices.push_back({x + w, y + h, c.r, c.g, c.b, c.a});
        vertices.push_back({x, y + h, c.r, c.g, c.b, c.a});
    }

    void Drawing::line(float x1, float y1, float x2, float y2) {

    }

    void Drawing::text(float x, float y, const char* str) {

    }

    const std::vector<Vertex>& Drawing::getvertices() {
        return vertices;
    }

    void Drawing::clear() {
        vertices.clear();
    }
}
