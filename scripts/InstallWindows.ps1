$installPath = "$env:LOCALAPPDATA\Programs\iona"

Write-Host "Installing iona bundle"

if (Test-Path -Path $installPath)
{
	Write-Host "Directory $installPath already exists. Deleting it to ensure a clean install"

	Remove-Item -Recurse -Force $installPath
}

New-Item -ItemType Directory -Path $installPath

Write-Host "Directory $installPath created"

Write-Host "Downloading iona_bundle_windows.zip"

Invoke-WebRequest -Uri "https://github.com/zixoan/iona/releases/download/0.0.0/iona_bundle_windows.zip" -OutFile "$installPath/iona_bundle_windows.zip"

Write-Host "Download finished"

Write-Host "Unzipping iona_bundle_windows.zip"

Add-Type -assembly "System.IO.Compression.Filesystem"
[IO.Compression.Zipfile]::ExtractToDirectory("$installPath/iona_bundle_windows.zip", $installPath);

Remove-Item -Force -Path "$installPath/iona_bundle_windows.zip"

$oldPathVariable = [Environment]::GetEnvironmentVariable("Path", [EnvironmentVariableTarget]::User)
if (!$oldPathVariable.Contains("$installPath"))
{
    echo "Updating PATH variable"
    $newPathVariable = "$oldPathVariable;$installPath"
    
    [Environment]::SetEnvironmentVariable(
        "Path",
        $newPathVariable,
        [EnvironmentVariableTarget]::User)
}
else
{
    echo "PATH variable already updated"
}

Write-Host "Setting IONA_PATH environment variable"

[Environment]::SetEnvironmentVariable(
        "IONA_PATH",
        $installPath,
        [EnvironmentVariableTarget]::User)

$env:Path += ";$installPath"

Write-Host "Finished installing windows iona bundle"