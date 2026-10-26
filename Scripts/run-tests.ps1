<#
    Runs the project's Automation Tests (Unit/Integration/Gameplay/Save-Load/AI/Scenario/Performance/Determinism)
    headlessly through the Unreal Automation Test framework.
#>

param(
    [string]$EngineRoot = "C:\Program Files\Epic Games\UE_5.4",
    [string]$ProjectPath = "$PSScriptRoot\..\WarMachineZeroHour.uproject",
    [string]$TestFilter = "WarMachine"
)

$editorCmd = Join-Path $EngineRoot "Engine\Binaries\Win64\UnrealEditor-Cmd.exe"

if (-not (Test-Path $editorCmd)) {
    Write-Host "[ERROR] UnrealEditor-Cmd.exe not found at $editorCmd. Check -EngineRoot." -ForegroundColor Red
    exit 1
}

Write-Host "=== Running automation tests matching '$TestFilter' ===" -ForegroundColor Cyan

& $editorCmd $ProjectPath `
    -ExecCmds="Automation RunTests $TestFilter; Quit" `
    -unattended -nopause -nosplash -log

if ($LASTEXITCODE -ne 0) {
    Write-Host "[ERROR] Test run reported failures (exit code $LASTEXITCODE). See Saved\Logs for the automation report." -ForegroundColor Red
    exit $LASTEXITCODE
}

Write-Host "[OK] Automation tests completed. See Saved\Logs\ for the full report." -ForegroundColor Green
