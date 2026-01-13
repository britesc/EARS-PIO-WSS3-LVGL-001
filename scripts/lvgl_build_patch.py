# Place this file in project root as: lvgl_build_patch.py
# This pre-build script excludes ARM assembly files from LVGL compilation

Import("env") # type: ignore
import os

def exclude_arm_files(node):
    """Exclude ARM Helium and NEON assembly files"""
    return None if ("helium" in node.get_path() or 
                    "neon" in node.get_path() or
                    node.get_path().endswith(".S")) else node

# Apply filter to source files
env.AddBuildMiddleware(exclude_arm_files, "*")

print("=" * 60)
print("LVGL Build Patch: Excluding ARM assembly files")
print("=" * 60)
