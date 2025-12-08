#!/bin/bash
# Quick Test Runner for Library Management System

echo "╔════════════════════════════════════════════════════════════════╗"
echo "║     Library Management System - B-Tree Implementation         ║"
echo "║                    Quick Test Script                          ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""

# Navigate to project directory
cd "$(dirname "$0")"

echo "📁 Current Directory: $(pwd)"
echo ""

# Menu
echo "Choose an option:"
echo "  1. Run all tests (recommended)"
echo "  2. Run demo application"
echo "  3. Clean and rebuild everything"
echo "  4. Show project structure"
echo "  5. Run tests and demo (full verification)"
echo ""
read -p "Enter choice (1-5): " choice

case $choice in
    1)
        echo ""
        echo "🧪 Running Test Suite..."
        echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
        make test
        ;;
    2)
        echo ""
        echo "🚀 Running Demo Application..."
        echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
        make run
        ;;
    3)
        echo ""
        echo "🧹 Cleaning and rebuilding..."
        echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
        make clean
        make build-all
        echo ""
        echo "✅ Build complete!"
        ;;
    4)
        echo ""
        echo "📂 Project Structure:"
        echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
        tree -L 2 -I 'build|.git' || ls -R
        ;;
    5)
        echo ""
        echo "🔍 Full Verification (Tests + Demo)..."
        echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
        echo ""
        echo "Step 1: Running Tests..."
        make clean
        make test
        
        echo ""
        echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
        echo "Step 2: Running Demo..."
        make run
        
        echo ""
        echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
        echo "✅ Full verification complete!"
        ;;
    *)
        echo "❌ Invalid choice. Please run again."
        exit 1
        ;;
esac

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "✨ Done!"
echo ""
echo "📚 Documentation Files:"
echo "  • README_BTREE.md          - Detailed documentation"
echo "  • BTREE_QUICK_GUIDE.md     - Quick reference"
echo "  • IMPLEMENTATION_SUMMARY.md - Summary and highlights"
echo "  • ARCHITECTURE_DIAGRAM.md   - System architecture"
echo ""
echo "🔧 Make Commands:"
echo "  • make test       - Run test suite"
echo "  • make run        - Run demo"
echo "  • make clean      - Clean build files"
echo "  • make build-all  - Build tests + demo"
echo ""
