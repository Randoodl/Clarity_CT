## Clarity CT - RGB Colour Tool 🎨

A C++ based RGB Colour Picker built with the raylib library.

### 📖 Contents
+ [About](#about)
+ [Features](#features)
+ [Prerequisites](#prerequisites)
+ [Build from Source](#build)

### ❓About <a name = "about"></a>
Clarity CT is a simple, customisable RGB colour picker and palette tool intended to be used alongside graphic design applications.  
It started as a small, fun foray into raylib and turned into the behemoth it is now.

>`"This code is not written very well"`  
>*-Someone who has actually programmed before, presumably*

It is not, and it is not meant to be. This is quite literally just a pet project that grew way beyond the scope it was supposed to have. If you want to have a peek under the hood, that's on you.


### 🛠️ Features <a name = "features"></a>
- **Interactive Colour Picker:** - Select and narrow down specific colours using the colour dial.
- **Display related colours:** - Generate palettes of the selected colour, its complement, and its triads.
- **Copyable HEX and RGB values:** - Easily copy colour values for use in other applications.
- **Custom UI:** - Scale and move elements to create a comfortable layout in an optional Dark Mode.

### 🗹 Prerequisites <a name = "prerequisites"></a>
- **raylib** (www.raylib.com)
- **GNU Make** (www.gnu.org/software/make)

### 🏗️ Building from source <a name = "build"></a>
Compilation is currently only tested with C++20 on GCC 13.3
#### Using Make:
Clone the repository and compile using Make
```sh
git clone https://github.com/Randoodl/Clarity_CT.git
cd Clarity_CT
make
```