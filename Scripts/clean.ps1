<#
    Removes generated build artifacts (Binaries, Intermediate, Saved, DerivedDataCache, .vs)
    so the next build.ps1 run starts from a clean slate. Source and Content are never touched.
#>

param(
    [string]$ProjectRoot = "$PSScriptRoot\.."
)

$FoldersToRemove = @("Binaries", "Intermediate", "Saved", "DerivedDataCache", ".vs")

foreach ($Folder in $FoldersToRemove) {
    $FullPath = Join-Path $ProjectRoot $Folder
    if (Test-Path $FullPath) {
        Write-Host "Removing $FullPath ..." -ForegroundColor Yellow
        Remove-Item -Recurse -Force $FullPath
    }
}

Write-Host "[OK] Clean complete." -ForegroundColor Green
