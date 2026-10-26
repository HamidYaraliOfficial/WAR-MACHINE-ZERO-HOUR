<#
    Packages a Shipping build of the game for Windows using the engine's automation tool (RunUAT).
#>

param(
    [string]$EngineRoot = "C:\Program Files\Epic Games\UE_5.4",
    [string]$ProjectPath = "$PSScriptRoot\..\WarMachineZeroHour.uproject",
    [string]$OutputDir = "$PSScriptRoot\..\Packaged"
)

$runUAT = Join-Path $EngineRoot "Engine\Build\BatchFiles\RunUAT.bat"

if (-not (Test-Path $runUAT)) {
    Write-Host "[ERROR] RunUAT.bat not found at $runUAT. Check -EngineRoot." -ForegroundColor Red
    exit 1
}

Write-Host "=== Packaging Shipping build for Win64 ===" -ForegroundColor Cyan

& $runUAT BuildCookRun `
    -project="$ProjectPath" `
    -noP4 -platform=Win64 -clientconfig=Shipping -serverconfig=Shipping `
    -cook -allmaps -build -stage -pak -archive `
    -archivedirectory="$OutputDir"

if ($LASTEXITCODE -ne 0) {
    Write-Host "[ERROR] Packaging failed with exit code $LASTEXITCODE." -ForegroundColor Red
    exit $LASTEXITCODE
}

Write-Host "[OK] Package created at $OutputDir" -ForegroundColor Green
