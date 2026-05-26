#pragma once
#include <vector>
#include <string>

namespace kel {
    struct Color {
        float r, g, b, a;
    };

    struct Vertex {
        float x, y;
        float u, v; // Texture coordinates
        float r, g, b, a;
    };

    class Drawing {
    public:
        static void rect(float x, float y, float w, float h, Color color);
        static void line(float x1, float y1, float x2, float y2, Color color);
        static void text(float x, float y, const std::string& str, Color color);

        static const std::vector<Vertex>& getvertices();
        static void clear();

    private:
        static std::vector<Vertex> vertices;
        static void addglyph(float x, float y, char c, Color color);
    };
}
