# Scripts

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
⋅⋅⋅```libx11-dev``` gives you the x header files
⋅⋅⋅```libbluetooth-dev``` gives you the bluetooth linux header files
+ Run ```git clone https://github.com/PolarToCartesian/Weiss-lib``` to clone the repository in your desired directory
+ Run ```cd Weiss-lib``` to navigate the cloned repository
+ Run ```cmake .``` to generate the build files
+ Run ```make``` to build the application