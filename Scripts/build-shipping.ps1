<#
    Compiles a Shipping build of the game (no editor, no dev-only console commands).
#>

param(
    [string]$EngineRoot = "C:\Program Files\Epic Games\UE_5.4",
    [string]$ProjectPath = "$PSScriptRoot\..\WarMachineZeroHour.uproject"
)

$ubt = Join-Path $EngineRoot "Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"

if (-not (Test-Path $ProjectPath)) {
    Write-Host "[ERROR] Project file not found at $ProjectPath" -ForegroundColor Red
    exit 1
}
if (-not (Test-Path $ubt)) {
    Write-Host "[ERROR] UnrealBuildTool.exe not found at $ubt. Check -EngineRoot." -ForegroundColor Red
    exit 1
}

Write-Host "=== Compiling Shipping build ===" -ForegroundColor Cyan
& $ubt WarMachine Win64 Shipping -Project="$ProjectPath" -WaitMutex -FromMsBuild

if ($LASTEXITCODE -ne 0) {
    Write-Host "[ERROR] Shipping build failed with exit code $LASTEXITCODE." -ForegroundColor Red
    exit $LASTEXITCODE
}

Write-Host "[OK] Shipping build succeeded." -ForegroundColor Green
