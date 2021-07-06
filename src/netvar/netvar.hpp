#pragma once

namespace csgo
{
    struct netvar_t
    {
        netvar_t(const modules_t& modules) : m_modules(modules) {}
        bool init() const;
        std::unordered_map<std::string, /// table name
            std::unordered_map<std::string, /// prop name
            uintptr_t /// offset
        >>& get_map() const;

    private:
        const modules_t& m_modules;
    };
}
