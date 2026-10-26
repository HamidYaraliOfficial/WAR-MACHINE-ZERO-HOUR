<#
    Generates project files and compiles a Development build via Unreal Build Tool.
#>

param(
    [string]$EngineRoot = "C:\Program Files\Epic Games\UE_5.4",
    [string]$ProjectPath = "$PSScriptRoot\..\WarMachineZeroHour.uproject"
)

$ubt = Join-Path $EngineRoot "Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
$genProjFiles = Join-Path $EngineRoot "Engine\Build\BatchFiles\GenerateProjectFiles.bat"

if (-not (Test-Path $ProjectPath)) {
    Write-Host "[ERROR] Project file not found at $ProjectPath" -ForegroundColor Red
    exit 1
}

Write-Host "=== Generating Visual Studio project files ===" -ForegroundColor Cyan
if (Test-Path $genProjFiles) {
    & $genProjFiles $ProjectPath
} else {
    Write-Host "[WARN] GenerateProjectFiles.bat not found at expected engine path; skipping and trying UBT directly." -ForegroundColor Yellow
}

Write-Host "=== Compiling Development Editor build ===" -ForegroundColor Cyan
if (-not (Test-Path $ubt)) {
    Write-Host "[ERROR] UnrealBuildTool.exe not found at $ubt. Check -EngineRoot." -ForegroundColor Red
    exit 1
}

& $ubt WarMachineEditor Win64 Development -Project="$ProjectPath" -WaitMutex -FromMsBuild

if ($LASTEXITCODE -ne 0) {
    Write-Host "[ERROR] Build failed with exit code $LASTEXITCODE. Check the log above for the first error (often a missing include or a Build.cs dependency)." -ForegroundColor Red
    exit $LASTEXITCODE
}

Write-Host "[OK] Development build succeeded." -ForegroundColor Green
