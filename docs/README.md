# Weiss' Contents

## Weiss Core Engine

+ A **Templated SIMD Math Library** that uses SIMD Extensions when available
+ An **Image Reading/Writing Library** with a custom PNG encoder/decoder
+ A **Windowing Library** that uses win32 api calls on windows and X11 on linux
+ A **Networking Socket Library** that makes networking easier to handle

## Weiss Editor

+ ...

# Building

## Setup On Windows

+ Download & Install [Visual Studio](https://visualstudio.microsoft.com/downloads) With the C++ Extensions
+ Download & Install The [DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
+ Download & Install A [Vulkan SDK](https://vulkan.lunarg.com/)
+ Download & Install [CMake](https://cmake.org/download/)
+ Download & Install [Git](https://git-scm.com/)
+ Run ```git clone https://github.com/PolarToCartesian/Weiss-lib``` to clone the repository
+ Run ```cd Weiss-lib``` to navigate the cloned repository
+ Run ```cmake .``` to generate the build files
+ Open up the solution directory and click on build/run

## Setup On Linux (Debian/Ubuntu)

+ Run ```sudo apt-get update```
+ Run ```sudo apt-get install git g++ libx11-dev libbluetooth-dev cmake make```
  * ```libx11-dev``` gives you the x header files
  * ```libbluetooth-dev``` gives you the bluetooth linux header files
+ Run ```git clone https://github.com/PolarToCartesian/Weiss-lib``` to clone the repository in your desired directory
+ Run ```cd Weiss-lib``` to navigate the cloned repository
+ Run ```cmake .``` to generate the build files
+ Run ```make``` to build the application

# Documentation Used By Weiss-lib

## For The PNG Encoder/Decoder

+ [PNG Specification](libpng-png-1.2.pdf) from [libpng](http://www.libpng.org/pub/png/)
+ [Deflate Specification](DEFLATE-rfc1951.pdf)
+ [ZLIB Specification](ZLIB-rfc1950.pdf)

## Graphics APIs

+ [Vulkan Specification](vkspec.pdf) from [The Khronos Group](https://www.khronos.org/)

Thanks to all of the authors and maintainers of these open file formats!
