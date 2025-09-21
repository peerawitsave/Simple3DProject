#include "Config.h"
#include <fstream>
#include <sstream>
#include <algorithm>

static bool parseBool(const std::string& s, bool defaultValue) {
    if (s.empty()) return defaultValue;
    std::string v = s;
    std::transform(v.begin(), v.end(), v.begin(), ::tolower);
    if (v == "true" || v == "1" || v == "yes" || v == "on") return true;
    if (v == "false" || v == "0" || v == "no" || v == "off") return false;
    return defaultValue;
}

bool Config::loadFromFile(const std::string& path)
{
    values.clear();
    std::ifstream f(path);
    if (!f.is_open()) return false;

    std::string line;
    while (std::getline(f, line)) {
        // strip comments
        auto commentPos = line.find_first_of("#;");
        if (commentPos != std::string::npos) line = line.substr(0, commentPos);
        line = trim(line);
        if (line.empty()) continue;
        auto eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = trim(line.substr(0, eq));
        std::string val = trim(line.substr(eq + 1));
        values[key] = val;
    }
    return true;
}

int Config::getInt(const std::string& key, int defaultValue) const
{
    auto it = values.find(key);
    if (it == values.end()) return defaultValue;
    try { return std::stoi(it->second); } catch (...) { return defaultValue; }
}

float Config::getFloat(const std::string& key, float defaultValue) const
{
    auto it = values.find(key);
    if (it == values.end()) return defaultValue;
    try { return std::stof(it->second); } catch (...) { return defaultValue; }
}

bool Config::getBool(const std::string& key, bool defaultValue) const
{
    auto it = values.find(key);
    if (it == values.end()) return defaultValue;
    return parseBool(it->second, defaultValue);
}

std::string Config::getString(const std::string& key, const std::string& defaultValue) const
{
    auto it = values.find(key);
    if (it == values.end()) return defaultValue;
    return it->second;
}

std::string Config::trim(const std::string& s)
{
    size_t start = 0;
    while (start < s.size() && isspace(static_cast<unsigned char>(s[start]))) start++;
    size_t end = s.size();
    while (end > start && isspace(static_cast<unsigned char>(s[end - 1]))) end--;
    return s.substr(start, end - start);
}

