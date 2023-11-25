param([string]$XeniaDir)

$XeniaExePath = "$($XeniaDir)\xenia.exe"
$XeniaZipPath = "$($XeniaDir)\xenia.zip"
$XeniaDownloadUri = "https://github.com/xenia-project/release-builds-windows/releases/latest/download/xenia_master.zip"

if (!$XeniaDir) {
    echo "You need to provide a XeniaDir argument!"
    Exit 1
}

# Exit early if the file already exists
if ([System.IO.File]::Exists($XeniaExePath)) {
    Exit 0
}

# Download the latest Xenia binary
try {
    $null = mkdir -Force $XeniaDir
    $WebClient = New-Object System.Net.WebClient
    $WebClient.DownloadFile($XeniaDownloadUri, $XeniaZipPath)
} catch {
    echo "Could not download the Xenia binary!"
    echo $_.Exception
    Exit 1
}

# Unzip
try {
    Expand-Archive $XeniaZipPath -Destination $XeniaDir
} catch {
    echo "Could not unzip $($XeniaZipPath)"
    echo $_.Exception
    Exit 1
}

# Delete the zip file
try {
    Remove-Item $XeniaZipPath
} catch {
    echo "Could not delete $($XeniaZipPath)"
    echo $_.Exception
    Exit 1
}
