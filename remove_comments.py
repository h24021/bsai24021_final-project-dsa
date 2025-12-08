#!/usr/bin/env python3
import re
import os
import sys

def remove_comments(text):
    # Remove single-line comments
    text = re.sub(r'//.*?$', '', text, flags=re.MULTILINE)
    # Remove multi-line comments
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.DOTALL)
    # Remove multiple consecutive blank lines
    text = re.sub(r'\n\s*\n\s*\n', '\n\n', text)
    return text

def process_file(filepath):
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        
        cleaned = remove_comments(content)
        
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(cleaned)
        
        print(f"✓ {filepath}")
        return True
    except Exception as e:
        print(f"✗ {filepath}: {e}")
        return False

def main():
    processed = 0
    failed = 0
    
    # Process backend files
    for root, dirs, files in os.walk('backend'):
        for file in files:
            if file.endswith(('.cpp', '.h')):
                filepath = os.path.join(root, file)
                if process_file(filepath):
                    processed += 1
                else:
                    failed += 1
    
    # Process test files
    for root, dirs, files in os.walk('tests'):
        for file in files:
            if file.endswith(('.cpp', '.h')):
                filepath = os.path.join(root, file)
                if process_file(filepath):
                    processed += 1
                else:
                    failed += 1
    
    print(f"\n{'='*50}")
    print(f"✓ Processed: {processed} files")
    print(f"✗ Failed: {failed} files")
    print(f"{'='*50}")

if __name__ == '__main__':
    main()
