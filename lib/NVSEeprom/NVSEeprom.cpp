#include "NVSEeprom.h"

const char* NVSEeprom::NAMESPACE = "EARS";

NVSEeprom::NVSEeprom() {
}

NVSEeprom::~NVSEeprom() {
}

String NVSEeprom::getHash(const char* key, const String& defaultValue) {
    // Open namespace in read-only mode
    if (!begin(NAMESPACE, true)) {
        return defaultValue;
    }
    
    String hash = getString(key, defaultValue);
    end();
    
    return hash;
}

bool NVSEeprom::putHash(const char* key, const String& value) {
    // Open namespace in read-write mode
    if (!begin(NAMESPACE, false)) {
        return false;
    }
    
    size_t result = putString(key, value);
    end();
    
    return (result > 0);
}