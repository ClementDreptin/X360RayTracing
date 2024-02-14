# X360RayTracing

Ray tracing experiment on the Xbox 360.

![Ray traced spheres](./resources/screenshots/spheres.jpg)

## Technical notes

This project is an Xbox 360 port of [TheCherno's RayTracing renderer](https://github.com/TheCherno/RayTracing) built entirely on YouTube in [a fantastic series](https://www.youtube.com/playlist?list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl).

All the calculations are done on the CPU because first, that's how TheCherno builds it in his series, and second, because the Xbox 360 only has support for DirectX9, which doesn't have a ray tracing pipeline.

Each frame, a giant array of pixels is calculated on the CPU and written to a texture that fills up the whole screen. The texture size had to be fairly small (640x360) so that the hardware could handle it. The small size of the texture explains the pixelated affect around the spheres. A CPU implementation is far from optimal (hence the frame rate...), so I started a GPU implementation in the [`gpu` branch](https://github.com/ClementDreptin/X360RayTracing/tree/gpu) which, so far, seems to run a lot faster than the CPU implementation.

## Building

### Requirements

-   Having the Xbox 360 Software Development Kit (XDK) installed.

### Visual Studio 2019

Open `X360RayTracing.sln` in Visual Studio and build it.

### Visual Studio 2022

You can't build with the 64-bit version of MSBuild so you'll need to run the 32-bit version manually. Open a developer PowerShell in Visual Studio (`View > Terminal`) and run the following command:

```PS1
# Create an alias to the 32-bit version of MSBuild named msbuild
Set-Alias msbuild <path_vs2022>\MSBuild\Current\Bin\MSBuild.exe
```

To build X360RayTracing, deploy it to the console and launch it there, start a build with the `Release` configuration, either through the Visual Studio interface or with the following command:

```PS1
# Equivalent to msbuild /p:Configuration=Release
msbuild
```

To build X360RayTracing and run it in Xenia, start a build with the `Xenia` configuration, either through the Visual Studio interface or with the following command (Xenia will be downloaded automatically the first time you run the command):

```PS1
msbuild /p:Configuration=Xenia
```
