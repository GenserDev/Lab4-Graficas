#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

struct Face {
    std::vector<int> vertexIndices;
    std::string groupName;  // Nombre del grupo al que pertenece esta cara
};

struct Model {
    std::vector<glm::vec3> vertices;
    std::vector<Face> faces;
};

bool loadOBJ(const std::string& filename, Model& model) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return false;
    }

    std::string line;
    std::string currentGroup = "default";  // Grupo actual
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            // Vértice
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            model.vertices.push_back(vertex);
        }
        else if (prefix == "g" || prefix == "o") {
            // Grupo u objeto
            iss >> currentGroup;
            std::cout << "Grupo encontrado: " << currentGroup << std::endl;
        }
        else if (prefix == "f") {
            // Cara
            Face face;
            face.groupName = currentGroup;  // Asignar el grupo actual
            std::string vertexData;
            
            while (iss >> vertexData) {
                std::istringstream viss(vertexData);
                std::string indexStr;
                
                // Obtener el primer índice (vértice)
                std::getline(viss, indexStr, '/');
                int vertexIndex = std::stoi(indexStr) - 1; // OBJ usa índices base 1
                face.vertexIndices.push_back(vertexIndex);
            }
            
            model.faces.push_back(face);
        }
    }

    file.close();
    
    std::cout << "Modelo cargado: " << model.vertices.size() << " vértices, " 
              << model.faces.size() << " caras" << std::endl;
    
    return true;
}