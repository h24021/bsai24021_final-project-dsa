@echo off
REM Library Management System API - Windows Process Supervisor
REM This script keeps the API server running and restarts it if it crashes

setlocal EnableDelayedExpansion

REM Configuration
set "SERVER_EXECUTABLE=build\http_api_server.exe"
set "LOG_DIR=logs"
set "LOG_FILE=%LOG_DIR%\api_server.log"
set "ERROR_LOG=%LOG_DIR%\api_server_error.log"
set "RESTART_LOG=%LOG_DIR%\restart_history.log"
set "MAX_RESTARTS=10"
set "RESTART_WINDOW=60"
set "PORT=8080"
set "restart_count=0"

REM Create log directory
if not exist "%LOG_DIR%" mkdir "%LOG_DIR%"

echo ======================================
echo Library API Server Supervisor
echo ======================================
echo.

REM Check if executable exists
if not exist "%SERVER_EXECUTABLE%" (
    echo [ERROR] Server executable not found: %SERVER_EXECUTABLE%
    echo Please build the project first using: make
    pause
    exit /b 1
)

REM Kill any existing instances
echo Checking for existing server instances...
taskkill /F /IM http_api_server.exe >nul 2>&1
timeout /t 2 /nobreak >nul

:start_server
echo [%date% %time%] Starting API server...
echo [%date% %time%] Starting server >> "%LOG_FILE%"

REM Start server in background
start /B "" "%SERVER_EXECUTABLE%" >> "%LOG_FILE%" 2>> "%ERROR_LOG%"

REM Wait for server to start
timeout /t 3 /nobreak >nul

REM Check if server is running
tasklist /FI "IMAGENAME eq http_api_server.exe" 2>NUL | find /I /N "http_api_server.exe">NUL
if "%ERRORLEVEL%"=="0" (
    echo [SUCCESS] Server started successfully
    echo [SUCCESS] Listening on port %PORT%
    echo [%date% %time%] Server started successfully >> "%LOG_FILE%"
) else (
    echo [ERROR] Server failed to start
    echo [%date% %time%] Server failed to start >> "%ERROR_LOG%"
    goto error_exit
)

echo.
echo ======================================
echo Supervisor is now monitoring the server
echo ======================================
echo.
echo Logs:
echo   Server log: %LOG_FILE%
echo   Error log:  %ERROR_LOG%
echo   Restart log: %RESTART_LOG%
echo.
echo Press Ctrl+C to stop the supervisor and server
echo.

:monitor_loop
REM Check if server is still running
tasklist /FI "IMAGENAME eq http_api_server.exe" 2>NUL | find /I /N "http_api_server.exe">NUL
if "%ERRORLEVEL%"=="0" (
    REM Server is running, wait and check again
    timeout /t 5 /nobreak >nul
    goto monitor_loop
) else (
    REM Server crashed
    echo [ERROR] Server stopped or crashed!
    echo [%date% %time%] Server crash detected >> "%RESTART_LOG%"
    
    REM Increment restart counter
    set /a restart_count+=1
    
    REM Check restart limit
    if !restart_count! geq %MAX_RESTARTS% (
        echo [ERROR] Maximum restart limit reached ^(%MAX_RESTARTS% restarts^)
        echo [ERROR] Server appears to be in a crash loop. Please check logs:
        echo   - %LOG_FILE%
        echo   - %ERROR_LOG%
        echo [%date% %time%] Supervisor stopped - restart limit exceeded >> "%RESTART_LOG%"
        goto error_exit
    )
    
    echo [RESTART] Restarting server ^(attempt !restart_count!/%MAX_RESTARTS%^)...
    echo [%date% %time%] Restart attempt !restart_count! >> "%RESTART_LOG%"
    
    REM Wait before restarting
    timeout /t 2 /nobreak >nul
    
    REM Restart server
    goto start_server
)

:error_exit
echo.
echo [ERROR] Supervisor terminated with errors
pause
exit /b 1

:cleanup
echo.
echo Shutting down supervisor...
taskkill /F /IM http_api_server.exe >nul 2>&1
echo [SUCCESS] Supervisor stopped
echo [%date% %time%] Supervisor stopped by user >> "%LOG_FILE%"
exit /b 0
