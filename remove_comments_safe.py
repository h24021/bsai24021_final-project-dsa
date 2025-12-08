#!/usr/bin/env python3
import re
import os

def remove_comments_safe(text):
    def replacer(match):
        s = match.group(0)
        if s.startswith('/'):
            return " "
        else:
            return s
    
    pattern = re.compile(
        r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
        re.DOTALL | re.MULTILINE
    )
    
    result = re.sub(pattern, replacer, text)
    result = re.sub(r'\n\s*\n\s*\n+', '\n\n', result)
    
    return result

def process_file(filepath):
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        
        if not content.strip():
            print(f"⊘ {filepath} (empty, skipping)")
            return True
        
        cleaned = remove_comments_safe(content)
        
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
    
    for root, dirs, files in os.walk('backend'):
        for file in files:
            if file.endswith(('.cpp', '.h')):
                filepath = os.path.join(root, file)
                if process_file(filepath):
                    processed += 1
                else:
                    failed += 1
    
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
