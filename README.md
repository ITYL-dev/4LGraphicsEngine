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

Add the created binaries to Libs/lib. Note that the content of Libs/include is a copy of glfw/include

