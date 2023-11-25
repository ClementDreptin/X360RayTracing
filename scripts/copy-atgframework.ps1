$SourceDir = "$($env:XEDK)\Source\Samples\Common"
$DestinationDir = "$($PSScriptRoot)\..\deps\AtgFramework"

# Make sure that the Xbox 360 SDK is installed
if (!$env:XEDK) {
    echo "You need to have the Xbox 360 SDK installed!"
    Exit 1
}

# Get the full path of each file included in vcxproj file
$FilePaths = `
    Select-Xml `
    -Path "$($DestinationDir)\AtgFramework.vcxproj" `
    -Namespace @{msbuild = "http://schemas.microsoft.com/developer/msbuild/2003"} `
    -XPath "//msbuild:ClInclude/@Include | //msbuild:ClCompile/@Include" `
    | foreach { "$($SourceDir)\$($_.Node."#text")"}

# Copy the files
try {
    Copy-Item -Path $FilePaths -Destination $DestinationDir
} catch {
    echo "Could not copy $($SourceDir) into $($DestinationDir)!"
    Exit 1
}
