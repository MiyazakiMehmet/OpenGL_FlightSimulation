#pragma once
#pragma once
#include <vector>
#include <cmath>
#include <numeric>
#include <random>
#include <algorithm>

class PerlinNoise {
private:
    // Permütasyon tablosu (256 eleman x2 = 512, taþmalarý önlemek için çiftlenir)
    std::vector<int> p;

    // 2D uzayda 8 yönlü birim gradyan vektörleri (g)
    const float gradients[8][2] = {
        { 1.0f,  0.0f }, { -1.0f,  0.0f },
        { 0.0f,  1.0f }, {  0.0f, -1.0f },
        { 0.7071f,  0.7071f }, { -0.7071f,  0.7071f },
        { 0.7071f, -0.7071f }, { -0.7071f, -0.7071f }
    };

    // Ken Perlin'in türevi sýfýrlayan Quintic Fade eðrisi: 6t^5 - 15t^4 + 10t^3
    static float fade(float t) {
        return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }

    // Doðrusal enterpolasyon
    static float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

    // Köþedeki gradyan vektörü ile mesafe vektörünün Dot Product çarpýmý (g · d)
    float dotGridGradient(int hash, float dx, float dz) const {
        const float* g = gradients[hash & 7]; // 0-7 arasý bir yön seç
        return (g[0] * dx) + (g[1] * dz);
    }

public:
    // Rastgele bir seed ile veya varsayýlan permütasyonla baþlat
    explicit PerlinNoise(unsigned int seed = 1337) {
        p.resize(256);
        std::iota(p.begin(), p.end(), 0); // 0, 1, 2 ... 255 doldur

        std::default_random_engine engine(seed);
        std::shuffle(p.begin(), p.end(), engine);

        // Diziyi iki kez arka arkaya ekle (p[X + 1] iþlemlerinde sýnýr kontrolüyle uðraþmamak için)
        p.insert(p.end(), p.begin(), p.end());
    }

    // Tek bir katman Perlin Noise hesapla (Sonuç yaklaþýk [-1.0, 1.0] aralýðýnda döner)
    float noise(float x, float z) const {
        // 1. Hücrenin tam sayý köþe koordinatlarýný bul (Negatif koordinatlar için std::floor)
        int X0 = static_cast<int>(std::floor(x)) & 255;
        int Z0 = static_cast<int>(std::floor(z)) & 255;
        int X1 = (X0 + 1) & 255;
        int Z1 = (Z0 + 1) & 255;

        // 2. Noktanýn hücre içindeki yerel konumu (0.0 - 1.0 arasý)
        float xf = x - std::floor(x);
        float zf = z - std::floor(z);

        // 3. Fade eðrisi katsayýlarý
        float u = fade(xf);
        float v = fade(zf);

        // 4. Dört köþe için hash tablosundan gradyan indekslerini al
        int g00 = p[p[X0] + Z0]; // Sol-Alt
        int g10 = p[p[X1] + Z0]; // Sað-Alt
        int g01 = p[p[X0] + Z1]; // Sol-Üst
        int g11 = p[p[X1] + Z1]; // Sað-Üst

        // 5. 4 köþe için (Gradyan · Mesafe) Dot Product hesapla
        // d vektörleri: (xf - köþe_x, zf - köþe_z)
        float n00 = dotGridGradient(g00, xf, zf); // Sol-Alt
        float n10 = dotGridGradient(g10, xf - 1.0f, zf); // Sað-Alt
        float n01 = dotGridGradient(g01, xf, zf - 1.0f); // Sol-Üst
        float n11 = dotGridGradient(g11, xf - 1.0f, zf - 1.0f); // Sað-Üst

        // 6. Fade katsayýlarýyla çift yönlü harmanla (Bilinear Blend)
        float x_alt = lerp(n00, n10, u);
        float x_ust = lerp(n01, n11, u);

        return lerp(x_alt, x_ust, v);
    }

    // fBm
    float fbm(float x, float z, int octaves, float persistence = 0.5f, float lacunarity = 2.1f) const {
        float total = 0.0f;
        float amplitude = 1.0f; // Standart oran 1.0
        float frequency = 1.0f; // Baþlangýc Frekansý
        float maxValue = 0.0f;

        for (int i = 0; i < octaves; ++i) {
            total += noise(x * frequency, z * frequency) * amplitude;
            maxValue += amplitude;

            amplitude *= persistence;
            frequency *= lacunarity;
        }

        return total / maxValue;
    }
};