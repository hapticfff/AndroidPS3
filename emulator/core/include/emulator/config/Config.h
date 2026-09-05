#pragma once

#include <string>
#include <unordered_map>

namespace emu::config {

class Config {
public:
    static Config& Instance();

    void SetString(const std::string& key, const std::string& value);
    void SetBool(const std::string& key, bool value);
    void SetInt(const std::string& key, int value);

    std::string GetString(const std::string& key, const std::string& default_value) const;
    bool GetBool(const std::string& key, bool default_value) const;
    int GetInt(const std::string& key, int default_value) const;

    bool HasKey(const std::string& key) const;
    std::string Serialize() const;

private:
    Config() = default;
    std::unordered_map<std::string, std::string> values_;
};

} // namespace emu::config
