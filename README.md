# Blocks Engine

This Engine was developed as a way to learn DirectX and modern 3D Graphics. The solution contains two projects, one being the Engine and the other an implementation of Minecraft using the Blocks Engine. 

# Used Technologies

* Direct3D for 3D rendering
* Direct2D for hardware accelerated 2D rendering
* DirectWrite for fast text rendering in the 2D context
* Nvidia PhysX for physics simulation
* Win32 to handle the OS-Communication and Window management

# Important

Currently the Engine only runs in the Debug Mode. PhysX had to be added as fast as possible to comply with a school deadline. To achieve this deadline the multithreading was not implemented correctly and there are multiple raceconditions which i have not had the time to fix yet. These raceconditions log warnings in Debug Mode, however, in Release they are critical errors which stop execution.

# Installation

To install the Engine first clone the repository.

    git clone https://github.com/jorgeparavicini/Blocks.git

Then download [vcpkg](https://vcpkg.io/en/index.html) and install physX. Important is, that the 64 bit version is specified.

    vcpkg.exe install physx:x64-windows 

After this open the solution and run the Blocks project in Debug mode.
