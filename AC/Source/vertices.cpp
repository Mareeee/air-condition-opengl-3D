#define _USE_MATH_DEFINES
#include "../Header/vertices.h"
#include <string>
#include <cmath>
#include <map>
#define NUM_SLICES 40
#include <vector>

float verticesButton[] = {
     0.75f,  0.2f, -1.30f,  0.0f,  0.0f, -1.0f,
     0.8f,  0.2f, -1.30f,  0.0f,  0.0f, -1.0f,
     0.8f,  0.25f, -1.30f,  0.0f,  0.0f, -1.0f,
     0.8f,  0.25f, -1.30f,  0.0f,  0.0f, -1.0f,
     0.75f,  0.25f, -1.30f,  0.0f,  0.0f, -1.0f,
     0.75f,  0.2f, -1.30f,  0.0f,  0.0f, -1.0f,

     0.75f,  0.2f, -1.25f,  0.0f,  0.0f, 1.0f,
     0.8f,  0.2f, -1.25f,  0.0f,  0.0f, 1.0f,
     0.8f,  0.25f, -1.25f,  0.0f,  0.0f, 1.0f,
     0.8f,  0.25f, -1.25f,  0.0f,  0.0f, 1.0f,
     0.75f,  0.25f, -1.25f,  0.0f,  0.0f, 1.0f,
     0.75f,  0.2f, -1.25f,  0.0f,  0.0f, 1.0f,

     0.75f,  0.25f, -1.25f, -1.0f,  0.0f,  0.0f,
     0.75f,  0.25f, -1.30f, -1.0f,  0.0f,  0.0f,
     0.75f,  0.2f, -1.30f, -1.0f,  0.0f,  0.0f,
     0.75f,  0.2f, -1.30f, -1.0f,  0.0f,  0.0f,
     0.75f,  0.2f, -1.25f, -1.0f,  0.0f,  0.0f,
     0.75f,  0.25f, -1.25f, -1.0f,  0.0f,  0.0f,

     0.8f,  0.25f, -1.25f,  1.0f,  0.0f,  0.0f,
     0.8f,  0.25f, -1.30f,  1.0f,  0.0f,  0.0f,
     0.8f,  0.2f, -1.30f,  1.0f,  0.0f,  0.0f,
     0.8f,  0.2f, -1.30f,  1.0f,  0.0f,  0.0f,
     0.8f,  0.2f, -1.25f,  1.0f,  0.0f,  0.0f,
     0.8f,  0.25f, -1.25f,  1.0f,  0.0f,  0.0f,

     0.75f,  0.2f, -1.30f,  0.0f, -1.0f,  0.0f,
     0.8f,  0.2f, -1.30f,  0.0f, -1.0f,  0.0f,
     0.8f,  0.2f, -1.25f,  0.0f, -1.0f,  0.0f,
     0.8f,  0.2f, -1.25f,  0.0f, -1.0f,  0.0f,
     0.75f,  0.2f, -1.25f,  0.0f, -1.0f,  0.0f,
     0.75f,  0.2f, -1.30f,  0.0f, -1.0f,  0.0f,

     0.75f,  0.25f, -1.30f,  0.0f,  1.0f,  0.0f,
     0.8f,  0.25f, -1.30f,  0.0f,  1.0f,  0.0f,
     0.8f,  0.25f, -1.25f,  0.0f,  1.0f,  0.0f,
     0.8f,  0.25f, -1.25f,  0.0f,  1.0f,  0.0f,
     0.75f,  0.25f, -1.25f,  0.0f,  1.0f,  0.0f,
     0.75f,  0.25f, -1.30f,  0.0f,  1.0f,  0.0f
};

float verticesПречага[] = {
    -0.925f,  0.0f,  -1.7f,  0.0f,  0.0f, -1.0f,
    0.775f,  0.0f,  -1.7f,  0.0f,  0.0f, -1.0f,
    0.775f,  -0.02f, -1.7f,  0.0f,  0.0f, -1.0f,
    0.775f,  -0.02f, -1.7f,  0.0f,  0.0f, -1.0f,
    -0.925f,  -0.02f, -1.7f,  0.0f,  0.0f, -1.0f,
    -0.925f,  0.0f,  -1.7f,  0.0f,  0.0f, -1.0f,

    -0.925f,   0.03f,   -1.35f,  0.0f,  0.0f, 1.0f,
    0.775f,   0.03f,   -1.35f,  0.0f,  0.0f, 1.0f,
    0.775f,   0.05f,   -1.35f,  0.0f,  0.0f, 1.0f,
    0.775f,   0.05f,   -1.35f,  0.0f,  0.0f, 1.0f,
    -0.925f,   0.05f,   -1.35f,  0.0f,  0.0f, 1.0f,
    -0.925f,   0.03f,   -1.35f,  0.0f,  0.0f, 1.0f,

    -0.925f,   0.05f, -1.35f, -1.0f,  0.0f,  0.0f,
    -0.925f,   0.0f,  -1.7f, -1.0f,  0.0f,  0.0f,
    -0.925f,  -0.02f, -1.7f, -1.0f,  0.0f,  0.0f,
    -0.925f,   0.05f, -1.35f, -1.0f,  0.0f,  0.0f,
    -0.925f,   0.03f, -1.35f, -1.0f,  0.0f,  0.0f,
    -0.925f,  -0.02f, -1.7f, -1.0f,  0.0f,  0.0f,

    0.775f,  0.05f, -1.35f,  1.0f,  0.0f,  0.0f,
    0.775f,  0.0f, -1.7f,  1.0f,  0.0f,  0.0f,
    0.775f,  -0.02f, -1.7f,  1.0f,  0.0f,  0.0f,
    0.775f,  0.05f, -1.35f,  1.0f,  0.0f,  0.0f,
    0.775f,  0.03f, -1.35f,  1.0f,  0.0f,  0.0f,
    0.775f,  -0.02f, -1.7f,  1.0f,  0.0f,  0.0f,

    -0.925f,  -0.02f, -1.7f,  0.0f, -1.0f,  0.0f,
    0.775f,  -0.02f, -1.7f,  0.0f, -1.0f,  0.0f,
    0.775f,  0.03f, -1.35f,  0.0f, -1.0f,  0.0f,
    0.775f,  0.03f, -1.35f,  0.0f, -1.0f,  0.0f,
    -0.925f,  0.03, -1.35f,  0.0f, -1.0f,  0.0f,
    -0.925f,  -0.02f, -1.7f,  0.0f, -1.0f,  0.0f,

    -0.925f,  0.0f, -1.7f,  0.0f,  1.0f,  0.0f,
    0.775f,  0.0f, -1.7f,  0.0f,  1.0f,  0.0f,
    0.775f,  0.05f, -1.35f,  0.0f,  1.0f,  0.0f,
    0.775f,  0.05f, -1.35f,  0.0f,  1.0f,  0.0f,
    -0.925f,  0.05f, -1.35f,  0.0f,  1.0f,  0.0f,
    -0.925f,  0.0f, -1.7f,  0.0f,  1.0f,  0.0f
};

float verticesCrosshair[] = {
    -0.0056f, -0.01f, 0.0f,  0,0,1,
     0.0056f, -0.01f, 0.0f,  0,0,1,
     0.0056f,  0.01f, 0.0f,  0,0,1,
     0.0056f,  0.01f, 0.0f,  0,0,1,
    -0.0056f,  0.01f, 0.0f,  0,0,1,
    -0.0056f, -0.01f, 0.0f,  0,0,1
};

std::vector<float> generateCylinder(float radius, float height, int segments) {
    std::vector<float> vertices;
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float nextTheta = 2.0f * 3.1415926f * float(i + 1) / float(segments);

        float x1 = radius * cos(theta);
        float z1 = radius * sin(theta);
        float x2 = radius * cos(nextTheta);
        float z2 = radius * sin(nextTheta);

        vertices.insert(vertices.end(), { x1, 0.0f, z1, 0.0f, 1.0f, 1.0f });
        vertices.insert(vertices.end(), { x2, 0.0f, z2, 0.0f, 1.0f, 1.0f });
        vertices.insert(vertices.end(), { x1, height, z1, 0.0f, 0.0f, 1.0f });

        vertices.insert(vertices.end(), { x1, height, z1, 0.0f, 0.0f, 1.0f });
        vertices.insert(vertices.end(), { x2, 0.0f, z2, 0.0f, 1.0f, 1.0f });
        vertices.insert(vertices.end(), { x2, height, z2, 0.0f, 0.0f, 1.0f });

        vertices.insert(vertices.end(), { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f });
        vertices.insert(vertices.end(), { x1, 0.0f, z1, 0.0f, 1.0f, 0.0f });
        vertices.insert(vertices.end(), { x2, 0.0f, z2, 0.0f, 1.0f, 0.0f });
    }
    return vertices;
}

std::vector<float> generateSphere(float radius, int segments) {
    std::vector<float> triangleVertices;
    for (int i = 0; i < segments; ++i) {
        for (int j = 0; j < segments; ++j) {
            float lat1 = 3.1415926f * (-0.5f + (float)i / segments);
            float lat2 = 3.1415926f * (-0.5f + (float)(i + 1) / segments);
            float lon1 = 2.0f * 3.1415926f * (float)j / segments;
            float lon2 = 2.0f * 3.1415926f * (float)(j + 1) / segments;

            auto addVert = [&](float lat, float lon) {
                float x = cos(lat) * cos(lon);
                float y = cos(lat) * sin(lon);
                float z = sin(lat);
                triangleVertices.insert(triangleVertices.end(), { x * radius, y * radius, z * radius, x, y, z });
                };

            addVert(lat1, lon1); addVert(lat2, lon1); addVert(lat1, lon2);
            addVert(lat1, lon2); addVert(lat2, lon1); addVert(lat2, lon2);
        }
    }
    return triangleVertices;
}

std::map<std::string, float*> getVerticesMap() {
    std::map<std::string, float*> m;

    m["verticesButton"] = verticesButton;
    m["verticesПречага"] = verticesПречага;
    m["verticesCrosshair"] = verticesCrosshair;

    return m;
}

std::map<std::string, size_t> getVerticesSizes() {
    std::map<std::string, size_t> s;

    s["verticesButton"] = sizeof(verticesButton);
    s["verticesПречага"] = sizeof(verticesПречага);
    s["verticesCrosshair"] = sizeof(verticesCrosshair);

    return s;
}
