"""
Doxygen Annotation Validator
Checks all .cpp and .h files for proper application-specific Doxygen annotations
Run this before compilation to catch documentation issues early
"""

import os
import re
import sys
from pathlib import Path

class DoxygenValidator:
    """Validates Doxygen annotations in source files"""
    
    def __init__(self, project_root):
        self.project_root = Path(project_root)
        self.errors = []
        self.warnings = []
        
        # Define allowed Doxygen commands for your application
        self.allowed_commands = {
            '@file', '@brief', '@author', '@date', '@version',
            '@param', '@return', '@returns', '@note', '@warning',
            '@see', '@class', '@struct', '@enum', '@var',
            '@code', '@endcode', '@example', '@details',
            '@pre', '@post', '@todo', '@bug', '@deprecated'
        }
        
        # Commands that should NOT be used
        self.forbidden_commands = {
            '@internal', '@mainpage', '@page', '@section',
            '@subsection', '@private'  # Add any others you want to forbid
        }
    
    def validate_file(self, filepath):
        """Validate a single file for proper Doxygen usage"""
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
                lines = content.split('\n')
                
            # Find all Doxygen comments
            in_comment = False
            comment_start_line = 0
            
            for line_num, line in enumerate(lines, 1):
                # Check for Doxygen comment blocks
                if '/**' in line or '/*!' in line:
                    in_comment = True
                    comment_start_line = line_num
                
                if in_comment:
                    # Find all @ commands
                    commands = re.findall(r'@\w+', line)
                    
                    for cmd in commands:
                        # Check for forbidden commands
                        if cmd in self.forbidden_commands:
                            self.errors.append(
                                f"{filepath}:{line_num} - Forbidden Doxygen command: {cmd}"
                            )
                        
                        # Check for unknown commands
                        elif cmd not in self.allowed_commands:
                            self.warnings.append(
                                f"{filepath}:{line_num} - Unknown Doxygen command: {cmd}"
                            )
                
                if '*/' in line:
                    in_comment = False
                    
        except Exception as e:
            self.errors.append(f"Error reading {filepath}: {str(e)}")
    
    def scan_directory(self, directory, extensions=('.cpp', '.h')):
        """Scan directory recursively for source files"""
        file_count = 0
        
        for root, dirs, files in os.walk(directory):
            # Skip certain directories
            dirs[:] = [d for d in dirs if d not in ['.git', '.pio', 'build', 'test']]
            
            for file in files:
                if file.endswith(extensions):
                    filepath = Path(root) / file
                    self.validate_file(filepath)
                    file_count += 1
        
        return file_count
    
    def report(self):
        """Generate validation report"""
        print("\n" + "="*70)
        print("DOXYGEN ANNOTATION VALIDATION REPORT")
        print("="*70)
        
        if not self.errors and not self.warnings:
            print("\n✓ All files passed validation!")
            return 0
        
        if self.errors:
            print(f"\n❌ ERRORS ({len(self.errors)}):")
            print("-"*70)
            for error in self.errors:
                print(f"  {error}")
        
        if self.warnings:
            print(f"\n⚠ WARNINGS ({len(self.warnings)}):")
            print("-"*70)
            for warning in self.warnings:
                print(f"  {warning}")
        
        print("\n" + "="*70)
        
        return len(self.errors)  # Return error count for exit code


def main():
    """Main entry point"""
    # Get project root (assumes script is in project root or tools folder)
    if len(sys.argv) > 1:
        project_root = sys.argv[1]
    else:
        project_root = os.getcwd()
    
    print(f"Scanning project: {project_root}")
    
    validator = DoxygenValidator(project_root)
    
    # Scan source directories
    src_dir = Path(project_root) / 'src'
    lib_dir = Path(project_root) / 'lib'
    include_dir = Path(project_root) / 'include'
    
    file_count = 0
    
    if src_dir.exists():
        print(f"Scanning src/...")
        file_count += validator.scan_directory(src_dir)
    
    if lib_dir.exists():
        print(f"Scanning lib/...")
        file_count += validator.scan_directory(lib_dir)
    
    if include_dir.exists():
        print(f"Scanning include/...")
        file_count += validator.scan_directory(include_dir)
    
    print(f"\nScanned {file_count} files")
    
    # Generate report and exit with appropriate code
    error_count = validator.report()
    sys.exit(error_count)


if __name__ == "__main__":
    main()