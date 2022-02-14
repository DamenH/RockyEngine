You need VSCode CMake Tools extension installed.

I built and installed Ogre with vcpkg.

```
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install ogre
```

This didn't actually install vcpkg globally, but it built Ogre, and that's all I care about.

The `c_cpp_properties.json` in `.vscode` directory is pretty specific, so if you have any problems, check there first.

# Building and Running
To build search `CMake: Build` in the command palette or just press `F7`.

`./build/0_Bootstrap` to run.

# Debugging
To debug and run, search `CMake: Debug` in the command palette or press `Ctrl + F5`.
