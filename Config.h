#pragma once
#include <string>
#include <unordered_map>

class Config
{
public:
    bool loadFromFile(const std::string& path);

    // Accessors with defaults
    int getInt(const std::string& key, int defaultValue = 0) const;
    float getFloat(const std::string& key, float defaultValue = 0.0f) const;
    bool getBool(const std::string& key, bool defaultValue = false) const;
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;

private:
    std::unordered_map<std::string, std::string> values;
    static std::string trim(const std::string& s);
};

