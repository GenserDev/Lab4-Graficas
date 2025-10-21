<h1 align="center">
<br>
  Laboratorio 8 - Graficas
</h1>

<p align="center">
<br>
Para su primer ejercicio del primer proyecto, deben cargar el modelo obj que hicieron en el último lab y deben pintarlo en la pantalla usando su función para pintar triángulos. Recorran manualmente las caras, tomen los valores de los índices de los vertices y usenlos para conseguir los tres vértices que se deben utilizar para dibujar un triángulo. Dibujen todos los triangulos en su pantalla. 
</p>
<br>

<p align="center">
<br>
<img align="center" width="793" height="610" alt="StarWars" src="https://github.com/user-attachments/assets/f16e511c-9fcf-4bce-b048-e22dd1fb65ef" />
<br>
</p>

## Requisitos
- Compilador para lenguajes C y C++


## ¿Como ejecutar el programa? 
1. Clonar el repositorio.
```
git clone https://github.com/GenserDev/Lab4-Graficas.git
```
2. Entrar a la carpeta del repositorio.
```
cd Lab4-Graficas
```
3. Abrir su compilador de C y C++ y ejecutar los comandos

- Para compilar
```
g++ -std=c++17 main.cpp framebuffer.cpp triangle.cpp -o renderer.exe $(pkg-config --cflags --libs sdl2)
```
- Para correr el programa
```
./renderer.exe
```
