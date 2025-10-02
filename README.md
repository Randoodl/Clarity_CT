## Clarity CT - RGB Colour Tool 🎨

A C++ based RGB Colour Picker Tool built with the raylib library.

### 📖 Contents
+ [About](#about)
+ [Features](#features)
+ [Prerequisites](#prerequisites)
+ [Build from Source](#build)

### ❓About <a name = "about"></a>
Clarity CT is a simple, customisable RGB colour picker and palette tool intended to be used alongside graphic design applications.  
Its whole purpose is to provide you with an easily copyable colour value in either RGB or HEX notation.  
It started as a small, fun foray into raylib and turned into the behemoth it is now.

>`"This code is not written very well"`  
>*-Someone who has actually programmed before, presumably*

It is not, and it is not meant to be. This is quite literally just a pet project that grew way beyond the scope it was supposed to have. If you want to have a peek under the hood, that's on you.


### 🛠️ Features <a name = "features"></a>
- **Interactive Colour Picker:** - Select a specific hue using the colour dial and narrow down its shade using the shader square.
- **Display related colours:** - Automatically generate complement and triadic colours, alongside shades and tints thereof.
- **Copyable HEX and RGB values:** - Easily copy colour values for use in other applications with but a single click.
- **Custom UI:** - Scale and move elements to create a comfortable layout in an optional Dark Mode.

### 🗹 Prerequisites <a name = "prerequisites"></a>
- **raylib** (www.raylib.com)
- **GNU Make** (www.gnu.org/software/make)

### 🏗️ Building from source <a name = "build"></a>
Compilation is currently only tested with C++20 on GCC 13.3 and raylib v5.6
#### Using Make:
Clone the repository and compile using Make
```sh
git clone https://github.com/Randoodl/Clarity_CT.git
cd Clarity_CT
make
```