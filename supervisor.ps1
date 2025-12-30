# Library Management System API - PowerShell Supervisor
# This script keeps the API server running and restarts it if it crashes
# More robust than batch script with better process management

param(
    [int]$MaxRestarts = 10,
    [int]$RestartWindow = 60,
    [int]$Port = 8080,
    [string]$ServerExecutable = ".\build\http_api_server.exe"
)

# Configuration
$LogDir = ".\logs"
$LogFile = Join-Path $LogDir "api_server.log"
$ErrorLog = Join-Path $LogDir "api_server_error.log"
$RestartLog = Join-Path $LogDir "restart_history.log"

# Create log directory
if (-not (Test-Path $LogDir)) {
    New-Item -ItemType Directory -Path $LogDir | Out-Null
}

# Colors
$ColorInfo = "Cyan"
$ColorSuccess = "Green"
$ColorWarning = "Yellow"
$ColorError = "Red"

# Variables
$RestartCount = 0
$WindowStartTime = Get-Date
$ServerProcess = $null

# Functions
function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogMessage = "[$Timestamp] [$Level] $Message"
    Add-Content -Path $LogFile -Value $LogMessage
}

function Write-ColorOutput {
    param([string]$Message, [string]$Color = "White")
    Write-Host $Message -ForegroundColor $Color
}

function Kill-ExistingServers {
    Write-ColorOutput "Checking for existing server instances..." $ColorWarning
    
    # Kill by process name
    $existingProcesses = Get-Process -Name "http_api_server" -ErrorAction SilentlyContinue
    if ($existingProcesses) {
        foreach ($proc in $existingProcesses) {
            Write-ColorOutput "Killing existing server (PID: $($proc.Id))" $ColorWarning
            Stop-Process -Id $proc.Id -Force -ErrorAction SilentlyContinue
        }
        Start-Sleep -Seconds 1
    }
    
    # Kill by port
    $portProcesses = Get-NetTCPConnection -LocalPort $Port -ErrorAction SilentlyContinue | Select-Object -ExpandProperty OwningProcess -Unique
    if ($portProcesses) {
        foreach ($pid in $portProcesses) {
            Write-ColorOutput "Killing process using port $Port (PID: $pid)" $ColorWarning
            Stop-Process -Id $pid -Force -ErrorAction SilentlyContinue
        }
        Start-Sleep -Seconds 1
    }
}

function Start-Server {
    Write-ColorOutput "Starting API server..." $ColorSuccess
    Write-Log "Starting server"
    
    try {
        # Start process and capture output
        $psi = New-Object System.Diagnostics.ProcessStartInfo
        $psi.FileName = $ServerExecutable
        $psi.UseShellExecute = $false
        $psi.RedirectStandardOutput = $true
        $psi.RedirectStandardError = $true
        $psi.CreateNoWindow = $false
        
        $script:ServerProcess = [System.Diagnostics.Process]::Start($psi)
        
        # Asynchronously read output
        $outputHandler = {
            param($sender, $e)
            if ($e.Data) {
                Add-Content -Path $using:LogFile -Value $e.Data
            }
        }
        
        $errorHandler = {
            param($sender, $e)
            if ($e.Data) {
                Add-Content -Path $using:ErrorLog -Value $e.Data
            }
        }
        
        Register-ObjectEvent -InputObject $ServerProcess -EventName OutputDataReceived -Action $outputHandler | Out-Null
        Register-ObjectEvent -InputObject $ServerProcess -EventName ErrorDataReceived -Action $errorHandler | Out-Null
        
        $ServerProcess.BeginOutputReadLine()
        $ServerProcess.BeginErrorReadLine()
        
        # Wait and check if started successfully
        Start-Sleep -Seconds 2
        
        if (-not $ServerProcess.HasExited) {
            Write-ColorOutput "✓ Server started successfully (PID: $($ServerProcess.Id))" $ColorSuccess
            Write-ColorOutput "✓ Listening on port $Port" $ColorSuccess
            Write-Log "Server started with PID: $($ServerProcess.Id)"
            return $true
        } else {
            Write-ColorOutput "✗ Server failed to start" $ColorError
            Write-Log "Server failed to start" "ERROR"
            return $false
        }
    } catch {
        Write-ColorOutput "✗ Error starting server: $_" $ColorError
        Write-Log "Error starting server: $_" "ERROR"
        return $false
    }
}

function Test-ServerRunning {
    if ($null -eq $script:ServerProcess) {
        return $false
    }
    
    try {
        if ($ServerProcess.HasExited) {
            return $false
        }
        return $true
    } catch {
        return $false
    }
}

function Test-RestartLimit {
    $currentTime = Get-Date
    $timeDiff = ($currentTime - $script:WindowStartTime).TotalSeconds
    
    if ($timeDiff -gt $RestartWindow) {
        # Reset counter if outside time window
        $script:RestartCount = 0
        $script:WindowStartTime = $currentTime
    }
    
    if ($RestartCount -ge $MaxRestarts) {
        Write-ColorOutput "✗ Maximum restart limit reached ($MaxRestarts restarts in $RestartWindow seconds)" $ColorError
        Write-ColorOutput "✗ Server appears to be in a crash loop. Please check logs:" $ColorError
        Write-ColorOutput "  - $LogFile" $ColorError
        Write-ColorOutput "  - $ErrorLog" $ColorError
        Write-Log "Supervisor stopped - restart limit exceeded" "ERROR"
        Add-Content -Path $RestartLog -Value "$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss') Supervisor stopped - restart limit exceeded"
        return $false
    }
    
    return $true
}

function Monitor-Server {
    while ($true) {
        if (Test-ServerRunning) {
            # Server is running
            Start-Sleep -Seconds 5
        } else {
            # Server crashed
            Write-ColorOutput "`n✗ Server stopped or crashed!" $ColorError
            Add-Content -Path $RestartLog -Value "$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss') Server crash detected"
            
            # Check restart limit
            if (-not (Test-RestartLimit)) {
                return $false
            }
            
            # Increment counter
            $script:RestartCount++
            
            Write-ColorOutput "⟳ Restarting server (attempt $RestartCount/$MaxRestarts)..." $ColorWarning
            Add-Content -Path $RestartLog -Value "$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss') Restart attempt $RestartCount"
            
            # Wait before restart
            Start-Sleep -Seconds 2
            
            # Restart server
            if (Start-Server) {
                Write-ColorOutput "✓ Server restarted successfully`n" $ColorSuccess
                Add-Content -Path $RestartLog -Value "$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss') Server restarted successfully"
            } else {
                Write-ColorOutput "✗ Failed to restart server`n" $ColorError
            }
        }
    }
}

function Stop-Supervisor {
    Write-ColorOutput "`nShutting down supervisor..." $ColorWarning
    
    if ($null -ne $script:ServerProcess -and -not $ServerProcess.HasExited) {
        Write-ColorOutput "Stopping server (PID: $($ServerProcess.Id))" $ColorWarning
        Stop-Process -Id $ServerProcess.Id -Force -ErrorAction SilentlyContinue
        Start-Sleep -Seconds 2
    }
    
    # Clean up any remaining instances
    Get-Process -Name "http_api_server" -ErrorAction SilentlyContinue | Stop-Process -Force -ErrorAction SilentlyContinue
    
    Write-ColorOutput "✓ Supervisor stopped" $ColorSuccess
    Write-Log "Supervisor stopped by user"
    exit 0
}

# Main execution
Write-ColorOutput "======================================" $ColorInfo
Write-ColorOutput "Library API Server Supervisor" $ColorInfo
Write-ColorOutput "======================================" $ColorInfo
Write-Host ""

# Check if executable exists
if (-not (Test-Path $ServerExecutable)) {
    Write-ColorOutput "✗ Server executable not found: $ServerExecutable" $ColorError
    Write-ColorOutput "Please build the project first using: make" $ColorWarning
    exit 1
}

# Register cleanup handler
Register-EngineEvent -SourceIdentifier PowerShell.Exiting -Action { Stop-Supervisor } | Out-Null
$null = Register-ObjectEvent -InputObject ([Console]) -EventName CancelKeyPress -Action {
    [Console]::TreatControlCAsInput = $false
    Stop-Supervisor
}

# Kill existing instances
Kill-ExistingServers

# Start server
if (-not (Start-Server)) {
    Write-ColorOutput "✗ Failed to start server on first attempt" $ColorError
    exit 1
}

Write-Host ""
Write-ColorOutput "======================================" $ColorInfo
Write-ColorOutput "✓ Supervisor is now monitoring the server" $ColorSuccess
Write-ColorOutput "======================================" $ColorInfo
Write-Host ""
Write-ColorOutput "Logs:" $ColorWarning
Write-Host "  Server log: $LogFile"
Write-Host "  Error log:  $ErrorLog"
Write-Host "  Restart log: $RestartLog"
Write-Host ""
Write-ColorOutput "Press Ctrl+C to stop the supervisor and server" $ColorWarning
Write-Host ""

# Start monitoring
try {
    Monitor-Server
} finally {
    Stop-Supervisor
}
