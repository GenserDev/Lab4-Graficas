#include "triangle.h"
#include "framebuffer.h"
#include <algorithm>
#include <cmath>

void triangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
    // Convertir coordenadas 3D a 2D (proyección simple)
    glm::vec2 p1(v1.x, v1.y);
    glm::vec2 p2(v2.x, v2.y);
    glm::vec2 p3(v3.x, v3.y);

    // Encontrar el bounding box del triángulo
    int minX = static_cast<int>(std::floor(std::min({p1.x, p2.x, p3.x})));
    int maxX = static_cast<int>(std::ceil(std::max({p1.x, p2.x, p3.x})));
    int minY = static_cast<int>(std::floor(std::min({p1.y, p2.y, p3.y})));
    int maxY = static_cast<int>(std::ceil(std::max({p1.y, p2.y, p3.y})));

    // Clamp al tamaño de la pantalla
    minX = std::max(0, minX);
    maxX = std::min(SCREEN_WIDTH - 1, maxX);
    minY = std::max(0, minY);
    maxY = std::min(SCREEN_HEIGHT - 1, maxY);

    // Función para calcular coordenadas baricéntricas
    auto sign = [](const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3) {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    };

    // Rasterizar cada pixel dentro del bounding box
    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            glm::vec2 pixelPoint(x + 0.5f, y + 0.5f);

            float d1 = sign(pixelPoint, p1, p2);
            float d2 = sign(pixelPoint, p2, p3);
            float d3 = sign(pixelPoint, p3, p1);

            bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
            bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

            // El punto está dentro del triángulo si todos los signos son iguales
            if (!(hasNeg && hasPos)) {
                point(x, y);  // Esta es la función que dibuja el pixel
            }
        }
    }
}