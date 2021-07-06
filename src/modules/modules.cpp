#include "../includes.hpp"

static constexpr std::array<std::string_view, 18> m_modules_path = 
{
    WIN_LINUX("bin\\steam_api.dll", "./bin/linux64/libsteam_api.so"),
    WIN_LINUX("bin\\tier0.dll", "./bin/linux64/libtier0_client.so"),
    WIN_LINUX("bin\\vstdlib.dll", "./bin/linux64/libvstdlib_client.so"),
    WIN_LINUX("bin\\phonon.dll", "./bin/linux64/libphonon.so"),
    WIN_LINUX("bin\\parsifal.dll", ""),
    WIN_LINUX("bin\\v8_libbase.dll", "./bin/linux64/libv8_libbase.so"),
    WIN_LINUX("bin\\icuuc.dll", "./bin/linux64/libicuuc.so"),
    WIN_LINUX("bin\\icui18n.dll", "./bin/linux64/libicui18n.so"),
    WIN_LINUX("bin\\v8.dll", "./bin/linux64/libv8.so"),
    WIN_LINUX("bin\\libavutil-54.dll", "./bin/linux64/libavutil.so.54"),
    WIN_LINUX("bin\\libavcodec-56.dll", "./bin/linux64/libavcodec.so.56"),
    WIN_LINUX("bin\\libavformat-56.dll", "./bin/linux64/libavformat.so.56"),
    WIN_LINUX("bin\\libavresample-2.dll", "./bin/linux64/libavresample.so.2"),
    WIN_LINUX("bin\\libswscale-3.dll", "./bin/linux64/libswscale.so.3"),
    WIN_LINUX("bin\\video.dll", "./bin/linux64/libvideo.so"),

    WIN_LINUX("csgo\\bin\\client.dll", "./csgo/bin/linux64/client_client.so"),

    WIN_LINUX("bin\\steamnetworkingsockets.dll", "./bin/linux64/libsteamnetworkingsockets.so"),
    WIN_LINUX("bin\\engine.dll", "./bin/linux64/engine_client.so"),
};

bool csgo::modules_t::init()
{
    static bool initialized = false;
    if (initialized)
        return true;

    constexpr size_t longest_path = std::max_element(m_modules_path.begin(), m_modules_path.end(),
                                                     [](const auto& a, const auto& b)
                                                     { 
                                                        return a.length() < b.length();
                                                     })->length();

    for (size_t i = 0; const auto& mod : m_modules_path)
    {
        if (mod.empty())
            continue;

        fs::path file = mod;
        std::string filename = file.filename().string();

        void* address = WIN_LINUX(LoadLibraryA(file.string().c_str()), dlopen(file.string().c_str(), RTLD_LAZY));
        if (!address)
        {
            WIN_LINUX(
                LOG_ERROR("Failed to load %s;\n  GetLastError(): 0x%x", filename.c_str(), GetLastError()),
                LOG_ERROR("Failed to load %s;\n  Reason: %s", filename.c_str(), dlerror())
            );
            return false;
        }

        LOG("%s%*s %p",
            filename.c_str(),
            static_cast<int>(longest_path - filename.length()),
            "=>",
            address
        );

        m_loaded_modules[i] = std::make_pair(std::move(filename), address);

        i += 1;
    }

    initialized = true;

    return true;
}

void* csgo::modules_t::create_interface(std::string_view module_name, std::string_view interface_name) const
{
    void* mod = get_module(module_name);
    if (!mod)
        return nullptr;

    using create_interface_t = void*(*)(const char*, int*);
    auto create_interface_fn = reinterpret_cast<create_interface_t>(
        WIN_LINUX(GetProcAddress(reinterpret_cast<HMODULE>(mod), "CreateInterface"), dlsym(mod, "CreateInterface"))
    );
    if (!create_interface_fn)
        return nullptr;

    return create_interface_fn(interface_name.data(), nullptr);
}

void* csgo::modules_t::get_module(std::string_view module_name) const
{
    for (const auto& [mod_name, mod_address] : m_loaded_modules)
        if (mod_name == module_name)
            return mod_address;

    return nullptr;
}