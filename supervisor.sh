#!/bin/bash

# Library Management System - Process Supervisor
# This script keeps both the API server and frontend server running

# Configuration
SERVER_EXECUTABLE="./build/http_api_server"
FRONTEND_DIR="./frontend"
LOG_DIR="./logs"
LOG_FILE="$LOG_DIR/api_server.log"
ERROR_LOG="$LOG_DIR/api_server_error.log"
FRONTEND_LOG="$LOG_DIR/frontend_server.log"
RESTART_LOG="$LOG_DIR/restart_history.log"
MAX_RESTARTS=10
RESTART_WINDOW=60  # seconds
BACKEND_PORT=8080
FRONTEND_PORT=8081

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Create log directory if it doesn't exist
mkdir -p "$LOG_DIR"

# Initialize restart counter
restart_count=0
window_start_time=$(date +%s)

echo -e "${BLUE}======================================${NC}"
echo -e "${BLUE}Library Management System Supervisor${NC}"
echo -e "${BLUE}Backend + Frontend Server Manager${NC}"
echo -e "${BLUE}======================================${NC}"
echo ""

# Function to check if backend server is running
is_server_running() {
    if [ -f /tmp/api_server.pid ]; then
        pid=$(cat /tmp/api_server.pid)
        if ps -p $pid > /dev/null 2>&1; then
            return 0
        fi
    fi
    return 1
}

# Function to check if frontend server is running
is_frontend_running() {
    if [ -f /tmp/frontend_server.pid ]; then
        pid=$(cat /tmp/frontend_server.pid)
        if ps -p $pid > /dev/null 2>&1; then
            return 0
        fi
    fi
    return 1
}

# Function to kill existing server instances
kill_existing_servers() {
    echo -e "${YELLOW}Checking for existing server instances...${NC}"
    
    # Kill backend by PID file
    if [ -f /tmp/api_server.pid ]; then
        old_pid=$(cat /tmp/api_server.pid)
        if ps -p $old_pid > /dev/null 2>&1; then
            echo -e "${YELLOW}Killing existing backend server (PID: $old_pid)${NC}"
            kill $old_pid 2>/dev/null || kill -9 $old_pid 2>/dev/null
            sleep 1
        fi
        rm /tmp/api_server.pid
    fi
    
    # Kill frontend by PID file
    if [ -f /tmp/frontend_server.pid ]; then
        old_pid=$(cat /tmp/frontend_server.pid)
        if ps -p $old_pid > /dev/null 2>&1; then
            echo -e "${YELLOW}Killing existing frontend server (PID: $old_pid)${NC}"
            kill $old_pid 2>/dev/null || kill -9 $old_pid 2>/dev/null
            sleep 1
        fi
        rm /tmp/frontend_server.pid
    fi
    
    # Kill by backend port
    port_pid=$(lsof -ti:$BACKEND_PORT 2>/dev/null)
    if [ ! -z "$port_pid" ]; then
        echo -e "${YELLOW}Killing process using port $BACKEND_PORT (PID: $port_pid)${NC}"
        kill $port_pid 2>/dev/null || kill -9 $port_pid 2>/dev/null
        sleep 1
    fi
    
    # Kill by frontend port
    port_pid=$(lsof -ti:$FRONTEND_PORT 2>/dev/null)
    if [ ! -z "$port_pid" ]; then
        echo -e "${YELLOW}Killing process using port $FRONTEND_PORT (PID: $port_pid)${NC}"
        kill $port_pid 2>/dev/null || kill -9 $port_pid 2>/dev/null
        sleep 1
    fi
    
    # Kill by executable name
    pkill -f "http_api_server" 2>/dev/null
    pkill -f "SimpleHTTPServer" 2>/dev/null
    pkill -f "http.server" 2>/dev/null
    sleep 1
}

# Function to start the backend server
start_server() {
    echo -e "${GREEN}Starting Backend API server...${NC}"
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] Starting backend server" >> "$LOG_FILE"
    
    # Start server in background and capture its PID
    $SERVER_EXECUTABLE >> "$LOG_FILE" 2>> "$ERROR_LOG" &
    server_pid=$!
    
    # Save PID to file
    echo $server_pid > /tmp/api_server.pid
    
    # Wait a moment and check if it started successfully
    sleep 2
    
    if ps -p $server_pid > /dev/null 2>&1; then
        echo -e "${GREEN}✓ Backend server started successfully (PID: $server_pid)${NC}"
        echo -e "${GREEN}✓ Listening on port $BACKEND_PORT${NC}"
        echo "[$(date '+%Y-%m-%d %H:%M:%S')] Backend server started with PID: $server_pid" >> "$LOG_FILE"
        return 0
    else
        echo -e "${RED}✗ Backend server failed to start${NC}"
        echo "[$(date '+%Y-%m-%d %H:%M:%S')] Backend server failed to start" >> "$ERROR_LOG"
        return 1
    fi
}

# Function to start the frontend server
start_frontend() {
    echo -e "${GREEN}Starting Frontend server...${NC}"
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] Starting frontend server" >> "$FRONTEND_LOG"
    
    # Start Python HTTP server in the frontend directory
    cd "$FRONTEND_DIR"
    python3 -m http.server $FRONTEND_PORT >> "../$FRONTEND_LOG" 2>&1 &
    frontend_pid=$!
    cd ..
    
    # Save PID to file
    echo $frontend_pid > /tmp/frontend_server.pid
    
    # Wait a moment and check if it started successfully
    sleep 2
    
    if ps -p $frontend_pid > /dev/null 2>&1; then
        echo -e "${GREEN}✓ Frontend server started successfully (PID: $frontend_pid)${NC}"
        echo -e "${GREEN}✓ Listening on port $FRONTEND_PORT${NC}"
        echo "[$(date '+%Y-%m-%d %H:%M:%S')] Frontend server started with PID: $frontend_pid" >> "$FRONTEND_LOG"
        return 0
    else
        echo -e "${RED}✗ Frontend server failed to start${NC}"
        echo "[$(date '+%Y-%m-%d %H:%M:%S')] Frontend server failed to start" >> "$FRONTEND_LOG"
        return 1
    fi
}

# Function to check restart limits
check_restart_limit() {
    current_time=$(date +%s)
    time_diff=$((current_time - window_start_time))
    
    if [ $time_diff -gt $RESTART_WINDOW ]; then
        # Reset counter if we're outside the time window
        restart_count=0
        window_start_time=$current_time
    fi
    
    if [ $restart_count -ge $MAX_RESTARTS ]; then
        echo -e "${RED}✗ Maximum restart limit reached ($MAX_RESTARTS restarts in $RESTART_WINDOW seconds)${NC}"
        echo -e "${RED}✗ Server appears to be in a crash loop. Please check logs:${NC}"
        echo -e "${RED}  - $LOG_FILE${NC}"
        echo -e "${RED}  - $ERROR_LOG${NC}"
        echo "[$(date '+%Y-%m-%d %H:%M:%S')] Supervisor stopped - restart limit exceeded" >> "$RESTART_LOG"
        return 1
    fi
    
    return 0
}

# Function to monitor and restart
monitor_server() {
    while true; do
        backend_running=false
        frontend_running=false
        
        # Check backend
        if is_server_running; then
            backend_running=true
        else
            echo -e "${RED}✗ Backend server stopped or crashed!${NC}"
            echo "[$(date '+%Y-%m-%d %H:%M:%S')] Backend server crash detected" >> "$RESTART_LOG"
            
            if check_restart_limit; then
                restart_count=$((restart_count + 1))
                echo -e "${YELLOW}⟳ Restarting backend server (attempt $restart_count/$MAX_RESTARTS)...${NC}"
                rm -f /tmp/api_server.pid
                sleep 2
                start_server
            else
                exit 1
            fi
        fi
        
        # Check frontend
        if is_frontend_running; then
            frontend_running=true
        else
            echo -e "${RED}✗ Frontend server stopped or crashed!${NC}"
            echo "[$(date '+%Y-%m-%d %H:%M:%S')] Frontend server crash detected" >> "$RESTART_LOG"
            
            if check_restart_limit; then
                restart_count=$((restart_count + 1))
                echo -e "${YELLOW}⟳ Restarting frontend server (attempt $restart_count/$MAX_RESTARTS)...${NC}"
                rm -f /tmp/frontend_server.pid
                sleep 2
                start_frontend
            else
                exit 1
            fi
        fi
        
        # Sleep before next check
        sleep 5
    done
}

# Cleanup function
cleanup() {
    echo ""
    echo -e "${YELLOW}Shutting down supervisor...${NC}"
    
    # Stop backend server
    if [ -f /tmp/api_server.pid ]; then
        pid=$(cat /tmp/api_server.pid)
        if ps -p $pid > /dev/null 2>&1; then
            echo -e "${YELLOW}Stopping backend server (PID: $pid)${NC}"
            kill $pid 2>/dev/null
            sleep 2
            
            # Force kill if still running
            if ps -p $pid > /dev/null 2>&1; then
                kill -9 $pid 2>/dev/null
            fi
        fi
        rm /tmp/api_server.pid
    fi
    
    # Stop frontend server
    if [ -f /tmp/frontend_server.pid ]; then
        pid=$(cat /tmp/frontend_server.pid)
        if ps -p $pid > /dev/null 2>&1; then
            echo -e "${YELLOW}Stopping frontend server (PID: $pid)${NC}"
            kill $pid 2>/dev/null
            sleep 2
            
            # Force kill if still running
            if ps -p $pid > /dev/null 2>&1; then
                kill -9 $pid 2>/dev/null
            fi
        fi
        rm /tmp/frontend_server.pid
    fi
    
    echo -e "${GREEN}✓ All servers stopped${NC}"
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] Supervisor stopped by user" >> "$LOG_FILE"
    exit 0
}

# Set up signal handlers
trap cleanup SIGINT SIGTERM

# Check if executable exists
if [ ! -f "$SERVER_EXECUTABLE" ]; then
    echo -e "${RED}✗ Server executable not found: $SERVER_EXECUTABLE${NC}"
    echo -e "${YELLOW}Please build the project first:${NC}"
    echo -e "  make"
    exit 1
fi

# Make executable if not already
chmod +x "$SERVER_EXECUTABLE"

# Kill any existing instances
kill_existing_servers

# Start the backend server
if ! start_server; then
    echo -e "${RED}✗ Failed to start backend server on first attempt${NC}"
    exit 1
fi

# Start the frontend server
if ! start_frontend; then
    echo -e "${RED}✗ Failed to start frontend server on first attempt${NC}"
    exit 1
fi

echo ""
echo -e "${BLUE}======================================${NC}"
echo -e "${GREEN}✓ Supervisor is now monitoring both servers${NC}"
echo -e "${BLUE}======================================${NC}"
echo ""
echo -e "${YELLOW}Services:${NC}"
echo -e "  Backend API:  http://localhost:$BACKEND_PORT"
echo -e "  Frontend App: http://localhost:$FRONTEND_PORT"
echo ""
echo -e "${YELLOW}Logs:${NC}"
echo -e "  Backend log:  $LOG_FILE"
echo -e "  Error log:    $ERROR_LOG"
echo -e "  Frontend log: $FRONTEND_LOG"
echo -e "  Restart log:  $RESTART_LOG"
echo ""
echo -e "${YELLOW}Press Ctrl+C to stop the supervisor and all servers${NC}"
echo ""

# Start monitoring
monitor_server
