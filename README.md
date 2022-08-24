# Specter

Specter is a cross-platform Dear ImGui/ImPlot based data visualization tool designed for online analysis in nuclear physics experiments. It is a C++ library/framework with capability of displaying 1D and 2D
spectra, applying cuts and gates graphically to spectra, and has a customizable analysis system. Specter is focused on providing a clean, pretty UI with good performance required by demainding experimental setups.

The only external dependencies are OpenGL development headers, as OpenGL is the rendering API, and the
C++17 standard library. All other dependencies are included as git submodules in the vendor directory. The current state of the library is such that the only readily developed data source type is for CAEN's CoMPASS DAQ for use with their digitizer modules. There are tentative plans to extend the sources to other DAQ frameworks.

This project would not be possible with out the amazing online tutorials of [@TheCherno](https://github.com/TheCherno) and in particular his Hazel tutorials, which much of the application model and basically
all of the renderer is based off of ([Hazel](https://github.com/TheCherno/Hazel)). I highly recommend checking out his work to anyone who is interested in projects like this, or just learning more about C++.

Additonally, the excellent [DearImGui](https://github.com/ocornut/imgui) and [ImPlot](https://github.com/epezent/implot) libraries are obviously critical to this work, and also deserve a look for anyone who wants
to design UI's and plotting tools in C++.

## Building Specter

Specter should be cloned from github using `git clone --recursive https://github.com/gwm17/Specter.git` The recursive flag tells git to clone not only the repository but also all of the submodule dependencies.

Specter uses CMake as its build system. If you are unfamiliar with CMake, building Specter is very straightforward. From within the Specter repository run the following commands:

- `mkdir build`
- `cd build && cmake .. && make -j 4 && cd ..`

This will build Specter and the example SpecProject, placing the Specter library in the created `lib` directory and the SpecProject executable in the created `bin` directory.

Note: On Linux distributions, typically Mesa OpenGL and X-window related header files are not installed by default. These can be installed using whatever package manager your distribution uses. For example, on Debian family distributions the necessary files can be installed using `sudo apt install libgl1 libgl1-mesa-dev libglu1-mesa libglu1-mesa-dev xorg-dev mesa-utils` which should fill out all of the dependencies. If this doesn't seem to work, check your distribution related documentation for OpenGL an X11 dependencies.

## Running Specter

Once Specter and SpecProject are built, simply run SpecProject either through an IDE or by moving to the `bin` directory of the repository and executing `SpecProject`. Note that the program is SpecProject. Specter is just a library/framework. Additionally, Specter depends on several resources (fonts, images, etc). These are stored in the `SpecProject` directory, and so running Specter from anywhere other than the `bin` or `SpecProject` directory will usually result in a crash. If needed this behavior can be changed by modifying the code in SpecProject.

SpecProject is an example showing what a typical implementation might look like, based on the Super-Enge Split-Pole Spectrograph at Florida State University. This can be used as a launching point for a project, or you can link to Specter your self. Note that if you want to link to Specter yourself, its important that you copy the resources directory to your project, as this contains the icons and font used by Specter. The resources should be placed wherever your program will be run from (the runtime directory).

Note: As Specter is still very early in development, the default build configuration is Debug. If you are actually going to use it, you will want to make sure that you switch to building in Release configuration otherwise performance may be poor.

## Suppported data sources
By default Specter only supports CAEN CoMPASS data sources.
### Status as of Aug 2022
Unfortunately, at the time of writing, CoMPASS does not fully support the use of external data analysis. CoMPASS versions 1.3-2.0 have a data socket, which can not be accessed outside of the localhost, meaning that use with Specter generally requires an additional server broadcasting the data to the machine running Specter, which is non-trivial. CoMPASS began supporting a public data socket as of version 2.1, however, it is now by default a time-sorted data socket. The time-sorting operation is so inefficient that any significant data rate on the aquisition basically results in a soft crash of CoMPASS. Currently there is no option to turn off the time sorting on the data socket. CAEN has been alerted to this issue, so hopefully this will be rectified shortly.