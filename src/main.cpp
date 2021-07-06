#include "includes.hpp"

int main()
{
    csgo::modules_t& modules = csgo::modules_t::get();
    if (!modules.init())
    {
        LOG_ERROR("Failed to load needed modules!");
        return 1;
    }

    csgo::netvar_t netvar(modules);
    if (!netvar.init())
    {
        LOG_ERROR("Failed to dump netvar!");
        return 1;
    }

    csgo::dumper_t dumper(netvar);
    dumper.to_file(WIN_LINUX("windows.", "linux.") "netvar.hpp", csgo::dumper_t::EType::CPP_HEADER);
    dumper.to_file(WIN_LINUX("windows.", "linux.") "netvar.cs" , csgo::dumper_t::EType::CSHARP);
    dumper.to_file(WIN_LINUX("windows.", "linux.") "netvar.txt", csgo::dumper_t::EType::RAW, false);

    return 0;
}