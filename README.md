# FruityMeshFeaturesetGenerator

A utility program for creating new featureset templates for [FruityMesh](https://github.com/mwaylabs/fruitymesh).

![A demo image of the generator](Demo.png)

## Quick Start

1. Download & install the [Vulkan SDK](https://vulkan.lunarg.com/).
2. Download & install [CMake](https://cmake.org/)
3. Navigate to the repository, open a terminal and execute:
    1. mkdir Build
	2. cd Build
	3. cmake ..
	4. cmake --build . --target FruityMeshFeaturesetGenerator
4. A new executable has been created, e.g. in `Build/bin/Debug/FruityMeshFeaturesetGenerator.exe`

## Usage

1. Enter the full path to your fruitymesh repository. Once the path is correct, the input field will turn blue (red if invalid).
2. Modify the settings to your needs.
3. Look at the three files that are about to be generated on the left.
4. Hit "Save".
5. The three featureset files have been created. WARNING: The program will overwrite existing files if a featureset with the given name already exists.
