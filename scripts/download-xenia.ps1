$XeniaDir = "$PSScriptRoot\..\tools\xenia"
$XeniaExePath = "$XeniaDir\xenia.exe"
$XeniaZipPath = "$XeniaDir\xenia.zip"
$XeniaDownloadUri = "https://github.com/xenia-project/release-builds-windows/releases/latest/download/xenia_master.zip"

# Exit early if the file already exists
if ([System.IO.File]::Exists($XeniaExePath)) {
    Exit 0
}

# Download the latest Xenia binary
try {
    $Null = mkdir -Force $XeniaDir
    $WebClient = New-Object System.Net.WebClient
    $WebClient.DownloadFile($XeniaDownloadUri, $XeniaZipPath)
}
catch {
    Write-Host -ForegroundColor Red "Could not download the Xenia binary!"
    Exit 1
}

# Unzip
try {
    Expand-Archive $XeniaZipPath -Destination $XeniaDir
}
catch {
    Write-Host -ForegroundColor Red "Could not unzip $XeniaZipPath"
    Exit 1
}

# Delete the zip file
try {
    Remove-Item $XeniaZipPath
}
catch {
    Write-Host -ForegroundColor Red "Could not delete $XeniaZipPath"
    Exit 1
}
