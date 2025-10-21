#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include "color.h"
#include "framebuffer.h"
#include "triangle.h"
#include "objLoader.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Model spaceshipModel;

// Factor de escala y offset (se calcularán automáticamente)
float SCALE = 20.0f;
glm::vec3 OFFSET(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);

// Mapa de colores para cada grupo
std::map<std::string, Color> groupColors;

void setupColors() {
    // Colores basados en el TIE Fighter
    groupColors["ala-derecha"] = Color(40, 40, 45);      // Gris oscuro para las alas
    groupColors["ala-izquierda"] = Color(40, 40, 45);    // Gris oscuro para las alas
    groupColors["sphere"] = Color(120, 130, 140);        // Gris medio para el cuerpo
    groupColors["cylinder"] = Color(90, 95, 100);        // Gris medio-oscuro para cilindros
    groupColors["conector-ala-1"] = Color(80, 85, 90);   // Gris para conectores
    groupColors["default"] = Color(120, 130, 140);       // Color por defecto
    
    std::cout << "\nColores configurados para los grupos:" << std::endl;
    for (const auto& pair : groupColors) {
        std::cout << "  " << pair.first << ": RGB(" 
                  << (int)pair.second.r << ", " 
                  << (int)pair.second.g << ", " 
                  << (int)pair.second.b << ")" << std::endl;
    }
}

Color getColorForGroup(const std::string& groupName) {
    auto it = groupColors.find(groupName);
    if (it != groupColors.end()) {
        return it->second;
    }
    return groupColors["default"];
}

void calculateOptimalScale() {
    if (spaceshipModel.vertices.empty()) return;
    
    // Encontrar los límites del modelo (bounding box)
    glm::vec3 minBounds = spaceshipModel.vertices[0];
    glm::vec3 maxBounds = spaceshipModel.vertices[0];
    
    for (const auto& vertex : spaceshipModel.vertices) {
        minBounds.x = std::min(minBounds.x, vertex.x);
        minBounds.y = std::min(minBounds.y, vertex.y);
        minBounds.z = std::min(minBounds.z, vertex.z);
        
        maxBounds.x = std::max(maxBounds.x, vertex.x);
        maxBounds.y = std::max(maxBounds.y, vertex.y);
        maxBounds.z = std::max(maxBounds.z, vertex.z);
    }
    
    // Calcular el tamaño del modelo
    glm::vec3 size = maxBounds - minBounds;
    
    // Calcular el centro del modelo en coordenadas del objeto
    glm::vec3 center = (minBounds + maxBounds) * 0.5f;
    
    // Invertir Y del centro porque lo invertiremos en transformVertex
    center.y = -center.y;
    
    // Centrar el modelo
    OFFSET = glm::vec3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f) - (center * SCALE);
    
    std::cout << "\nBounding Box: Min(" << minBounds.x << ", " << minBounds.y << ", " << minBounds.z << ")" << std::endl;
    std::cout << "             Max(" << maxBounds.x << ", " << maxBounds.y << ", " << maxBounds.z << ")" << std::endl;
    std::cout << "Centro del modelo: (" << center.x << ", " << center.y << ", " << center.z << ")" << std::endl;
    std::cout << "Tamaño del modelo: " << size.x << " x " << size.y << " x " << size.z << std::endl;
    std::cout << "Escala: " << SCALE << std::endl;
    std::cout << "Offset: (" << OFFSET.x << ", " << OFFSET.y << ", " << OFFSET.z << ")" << std::endl;
}

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Software Renderer - TIE Fighter", 
                               SDL_WINDOWPOS_CENTERED, 
                               SDL_WINDOWPOS_CENTERED, 
                               SCREEN_WIDTH, 
                               SCREEN_HEIGHT, 
                               SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void setColor(const Color& color) {
    currentColor = color;
}

glm::vec3 transformVertex(const glm::vec3& vertex) {
    // Escalar el vértice
    glm::vec3 scaled = vertex * SCALE;
    
    // Invertir el eje Y porque en pantalla Y aumenta hacia abajo
    scaled.y = -scaled.y;
    
    // Trasladar para centrar
    glm::vec3 transformed = scaled + OFFSET;
    
    return transformed;
}

void render() {
    // Iterar sobre todas las caras del modelo
    for (const Face& face : spaceshipModel.faces) {
        // Obtener el color para este grupo
        Color faceColor = getColorForGroup(face.groupName);
        setColor(faceColor);
        
        // Triangulamos la cara si tiene más de 3 vértices
        if (face.vertexIndices.size() >= 3) {
            // Tomamos el primer vértice como pivote para triangular
            int i0 = face.vertexIndices[0];
            glm::vec3 v0 = transformVertex(spaceshipModel.vertices[i0]);
            
            // Crear triángulos en forma de abanico
            for (size_t i = 1; i < face.vertexIndices.size() - 1; ++i) {
                int i1 = face.vertexIndices[i];
                int i2 = face.vertexIndices[i + 1];
                
                glm::vec3 v1 = transformVertex(spaceshipModel.vertices[i1]);
                glm::vec3 v2 = transformVertex(spaceshipModel.vertices[i2]);
                
                // Dibujar el triángulo
                triangle(v0, v1, v2);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    // Cargar el modelo OBJ
    if (!loadOBJ("../models/StarWars.obj", spaceshipModel)) {
        std::cerr << "Error al cargar el modelo. Asegúrate de que StarWars.obj esté en el directorio correcto." << std::endl;
        return 1;
    }

    // Configurar los colores para cada grupo
    setupColors();
    
    // Calcular la escala y offset óptimos para centrar el modelo
    calculateOptimalScale();

    init();
    bool running = true;
    
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            // Presiona ESC para salir
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        clear();
        render();
        renderBuffer(renderer);
        
        // Pequeño delay para no consumir el 100% del CPU
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}