<#
    WAR MACHINE: ZERO HOUR - Windows environment setup / validation script.
    Checks that the required toolchain is present and reports what is missing.
    Run this FIRST, from a normal (non-admin) PowerShell prompt, from the project root.
#>

param(
    [string]$EngineVersion = "5.4"
)

Write-Host "=== WAR MACHINE: ZERO HOUR - Environment Check ===" -ForegroundColor Cyan

function Test-CommandExists {
    param([string]$Name)
    return [bool](Get-Command $Name -ErrorAction SilentlyContinue)
}

$AllGood = $true

# 1. Git
if (Test-CommandExists "git") {
    Write-Host "[OK] Git found: $(git --version)" -ForegroundColor Green
} else {
    Write-Host "[MISSING] Git is not on PATH. Install from https://git-scm.com/download/win" -ForegroundColor Red
    $AllGood = $false
}

# 2. Visual Studio 2022 (via vswhere)
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (Test-Path $vswhere) {
    $vsInfo = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Workload.NativeDesktop -property installationVersion
    if ($vsInfo) {
        Write-Host "[OK] Visual Studio with C++ workload found: $vsInfo" -ForegroundColor Green
    } else {
        Write-Host "[MISSING] Visual Studio 2022 with 'Desktop development with C++' workload not detected." -ForegroundColor Red
        Write-Host "          Also enable the 'Game Development with C++' workload and the Windows 10/11 SDK component." -ForegroundColor Yellow
        $AllGood = $false
    }
} else {
    Write-Host "[MISSING] Visual Studio Installer not found. Install VS2022 from https://visualstudio.microsoft.com/" -ForegroundColor Red
    $AllGood = $false
}

# 3. Unreal Engine installation (via registry / typical Epic Games path)
$UePathCandidates = @(
    "C:\Program Files\Epic Games\UE_$EngineVersion",
    "C:\Program Files\Epic Games\UE_5",
    "D:\Epic Games\UE_$EngineVersion"
)
$UeFound = $false
foreach ($p in $UePathCandidates) {
    if (Test-Path $p) {
        Write-Host "[OK] Unreal Engine found at: $p" -ForegroundColor Green
        $UeFound = $true
        break
    }
}
if (-not $UeFound) {
    Write-Host "[MISSING] Unreal Engine $EngineVersion not found in the usual Epic Games install locations." -ForegroundColor Red
    Write-Host "          Install it via the Epic Games Launcher -> Unreal Engine tab -> Library -> Install Engine." -ForegroundColor Yellow
    $AllGood = $false
}

Write-Host ""
if ($AllGood) {
    Write-Host "All required tools were detected. You can proceed with build-development.ps1" -ForegroundColor Green
} else {
    Write-Host "One or more required tools are missing. Install them, then re-run this script." -ForegroundColor Yellow
}
