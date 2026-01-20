Import("env")
import subprocess
import re
import os

def extract_xtensa_version(source, target, env):
    """Extract Xtensa compiler version and create header file"""
    
    # Get the compiler path from PlatformIO environment
    compiler = env.subst("$CC")
    
    print(f"Compiler path: {compiler}")
    
    try:
        # Run compiler with --version flag
        result = subprocess.run(
            [compiler, "--version"],
            capture_output=True,
            text=True,
            timeout=5,
            shell=True  # Added for Windows compatibility
        )
        
        version_output = result.stdout + result.stderr  # Some compilers output to stderr
        
        print(f"Compiler output: {version_output[:200]}")  # Debug: show first 200 chars
        
        # Extract version number (looking for pattern like "8.4.0" or "5.2.0")
        version_match = re.search(r'(\d+)\.(\d+)\.(\d+)', version_output)
        
        if version_match:
            major = version_match.group(1)
            minor = version_match.group(2)
            patch = version_match.group(3)
            full_version = f"{major}.{minor}.{patch}"
            
            # Get project directory
            project_dir = env.subst("$PROJECT_DIR")
            
            # Create include directory if it doesn't exist
            include_dir = os.path.join(project_dir, "include")
            os.makedirs(include_dir, exist_ok=True)
            
            # Create header file
            header_path = os.path.join(include_dir, "compiler_version.h")
            
            with open(header_path, 'w') as f:
                f.write("// Auto-generated compiler version\n")
                f.write("// Do not edit manually\n\n")
                f.write("#ifndef COMPILER_VERSION_H\n")
                f.write("#define COMPILER_VERSION_H\n\n")
                f.write(f"#define XTENSA_COMPILER_VERSION \"{full_version}\"\n")
                f.write(f"#define XTENSA_COMPILER_MAJOR {major}\n")
                f.write(f"#define XTENSA_COMPILER_MINOR {minor}\n")
                f.write(f"#define XTENSA_COMPILER_PATCH {patch}\n\n")
                f.write("#endif // COMPILER_VERSION_H\n")
            
            print(f"✓ Xtensa compiler version: {full_version}")
            print(f"✓ Header file created: {header_path}")
        else:
            print("⚠ Warning: Could not parse compiler version from output")
            print(f"   Full output: {version_output}")
            
    except Exception as e:
        print(f"⚠ Error extracting compiler version: {e}")
        import traceback
        traceback.print_exc()

# Register the callback to run before build
env.AddPreAction("buildprog", extract_xtensa_version)