#ifndef NVSEEPROM_H
#define NVSEEPROM_H

#include <Preferences.h>
#include <Arduino.h>

class NVSEeprom : public Preferences {
public:
    NVSEeprom();
    ~NVSEeprom();
    
    // Hash functions - Step 1
    String getHash(const char* key, const String& defaultValue = "");
    bool putHash(const char* key, const String& value);
    
private:
    static const char* NAMESPACE;
};

#endif