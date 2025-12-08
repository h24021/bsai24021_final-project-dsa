#!/bin/bash

# Function to remove comments from a C++ file
remove_cpp_comments() {
    local file="$1"
    
    # Create a temporary file
    local temp_file="${file}.tmp"
    
    # Remove single-line comments (//) and multi-line comments (/* */)
    # This uses sed to remove comments while preserving code
    sed -e 's://.*$::g' \
        -e ':a' -e '/\/\*/!b;N;/*\//!ba;s:/\*.*\*/::g' \
        "$file" > "$temp_file"
    
    # Remove empty lines that may have been created
    sed -i '' '/^[[:space:]]*$/d' "$temp_file"
    
    # Replace original file
    mv "$temp_file" "$file"
    
    echo "Processed: $file"
}

# Export function for use with find
export -f remove_cpp_comments

# Find and process all .cpp and .h files
find backend -type f \( -name "*.cpp" -o -name "*.h" \) -exec bash -c 'remove_cpp_comments "$0"' {} \;
find tests -type f \( -name "*.cpp" -o -name "*.h" \) -exec bash -c 'remove_cpp_comments "$0"' {} \; 2>/dev/null

echo "All comments removed from C++ files!"
