Import("env")
import subprocess
import re
import os
import sys

def print_banner(message):
    """Print a visible banner message"""
    banner = "=" * 70
    print(f"\n{banner}")
    print(f"  {message}")
    print(f"{banner}\n")

def extract_xtensa_version(source, target, env):
    """Extract Xtensa compiler version and create header file"""
    
    print_banner("COMPILER VERSION EXTRACTION SCRIPT STARTED")
    
    # Get the compiler path from PlatformIO environment
    compiler = env.subst("$CC")
    
    print(f"[INFO] Compiler path: {compiler}")
    print(f"[INFO] Python version: {sys.version}")
    print(f"[INFO] Working directory: {os.getcwd()}")
    
    try:
        # Run compiler with --version flag
        print(f"[INFO] Running: {compiler} --version")
        
        result = subprocess.run(
            [compiler, "--version"],
            capture_output=True,
            text=True,
            timeout=10,
            shell=True
        )
        
        version_output = result.stdout + result.stderr
        
        print(f"[DEBUG] Return code: {result.returncode}")
        print(f"[DEBUG] Output length: {len(version_output)} characters")
        print(f"[DEBUG] First 300 chars of output:")
        print(f"        {version_output[:300]}")
        
        # Extract version number (looking for pattern like "8.4.0" or "5.2.0")
        version_match = re.search(r'(\d+)\.(\d+)\.(\d+)', version_output)
        
        if version_match:
            major = version_match.group(1)
            minor = version_match.group(2)
            patch = version_match.group(3)
            full_version = f"{major}.{minor}.{patch}"
            
            print(f"[SUCCESS] Extracted version: {full_version}")
            print(f"          Major={major}, Minor={minor}, Patch={patch}")
            
            # Get project directory
            project_dir = env.subst("$PROJECT_DIR")
            print(f"[INFO] Project directory: {project_dir}")
            
            # Create include directory if it doesn't exist
            include_dir = os.path.join(project_dir, "include")
            
            if not os.path.exists(include_dir):
                print(f"[INFO] Creating include directory: {include_dir}")
                os.makedirs(include_dir, exist_ok=True)
            else:
                print(f"[INFO] Include directory exists: {include_dir}")
            
            # Create header file
            header_path = os.path.join(include_dir, "compiler_version.h")
            
            print(f"[INFO] Writing header file: {header_path}")
            
            with open(header_path, 'w') as f:
                f.write("// Auto-generated compiler version\n")
                f.write("// Do not edit manually\n")
                f.write(f"// Generated on: {env.subst('$PIOENV')}\n\n")
                f.write("#ifndef COMPILER_VERSION_H\n")
                f.write("#define COMPILER_VERSION_H\n\n")
                f.write(f"#define XTENSA_COMPILER_VERSION \"{full_version}\"\n")
                f.write(f"#define XTENSA_COMPILER_MAJOR {major}\n")
                f.write(f"#define XTENSA_COMPILER_MINOR {minor}\n")
                f.write(f"#define XTENSA_COMPILER_PATCH {patch}\n\n")
                f.write("#endif // COMPILER_VERSION_H\n")
            
            # Verify file was created
            if os.path.exists(header_path):
                file_size = os.path.getsize(header_path)
                print(f"[SUCCESS] Header file created successfully!")
                print(f"          Path: {header_path}")
                print(f"          Size: {file_size} bytes")
                
                # Show content
                print(f"[INFO] Header file contents:")
                with open(header_path, 'r') as f:
                    for i, line in enumerate(f, 1):
                        print(f"        {i:2d}: {line.rstrip()}")
            else:
                print(f"[ERROR] Header file was NOT created!")
                
        else:
            print("[WARNING] Could not parse compiler version from output")
            print(f"[DEBUG] Full output:\n{version_output}")
            
    except subprocess.TimeoutExpired:
        print("[ERROR] Compiler command timed out!")
    except FileNotFoundError:
        print(f"[ERROR] Compiler not found: {compiler}")
    except Exception as e:
        print(f"[ERROR] Exception occurred: {e}")
        import traceback
        print("[DEBUG] Full traceback:")
        traceback.print_exc()
    
    print_banner("COMPILER VERSION EXTRACTION SCRIPT FINISHED")

# Register the callback to run before build
print("[SCRIPT] Registering pre-build action...")
env.AddPreAction("buildprog", extract_xtensa_version)
print("[SCRIPT] Pre-build action registered successfully!")