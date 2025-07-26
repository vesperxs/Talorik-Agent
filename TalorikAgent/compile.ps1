Write-Host "Compiling TalorikAgent Security Agent (PowerShell)..." -ForegroundColor Green

# Try to find Visual Studio installation
$vs2022Path = "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
$vs2019Path = "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

if (Test-Path $vs2022Path) {
    Write-Host "Found Visual Studio 2022" -ForegroundColor Yellow
    $vcvarsPath = $vs2022Path
} elseif (Test-Path $vs2019Path) {
    Write-Host "Found Visual Studio 2019" -ForegroundColor Yellow
    $vcvarsPath = $vs2019Path
} else {
    Write-Host "Visual Studio not found!" -ForegroundColor Red
    Write-Host "Please install Visual Studio Community (free) from:" -ForegroundColor Yellow
    Write-Host "https://visualstudio.microsoft.com/downloads/" -ForegroundColor Cyan
    Read-Host "Press Enter to exit"
    exit 1
}

# Set up the environment
Write-Host "Setting up Visual Studio environment..." -ForegroundColor Yellow
$vcvarsContent = Get-Content $vcvarsPath -Raw
$vcvarsContent = $vcvarsContent -replace 'call ', ''
$vcvarsContent = $vcvarsContent -replace 'goto :eof', ''

# Execute the vcvars script in a new process
$tempScript = [System.IO.Path]::GetTempFileName() + ".bat"
$vcvarsContent | Out-File -FilePath $tempScript -Encoding ASCII

try {
    # Run the vcvars script and capture environment
    $envVars = cmd /c "`"$tempScript`" && set"
    
    # Parse environment variables
    foreach ($line in $envVars) {
        if ($line -match '^([^=]+)=(.*)$') {
            $name = $matches[1]
            $value = $matches[2]
            [Environment]::SetEnvironmentVariable($name, $value, [EnvironmentVariableTarget]::Process)
        }
    }
    
    Write-Host "Environment set up successfully" -ForegroundColor Green
    
    # Compile the program
    Write-Host "Compiling build_simple.cpp..." -ForegroundColor Yellow
    $compileResult = cmd /c "cl /std:c++17 build_simple.cpp /Fe:TalorikAgent.exe 2>&1"
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Compilation successful!" -ForegroundColor Green
        Write-Host ""
        Write-Host "========================================" -ForegroundColor Cyan
        Write-Host "COMPILATION SUCCESSFUL!" -ForegroundColor Green
        Write-Host "========================================" -ForegroundColor Cyan
        Write-Host ""
        Write-Host "To run the security agent:" -ForegroundColor Yellow
        Write-Host "  .\TalorikAgent.exe" -ForegroundColor White
        Write-Host ""
        Write-Host "The agent will:" -ForegroundColor Yellow
        Write-Host "- Initialize logging" -ForegroundColor White
        Write-Host "- Create test messages and tasks" -ForegroundColor White
        Write-Host "- Show JSON serialization" -ForegroundColor White
        Write-Host "- Run continuously with heartbeat messages" -ForegroundColor White
        Write-Host ""
        
        $run = Read-Host "Press Enter to run the agent now (or 'n' to exit)"
        if ($run -ne 'n') {
            Write-Host ""
            Write-Host "Starting TalorikAgent Security Agent..." -ForegroundColor Green
            Write-Host "(Press Ctrl+C to stop)" -ForegroundColor Yellow
            Write-Host ""
            .\TalorikAgent.exe
        }
    } else {
        Write-Host "Compilation failed!" -ForegroundColor Red
        Write-Host $compileResult -ForegroundColor Red
    }
    
} catch {
    Write-Host "Error setting up environment: $_" -ForegroundColor Red
} finally {
    # Clean up
    if (Test-Path $tempScript) {
        Remove-Item $tempScript -Force
    }
} 