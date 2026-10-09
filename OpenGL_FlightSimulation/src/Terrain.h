#pragma once

#include "Mesh.h"
#include "PerlinNoise.h"

// Generate a heightfield mesh using Perlin noise and upload to the provided Mesh.
// width x depth grid, centered at origin.
// scale: spacing between samples; amplitude: vertical scale; octaves: fBm detail.
void CreatePerlinTerrain(Mesh &mesh, PerlinNoise &pn, int width, int depth, float scale, float amplitude, int octaves);
