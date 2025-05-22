# 4LGraphicsEngine
For Learning Graphics Engine based on OpenGL

## Compiling GLFW from source

Clone the repo with submodules: 
```
git clone https://github.com/ITYL-dev/4LGraphicsEngine.git --recursive
```

### Dependencies
Debian :

```
sudo apt install cmake
sudo apt install xorg-dev
sudo apt install libwayland-dev libxkbcommon-dev wayland-protocols extra-cmake-modules
```

Windows :
Cmake et Visual studio

### Build

```
cmake glfw -B glfw/build
cmake --build glfw/build
```

### Libs folder

Add the created binaries (found somewhere in glfw/build/src) to Libs/lib. Note that the content of Libs/include is a copy of glfw/include

### Linking

Add Libs/include in your Include directories and Libs/lib in your Library directories, and link the GLFW and openGL binaries (Windows : glfw3.lib, opengl32.lib. Linux : libglfw3.a, libGL.so. GCC linker options : `-lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl`). This is already done on Windows in the Visual Studio solution. TODO: use CMake to automatically do that for both systems, or make makefile

## GLAD

[glad](https://glad.dav1d.de/)

