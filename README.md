# JetBrains Internship 2021 Modern Window-management library for JVM. Author: Alexey Titov aka Alex2772

![Demo](https://sun9-44.userapi.com/impg/kNNs8NyoW5bAZqrfbNqto6QlPh9ksc1ldrzbBA/6UGuCJZjjVI.jpg?size=816x644&quality=96&sign=b17b1755e46a82e9812a5fe5078e2a1b&type=album)

## Why I'm interested
1. I've noticed that IDEA-based IDEs are lagging on hi-dpi monitors (4K on Windows and Retina on MacOS) and I want
   to help to solve this problem
2. I've already implemented [cross platform hardware accelerated UI](https://github.com/Alex2772/aui) with the C++
   language
3. I know how does JVM works inside


## Platform
Linux
- C++
- Xlib
- OpenGL
- CMake

## Works on
- OS: Ubuntu 20.04.1 LTS x86_64
- Compiler: gcc 9.3.0
- GPU: Intel UHD Graphics 620

## Required packages
```sh
sudo apt install libgl-dev cmake g++ build-essential
```

## Build instructions:
1. Install required packages
2. git clone this repository
3. create build folder and cd to it
4. ```cmake .. && make```
5. Execute: ```./jb_triangle```
