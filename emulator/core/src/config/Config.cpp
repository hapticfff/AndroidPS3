#include "emulator/config/Config.h"

#include <sstream>

namespace emu::config {

Config& Config::Instance() {
    static Config instance;
    return instance;
}

void Config::SetString(const std::string& key, const std::string& value) { values_[key] = value; }
void Config::SetBool(const std::string& key, bool value) { values_[key] = value ? "true" : "false"; }
void Config::SetInt(const std::string& key, int value) { values_[key] = std::to_string(value); }

std::string Config::GetString(const std::string& key, const std::string& default_value) const {
    auto it = values_.find(key);
    return it != values_.end() ? it->second : default_value;
}

bool Config::GetBool(const std::string& key, bool default_value) const {
    auto it = values_.find(key);
    if (it == values_.end()) return default_value;
    return it->second == "true";
}

int Config::GetInt(const std::string& key, int default_value) const {
    auto it = values_.find(key);
    if (it == values_.end()) return default_value;
    try { return std::stoi(it->second); } catch (...) { return default_value; }
}

bool Config::HasKey(const std::string& key) const { return values_.find(key) != values_.end(); }

std::string Config::Serialize() const {
    std::ostringstream out;
    for (const auto& [key, value] : values_) out << key << "=" << value << "\n";
    return out.str();
}

} // namespace emu::config
