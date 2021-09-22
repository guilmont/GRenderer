#pragma once

#include "GRender.h"


class Circle
{

public:
    Circle(uint32_t numCircles, uint32_t numVertices);
    ~Circle(void);

    void draw(const glm::vec3 &position, float radius, const glm::vec4 &color);
    void submit(float thickness = 1.0f);

private:
    struct Vertex
    {
        glm::vec3 pos;
        glm::vec4 color;
    };

    uint32_t
        vao, // Vertex array object
        vertex_buffer,
        index_buffer;

    uint32_t
        counter = 0, // Current number of quads
        maxVertices; // Maximum number of quads

    std::vector<glm::vec2> unitary;


    std::vector<uint32_t> vIndex;
    std::vector<Vertex> vtxBuffer;

}; // class-object