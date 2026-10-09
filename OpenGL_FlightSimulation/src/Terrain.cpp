#include "Terrain.h"
#include <glm/glm.hpp>
#include <vector>

// build positions (3 floats per vertex) and indices, compute normals, then call mesh.CompileMesh with interleaved pos+normal
static void computeNormals(const std::vector<float>& positions, const std::vector<unsigned int>& indices, std::vector<float>& outInterleaved) {
    size_t vcount = positions.size() / 3;
    std::vector<glm::vec3> pos(vcount);
    for (size_t i = 0; i < vcount; ++i)
        pos[i] = glm::vec3(positions[3*i+0], positions[3*i+1], positions[3*i+2]);

    std::vector<glm::vec3> normals(vcount, glm::vec3(0.0f));
    for (size_t i = 0; i + 2 < indices.size(); i += 3) {
        unsigned int ia = indices[i+0], ib = indices[i+1], ic = indices[i+2];
        glm::vec3 a = pos[ia], b = pos[ib], c = pos[ic];
        glm::vec3 faceNormal = glm::normalize(glm::cross(b - a, c - a));
        normals[ia] += faceNormal;
        normals[ib] += faceNormal;
        normals[ic] += faceNormal;
    }
    for (size_t i = 0; i < vcount; ++i) {
        if (glm::length(normals[i]) > 0.0f) normals[i] = glm::normalize(normals[i]);
        else normals[i] = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    outInterleaved.clear();
    outInterleaved.reserve(vcount * 6);
    for (size_t i = 0; i < vcount; ++i) {
        outInterleaved.push_back(pos[i].x);
        outInterleaved.push_back(pos[i].y);
        outInterleaved.push_back(pos[i].z);
        outInterleaved.push_back(normals[i].x);
        outInterleaved.push_back(normals[i].y);
        outInterleaved.push_back(normals[i].z);
    }
}

void CreatePerlinTerrain(Mesh &mesh, PerlinNoise &pn, int width, int depth, float scale, float amplitude, int octaves) {
    std::vector<float> positions;
    std::vector<unsigned int> indices;
    positions.reserve((size_t)width * depth * 3);

    // grid centered on origin
    for (int z = 0; z < depth; ++z) {
        for (int x = 0; x < width; ++x) {
            float fx = (x - (width - 1) * 0.5f) * scale;
            float fz = (z - (depth - 1) * 0.5f) * scale;
			float baseFreq = 0.001f; // frequency multiplier for Perlin noise
            float n = pn.fbm(fx * baseFreq, fz * baseFreq, octaves); // tune frequency
            float y = n * amplitude;
            positions.push_back(fx);
            positions.push_back(y);
            positions.push_back(fz);
        }
    }

    for (int z = 0; z < depth - 1; ++z) {
        for (int x = 0; x < width - 1; ++x) {
            unsigned int i0 =  z      * width + x;
            unsigned int i1 =  z      * width + (x + 1);
            unsigned int i2 = (z + 1) * width + x;
            unsigned int i3 = (z + 1) * width + (x + 1);
            // two triangles: i0,i2,i1 and i1,i2,i3  (CCW)
            indices.push_back(i0); indices.push_back(i2); indices.push_back(i1);
            indices.push_back(i1); indices.push_back(i2); indices.push_back(i3);
        }
    }

    std::vector<float> interleaved;
    computeNormals(positions, indices, interleaved);

    mesh.CompileMesh(interleaved, indices);
}
