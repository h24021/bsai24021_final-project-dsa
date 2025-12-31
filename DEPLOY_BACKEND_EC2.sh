#!/bin/bash

# ============================================
# EC2 Backend Deployment Script
# ============================================

set -e  # Exit on any error

EC2_IP="13.61.186.232"
EC2_USER="ubuntu"
KEY_FILE="mykey.pem"
REPO_URL="https://github.com/h24021/bsai24021_final-project-dsa.git"
PROJECT_DIR="bsai24021_final-project-dsa"

echo "======================================"
echo "Deploying Backend to EC2"
echo "======================================"
echo ""

# Step 1: Push latest code to GitHub
echo "Step 1: Pushing code to GitHub..."
git add -A
git commit -m "Deploy: Backend and Frontend with authentication system" || echo "No changes to commit"
git push origin main

echo ""
echo "✓ Code pushed to GitHub"
echo ""

# Step 2: Connect to EC2 and deploy
echo "Step 2: Connecting to EC2 and deploying..."
echo ""

ssh -i "$KEY_FILE" -o StrictHostKeyChecking=no ${EC2_USER}@${EC2_IP} << 'ENDSSH'

echo "======================================"
echo "Setting up environment on EC2..."
echo "======================================"

# Install dependencies if not already installed
if ! command -v g++ &> /dev/null; then
    echo "Installing dependencies..."
    sudo apt update
    sudo apt install -y git build-essential g++ make python3
fi

echo "✓ Dependencies installed"
echo ""

# Navigate to home directory
cd ~

# Clone or pull repository
if [ -d "bsai24021_final-project-dsa" ]; then
    echo "Repository exists, pulling latest changes..."
    cd bsai24021_final-project-dsa
    git pull origin main
else
    echo "Cloning repository..."
    git clone https://github.com/h24021/bsai24021_final-project-dsa.git
    cd bsai24021_final-project-dsa
fi

echo "✓ Code updated"
echo ""

# Kill any existing processes
echo "Stopping existing services..."
pkill -f supervisor.sh || true
pkill -f http_api_server || true
pkill -f 'http.server' || true
sleep 2

echo "✓ Existing services stopped"
echo ""

# Build the backend
echo "Building backend..."
make clean
make

echo "✓ Backend built successfully"
echo ""

# Make supervisor executable
chmod +x supervisor.sh

# Create logs directory if it doesn't exist
mkdir -p logs

# Start the supervisor in background
echo "Starting backend and frontend servers..."
nohup ./supervisor.sh > /dev/null 2>&1 &

sleep 3

# Check if servers are running
if pgrep -f http_api_server > /dev/null; then
    echo "✓ Backend API server is running"
else
    echo "✗ Backend API server failed to start"
    exit 1
fi

if pgrep -f 'http.server' > /dev/null; then
    echo "✓ Frontend server is running"
else
    echo "✗ Frontend server failed to start"
    exit 1
fi

echo ""
echo "======================================"
echo "Deployment Successful!"
echo "======================================"
echo ""
echo "Services are running:"
echo "  Backend:  http://13.61.186.232:8080"
echo "  Frontend: http://13.61.186.232:8081"
echo ""
echo "To check logs:"
echo "  tail -f ~/bsai24021_final-project-dsa/logs/api_server.log"
echo "  tail -f ~/bsai24021_final-project-dsa/logs/frontend_server.log"
echo ""

ENDSSH

echo ""
echo "======================================"
echo "Local Deployment Complete!"
echo "======================================"
echo ""
echo "Your application is now running on EC2:"
echo "  Backend API:  http://13.61.186.232:8080/api/v1"
echo "  Frontend App: http://13.61.186.232:8081"
echo ""
echo "Test the backend:"
echo "  curl http://13.61.186.232:8080/api/v1/books"
echo ""
