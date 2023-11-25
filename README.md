# X360RayTracing

Ray tracing experiment on the Xbox 360.

## Building

### Requirements

-   Having the Xbox 360 Software Development Kit (XDK) installed.
-   Having a Powershell execution policy that allows to run scripts. Check the [Microsoft documentation](https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_execution_policies) for details.
-   Running `.\scripts\copy-atgframework.ps1` to copy the AtgFramework source code from your XDK installation to `deps\AtgFramework\`.

### Visual Studio 2019

Open `XboxPlayground.sln` in Visual Studio and build it.

### Visual Studio 2022

You can't build with the 64-bit version of MSBuild so you'll need to run the 32-bit version manually. Open a developer powershell in Visual Studio (`View > Terminal`) and run the following command:

```PS1
# Create an alias to the 32-bit version of MSBuild named msbuild
Set-Alias msbuild <path_vs2022>\MSBuild\Current\Bin\MSBuild.exe
```

To build XboxPlayground, deploy it to the console and launch it there, start a build with the `Release` configuration, either through the Visual Studio interface or with the following command:

```PS1
# Equivalent to msbuild /p:Configuration=Release
msbuild
```

To build XboxPlayground and run it in Xenia, start a build with the `Xenia` configuration, either through the Visual Studio interface or with the following command (Xenia will be downloaded automatically the first time you run the command):

```PS1
msbuild /p:Configuration=Xenia
```
