# ==============================================================================
# Version Extraction Script for PlatformIO
# ==============================================================================
# Description: Automatically extracts and stores version information for:
#              - Xtensa ESP32-S3 Compiler
#              - Espressif32 Platform
#
# Output:      Creates include/compiler_version.h with version defines
#
# Author:      Auto-generated for EARS Project
# Version:     1.0.0
# ==============================================================================

Import("env")
import subprocess
import re
import os

def print_banner(message):
    """Print a visible banner message"""
    banner = "=" * 70
    print(f"\n{banner}")
    print(f"  {message}")
    print(f"{banner}\n")

def extract_versions(source, target, env):
    """Extract compiler and platform versions and create header file"""
    
    print_banner("VERSION EXTRACTION SCRIPT")
    
    # ========================================================================
    # EXTRACT XTENSA COMPILER VERSION
    # ========================================================================
    compiler = env.subst("$CC")
    
    xtensa_version = "UNKNOWN"
    xtensa_major = 0
    xtensa_minor = 0
    xtensa_patch = 0
    
    try:
        result = subprocess.run(
            [compiler, "--version"],
            capture_output=True,
            text=True,
            timeout=10,
            shell=True
        )
        
        version_output = result.stdout + result.stderr
        version_match = re.search(r'(\d+)\.(\d+)\.(\d+)', version_output)
        
        if version_match:
            xtensa_major = version_match.group(1)
            xtensa_minor = version_match.group(2)
            xtensa_patch = version_match.group(3)
            xtensa_version = f"{xtensa_major}.{xtensa_minor}.{xtensa_patch}"
            print(f"✓ Xtensa Compiler: {xtensa_version}")
        else:
            print("⚠ Warning: Could not parse Xtensa compiler version")
            
    except Exception as e:
        print(f"✗ Error extracting Xtensa version: {e}")
    
    # ========================================================================
    # EXTRACT ESPRESSIF PLATFORM VERSION
    # ========================================================================
    platform_version = "UNKNOWN"
    platform_major = 0
    platform_minor = 0
    platform_patch = 0
    
    try:
        platform = env.PioPlatform()
        platform_version_raw = platform.version
        
        version_match = re.search(r'(\d+)\.(\d+)\.(\d+)', str(platform_version_raw))
        
        if version_match:
            platform_major = version_match.group(1)
            platform_minor = version_match.group(2)
            platform_patch = version_match.group(3)
            platform_version = f"{platform_major}.{platform_minor}.{platform_patch}"
            print(f"✓ Espressif Platform: {platform_version}")
        else:
            print("⚠ Warning: Could not parse Espressif platform version")
        
    except Exception as e:
        print(f"✗ Error extracting platform version: {e}")
    
    # ========================================================================
    # CREATE HEADER FILE
    # ========================================================================
    project_dir = env.subst("$PROJECT_DIR")
    include_dir = os.path.join(project_dir, "include")
    
    if not os.path.exists(include_dir):
        os.makedirs(include_dir, exist_ok=True)
    
    header_path = os.path.join(include_dir, "compiler_version.h")
    
    try:
        with open(header_path, 'w') as f:
            f.write("// Auto-generated version information\n")
            f.write("// Do not edit manually\n")
            f.write(f"// Generated on: {env.subst('$PIOENV')}\n")
            f.write(f"// Build timestamp: {env.subst('$UNIX_TIME')}\n\n")
            
            f.write("#ifndef COMPILER_VERSION_H\n")
            f.write("#define COMPILER_VERSION_H\n\n")
            
            f.write("// Xtensa Compiler Version\n")
            f.write(f"#define XTENSA_COMPILER_VERSION \"{xtensa_version}\"\n")
            f.write(f"#define XTENSA_COMPILER_MAJOR {xtensa_major}\n")
            f.write(f"#define XTENSA_COMPILER_MINOR {xtensa_minor}\n")
            f.write(f"#define XTENSA_COMPILER_PATCH {xtensa_patch}\n\n")
            
            f.write("// Espressif Platform Version (espressif32)\n")
            f.write(f"#define ESPRESSIF_PLATFORM_VERSION \"{platform_version}\"\n")
            f.write(f"#define ESPRESSIF_PLATFORM_MAJOR {platform_major}\n")
            f.write(f"#define ESPRESSIF_PLATFORM_MINOR {platform_minor}\n")
            f.write(f"#define ESPRESSIF_PLATFORM_PATCH {platform_patch}\n\n")
            
            f.write("#endif // COMPILER_VERSION_H\n")
        
        if os.path.exists(header_path):
            file_size = os.path.getsize(header_path)
            print(f"✓ Header created: {header_path} ({file_size} bytes)")
        else:
            print(f"✗ Error: Header file was not created!")
            
    except Exception as e:
        print(f"✗ Error writing header file: {e}")
    
    print_banner("VERSION EXTRACTION COMPLETE")

# Register the callback to run before build
env.AddPreAction("buildprog", extract_versions)