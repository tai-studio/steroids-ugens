# Steroids UGens
*2020, LFSaw ([interaction](http://tai-studio.org) | [music](http://lfsaw.de))*

SuperCollider standard UGens on steroids.
Currently implemented:

+ `SSinOscFB` is `SinOscFB` with `ar`-modulable parameters (`freq`, `fb`),
+ `TDemand` is `Demand` as trigger version (like `TDuty`).

Implemented using the [SC Cookiecutter](https://github.com/supercollider/cookiecutter-supercollider-plugin) framework.

-----

### Requirements

- CMake >= 3.5
- SuperCollider source code

### Building

Clone the project:

    git clone https://github.com/tai-studio/steroids-ugens
    cd steroids-ugens
    mkdir build
    cd build

Then, use CMake to configure and build it:

    cmake .. -DCMAKE_BUILD_TYPE=Release
    cmake --build . --config Release
    cmake --build . --config Release --target install

You may want to manually specify the install location in the first step to point it at your
SuperCollider extensions directory: add the option `-DCMAKE_INSTALL_PREFIX=/path/to/extensions`.

It's expected that the SuperCollider repository is cloned at `../supercollider` relative to this repo. If
it's not, add the option `-DSC_PATH=/path/to/sc/source` to the initial cmake command.

### Developing

Use the command in `regenerate` to update `CMakeLists.txt` when you add or remove files from the
project. You do not need to run it if you only change the contents of existing files. You may need to
edit the command if you add, remove, or rename plugins to match the new plugin paths. Run the
script with `--help` to see all available options.
