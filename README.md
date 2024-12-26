# X360RayTracing

Ray tracing experiment on the Xbox 360.

![Ray traced spheres](./resources/screenshots/spheres.jpg)

This project is an Xbox 360 port of [TheCherno's RayTracing renderer](https://github.com/TheCherno/RayTracing) built entirely on YouTube in [a fantastic series](https://www.youtube.com/playlist?list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl).
The Cherno implemented a CPU-side ray tracer but this project utilizes the GPU as much as possible to do the heavy calculations.

## Disclaimer

There is currently a bug when running the code on real hardware. The image is split in half and forms 2 triangles, the bottom right triangle renders properly but the upper left one doesn't.

![Rendering but on real hardware](./resources/screenshots/spheres-bug.jpg)

It looks like a strange behavior of the `IDirect3DDevice9::Resolve` implementation on Xbox 360 because the bug doesn't occur in Xenia. It seems to happen when sampling from a texture that was written to using `IDirect3DDevice9::Resolve`.

If anyone has any explanation or a potential fix, feel free to open an issue and/or a pull request!

## Building

### Requirements

-   Having the Xbox 360 Software Development Kit (XDK) installed.

### Visual Studio 2010

Open `X360RayTracing.sln` in Visual Studio.

**Note**: Using Visual Studio 2010 is the only way to run X360RayTracing in a debugger (sadly).

### Visual Studio 2022 (or any environment)

You can't build with the 64-bit version of MSBuild so you'll need to run the 32-bit version manually. Open PowerShell (which can be done in `View > Terminal` in Visual Studio) and run the following command:

```PS1
# Create an alias to the 32-bit version of MSBuild named msbuild
# The default installation path of VS2022 is C:\Program Files\Microsoft Visual Studio\2022\Community
Set-Alias msbuild "<path_vs2022>\MSBuild\Current\Bin\MSBuild.exe"
```

To debug X360RayTracing, you need to build and run the debugger from Visual Studio 2010.

To build X360RayTracing in release mode, deploy it to the console and launch it there, start a release build, either through the Visual Studio interface or with the following command:

```PS1
msbuild /p:Configuration=Release
```

To build X360RayTracing and run it in Xenia, start a build with the `Xenia` configuration, either through the Visual Studio interface or with the following command (Xenia will be downloaded automatically the first time you run the command):

```PS1
msbuild /p:Configuration=Xenia
```
