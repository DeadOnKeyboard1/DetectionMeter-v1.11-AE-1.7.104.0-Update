param(
    [Parameter(Mandatory = $false)]
    [string]$PackagePath = (Join-Path $PSScriptRoot "..\..\DetectionMeter-v1.11-AE-1.7.104.0.zip")
)

$ErrorActionPreference = "Stop"
$sourceRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$package = (Resolve-Path $PackagePath).Path
$tempRoot = Join-Path $sourceRoot ".codex.tmp"
$tempPackage = Join-Path $tempRoot ([IO.Path]::GetFileName($package))
$docsRoot = Join-Path $tempRoot "Docs\Detection Meter"

if (Test-Path -LiteralPath $tempRoot) {
    Remove-Item -LiteralPath $tempRoot -Recurse -Force
}
New-Item -ItemType Directory -Path $docsRoot -Force | Out-Null
Copy-Item -LiteralPath $package -Destination $tempPackage

foreach ($file in @("LICENSE", "NOTICE.md", "THIRD_PARTY_NOTICES.md")) {
    Copy-Item -LiteralPath (Join-Path $sourceRoot $file) -Destination $docsRoot
}
Copy-Item -LiteralPath (Join-Path $sourceRoot "licenses") -Destination $docsRoot -Recurse

Add-Type -AssemblyName System.IO.Compression.FileSystem
$archive = [IO.Compression.ZipFile]::Open($tempPackage, [IO.Compression.ZipArchiveMode]::Update)
try {
    $prefix = "Docs/Detection Meter/"
    foreach ($entry in @($archive.Entries)) {
        $normalized = $entry.FullName.Replace('\', '/')
        if ($normalized.StartsWith($prefix, [StringComparison]::OrdinalIgnoreCase) -or
            $normalized -like "readmes/DetectionMeter-*-LICENSE.txt") {
            $entry.Delete()
        }
    }
    Get-ChildItem -LiteralPath $docsRoot -File -Recurse | ForEach-Object {
        $relative = $_.FullName.Substring($docsRoot.Length).TrimStart('\').Replace('\', '/')
        [IO.Compression.ZipFileExtensions]::CreateEntryFromFile(
            $archive, $_.FullName, "$prefix$relative",
            [IO.Compression.CompressionLevel]::Optimal) | Out-Null
    }
}
finally {
    $archive.Dispose()
}

$required = @(
    "SKSE/Plugins/MaxsuDetectionMeter.dll",
    "SKSE/Plugins/MaxsuDetectionMeter.pdb",
    "Docs/Detection Meter/LICENSE",
    "Docs/Detection Meter/NOTICE.md",
    "Docs/Detection Meter/THIRD_PARTY_NOTICES.md",
    "Docs/Detection Meter/licenses/DetectionMeter-MIT.txt",
    "Docs/Detection Meter/licenses/CommonLibSSE-MIT.txt",
    "Docs/Detection Meter/licenses/DKUtil-MIT.txt",
    "Docs/Detection Meter/licenses/ImGui-MIT.txt"
)
$check = [IO.Compression.ZipFile]::OpenRead($tempPackage)
try {
    $names = @($check.Entries | ForEach-Object { $_.FullName.Replace('\', '/') })
    foreach ($name in $required) {
        if ($names -notcontains $name) {
            throw "Package validation failed: missing $name"
        }
    }
}
finally {
    $check.Dispose()
}

Copy-Item -LiteralPath $tempPackage -Destination $package -Force
Remove-Item -LiteralPath $tempRoot -Recurse -Force
Write-Host "Updated and validated: $package"
