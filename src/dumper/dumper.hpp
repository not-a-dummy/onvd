#pragma once

namespace csgo
{
    struct netvar_t;

    struct dumper_t
    {
        enum class EType
        {
            CPP_HEADER,
            CSHARP,
            RAW,
        };

        dumper_t(const netvar_t& netvar);
        void to_file(std::string_view filename, EType type = EType::RAW, bool filter = true);

    private:
        void dump_cpp_header(std::ofstream& stream, bool filter);
        void dump_csharp(std::ofstream& stream, bool filter);
        void dump_raw(std::ofstream& stream, bool filter);

    private:
        const netvar_t& m_netvar;
        std::unordered_map<std::string, /// filtered table name
            std::unordered_map<std::string, /// filtered prop name
            uintptr_t /// offset
        >> m_filtered_netvar = {};
    };
}
