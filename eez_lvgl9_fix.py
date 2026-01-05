"""
EEZ Studio LVGL 9.3 Compatibility Fix
This script fixes LVGL function calls in EEZ Studio generated code
to be compatible with LVGL 9.3.0

Author: JTB
Created: 20250105
"""

# type: ignore - PlatformIO build script
Import("env")  # Provided by PlatformIO SCons environment
import os
import re

def fix_eez_lvgl9_compatibility(source, target, env):
    """
    Fix LVGL 9.x compatibility issues in eez-flow.cpp
    
    Fixes:
    1. lv_dropdown_set_selected() - Remove 3rd parameter (LV_ANIM_ON/OFF)
    """
    
    # Path to the eez-flow.cpp file
    eez_flow_path = os.path.join(env.subst("$PROJECT_DIR"), "src", "ui", "eez-flow.cpp")
    
    # Check if file exists
    if not os.path.exists(eez_flow_path):
        print(f"⚠️  EEZ flow file not found: {eez_flow_path}")
        return
    
    print(f"🔧 Checking EEZ Studio compatibility: {eez_flow_path}")
    
    # Read the file
    with open(eez_flow_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    original_content = content
    fixes_applied = 0
    
    # Fix 1: lv_dropdown_set_selected() - Remove animation parameter
    # Pattern: lv_dropdown_set_selected(obj, value, LV_ANIM_ON/OFF)
    # Replace with: lv_dropdown_set_selected(obj, value)
    
    pattern = r'lv_dropdown_set_selected\s*\(\s*([^,]+)\s*,\s*([^,]+)\s*,\s*LV_ANIM_(?:ON|OFF)\s*\)'
    replacement = r'lv_dropdown_set_selected(\1, \2)'
    
    content, count = re.subn(pattern, replacement, content)
    fixes_applied += count
    
    if count > 0:
        print(f"   ✅ Fixed {count} lv_dropdown_set_selected() call(s)")
    
    # Add more fixes here as needed for other LVGL 9.x compatibility issues
    # Example:
    # pattern2 = r'old_function\((.*?)\)'
    # replacement2 = r'new_function(\1)'
    # content, count2 = re.subn(pattern2, replacement2, content)
    # fixes_applied += count2
    
    # Write back if changes were made
    if content != original_content:
        with open(eez_flow_path, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"✅ EEZ Studio compatibility fixes applied: {fixes_applied} changes")
    else:
        print("✅ EEZ Studio code already compatible - no fixes needed")

# Register the callback to run before building
env.AddPreAction("$BUILD_DIR/${PROGNAME}.elf", fix_eez_lvgl9_compatibility)

print("📝 EEZ Studio LVGL 9.3 compatibility fixer loaded")
