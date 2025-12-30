#!/bin/bash

# Library Management System API - Process Supervisor
# This script keeps the API server running and restarts it if it crashes

# Configuration
SERVER_EXECUTABLE="./build/http_api_server"
LOG_DIR="./logs"
LOG_FILE="$LOG_DIR/api_server.log"
ERROR_LOG="$LOG_DIR/api_server_error.log"
RESTART_LOG="$LOG_DIR/restart_history.log"
MAX_RESTARTS=10
RESTART_WINDOW=60  # seconds
PORT=8080

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
echo -e "${BLUE}Library API Server Supervisor${NC}"
echo -e "${BLUE}======================================${NC}"
echo ""

# Function to check if server is running
is_server_running() {
    if [ -f /tmp/api_server.pid ]; then
        pid=$(cat /tmp/api_server.pid)
        if ps -p $pid > /dev/null 2>&1; then
            return 0
        fi
    fi
    return 1
}

# Function to kill existing server instances
kill_existing_servers() {
    echo -e "${YELLOW}Checking for existing server instances...${NC}"
    
    # Kill by PID file
    if [ -f /tmp/api_server.pid ]; then
        old_pid=$(cat /tmp/api_server.pid)
        if ps -p $old_pid > /dev/null 2>&1; then
            echo -e "${YELLOW}Killing existing server (PID: $old_pid)${NC}"
            kill $old_pid 2>/dev/null || kill -9 $old_pid 2>/dev/null
            sleep 1
        fi
        rm /tmp/api_server.pid
    fi
    
    # Kill by port
    port_pid=$(lsof -ti:$PORT 2>/dev/null)
    if [ ! -z "$port_pid" ]; then
        echo -e "${YELLOW}Killing process using port $PORT (PID: $port_pid)${NC}"
        kill $port_pid 2>/dev/null || kill -9 $port_pid 2>/dev/null
        sleep 1
    fi
    
    # Kill by executable name
    pkill -f "http_api_server" 2>/dev/null
    sleep 1
}

# Function to start the server
start_server() {
    echo -e "${GREEN}Starting API server...${NC}"
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] Starting server" >> "$LOG_FILE"
    
    # Start server in background and capture its PID
    $SERVER_EXECUTABLE >> "$LOG_FILE" 2>> "$ERROR_LOG" &
    server_pid=$!
    
    # Save PID to file
    echo $server_pid > /tmp/api_server.pid
    
    # Wait a moment and check if it started successfully
    sleep 2
    
    if ps -p $server_pid > /dev/null 2>&1; then
        echo -e "${GREEN}✓ Server started successfully (PID: $server_pid)${NC}"
        echo -e "${GREEN}✓ Listening on port $PORT${NC}"
        echo "[$(date '+%Y-%m-%d %H:%M:%S')] Server started with PID: $server_pid" >> "$LOG_FILE"
        return 0
    else
        echo -e "${RED}✗ Server failed to start${NC}"
        echo "[$(date '+%Y-%m-%d %H:%M:%S')] Server failed to start" >> "$ERROR_LOG"
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
        if is_server_running; then
            # Server is running, check again after delay
            sleep 5
        else
            # Server is not running
            echo -e "${RED}✗ Server stopped or crashed!${NC}"
            echo "[$(date '+%Y-%m-%d %H:%M:%S')] Server crash detected" >> "$RESTART_LOG"
            
            # Check if we should restart
            if ! check_restart_limit; then
                exit 1
            fi
            
            # Increment restart counter
            restart_count=$((restart_count + 1))
            
            echo -e "${YELLOW}⟳ Restarting server (attempt $restart_count/$MAX_RESTARTS)...${NC}"
            echo "[$(date '+%Y-%m-%d %H:%M:%S')] Restart attempt $restart_count" >> "$RESTART_LOG"
            
            # Clean up before restart
            rm -f /tmp/api_server.pid
            
            # Wait a moment before restarting
            sleep 2
            
            # Start the server
            if start_server; then
                echo -e "${GREEN}✓ Server restarted successfully${NC}"
                echo "[$(date '+%Y-%m-%d %H:%M:%S')] Server restarted successfully" >> "$RESTART_LOG"
            else
                echo -e "${RED}✗ Failed to restart server${NC}"
            fi
        fi
    done
}

# Cleanup function
cleanup() {
    echo ""
    echo -e "${YELLOW}Shutting down supervisor...${NC}"
    
    if [ -f /tmp/api_server.pid ]; then
        pid=$(cat /tmp/api_server.pid)
        if ps -p $pid > /dev/null 2>&1; then
            echo -e "${YELLOW}Stopping server (PID: $pid)${NC}"
            kill $pid 2>/dev/null
            sleep 2
            
            # Force kill if still running
            if ps -p $pid > /dev/null 2>&1; then
                kill -9 $pid 2>/dev/null
            fi
        fi
        rm /tmp/api_server.pid
    fi
    
    echo -e "${GREEN}✓ Supervisor stopped${NC}"
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

# Start the server
if ! start_server; then
    echo -e "${RED}✗ Failed to start server on first attempt${NC}"
    exit 1
fi

echo ""
echo -e "${BLUE}======================================${NC}"
echo -e "${GREEN}✓ Supervisor is now monitoring the server${NC}"
echo -e "${BLUE}======================================${NC}"
echo ""
echo -e "${YELLOW}Logs:${NC}"
echo -e "  Server log: $LOG_FILE"
echo -e "  Error log:  $ERROR_LOG"
echo -e "  Restart log: $RESTART_LOG"
echo ""
echo -e "${YELLOW}Press Ctrl+C to stop the supervisor and server${NC}"
echo ""

# Start monitoring
monitor_server
