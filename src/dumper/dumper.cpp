#include "../includes.hpp"

csgo::dumper_t::dumper_t(const netvar_t& netvar) : m_netvar(netvar)
{
    static auto clean_prop_name = [](std::string_view prop_name) -> std::string
    {
        if (!prop_name.starts_with("m_"))
            return {};

        /// it's a vector3 datatype, just grab the first element's offset
        if (prop_name.ends_with("[1]") || prop_name.ends_with("[2]"))
            return {};

        std::string ret = {};
        ret.resize(prop_name.length());

        std::replace_copy(
            prop_name.begin(), prop_name.end(), 
            ret.begin(), '.', '_'
        );

        if (prop_name.ends_with("[0]"))
            return std::string(ret.begin(), ret.end() - 3);

        return ret;
    };

    for (const auto& [table, prop_data] : m_netvar.get_map())
    {
        if (!table.starts_with("DT_"))
            continue;

        /// check if at least one of the prop name starts with "m_" so we won't get an empty "valid" table
        if (!std::any_of(prop_data.begin(), prop_data.end(), 
            [](const auto& prop) { return prop.first.starts_with("m_"); }))
            continue;

        for (const auto& [prop_name, prop_offset] : prop_data)
        {
            std::string cleaned_name = clean_prop_name(prop_name);
            if (!cleaned_name.empty())
                m_filtered_netvar[table][std::move(cleaned_name)] = prop_offset;
        }
    }
}

void csgo::dumper_t::to_file(std::string_view filename, EType type, bool filter)
{
    std::ofstream out(filename.data());
    if (out.fail())
    {
        LOG_ERROR("Failed to create '%s' file for output!", filename.data());
        return;
    }

    switch (type)
    {
    case EType::CPP_HEADER:
        dump_cpp_header(out, filter);
        break;

    case EType::CSHARP:
        dump_csharp(out, filter);
        break;

    case EType::RAW: 
    default:
        dump_raw(out, filter);
        break;
    }

    LOG_SUCCESS("File saved as '%s'\t(%s)", filename.data(), filter ? "filtered" : "unfiltered");
}

void csgo::dumper_t::dump_cpp_header(std::ofstream& stream, bool filter)
{
    stream << "#pragma once\n\n"
           << "#include <cstdint>\n\n";

    for (const auto& [table, prop_data] : filter ? m_filtered_netvar : m_netvar.get_map())
    {
        stream << "namespace " << table << "\n{\n";

        for (const auto& [prop_name, prop_offset] : prop_data)
        {
            stream << '\t';

            /// don't ask.
            if (filter && prop_name.find("localSound") != std::string::npos)
                stream << "/// ";

            stream << "constexpr uintptr_t " << prop_name << " = 0x"
                   << std::uppercase << std::setw(4) << std::setfill('0') << std::hex << prop_offset << ";\n";
        }

        stream << "} /// namespace " << table << "\n\n";
    }
}

void csgo::dumper_t::dump_csharp(std::ofstream& stream, bool filter)
{
    stream << "using System;\n\n"
           << "namespace onvd\n{";

    for (const auto &[table, prop_data] : filter ? m_filtered_netvar : m_netvar.get_map())
    {
        stream << "\n\tpublic static class " << table << "\n\t{\n";

        for (const auto& [prop_name, prop_offset] : prop_data)
        {
            stream << "\t\t";

            /// don't ask.
            if (filter && prop_name.find("localSound") != std::string::npos)
                stream << "/// ";

            stream << "public const Int32 " << prop_name << " = 0x"
                   << std::uppercase << std::setw(4) << std::setfill('0') << std::hex << prop_offset << ";\n";
        }

        stream << "\t} /// namespace " << table << "\n\n";
    }
}

void csgo::dumper_t::dump_raw(std::ofstream& stream, bool filter)
{
    for (const auto& [table, prop_data] : filter ? m_filtered_netvar : m_netvar.get_map())
    {
        stream << "[" << table << "]\n";

        for (const auto& [prop_name, prop_offset] : prop_data)
        {
            stream << "\t- " << prop_name << ' ' << std::string(40 - prop_name.length(), '-') << "> 0x"
                   << std::uppercase << std::setw(4) << std::setfill('0') << std::hex << prop_offset << '\n';
        }

        stream << '\n';
    }
}