#pragma once

namespace csgo
{
    struct modules_t
    {
        modules_t(modules_t&&) = delete;
        modules_t(const modules_t&) = delete;
        modules_t& operator=(modules_t&&) = delete;
        modules_t& operator=(const modules_t&) = delete;

        static modules_t& get()
        {
            static modules_t inst = {};
            return inst;
        }

        bool init();
        void* create_interface(std::string_view module_name, std::string_view interface_name) const;
        void* get_module(std::string_view module_name) const;

    private:
        modules_t() = default;
        ~modules_t() 
        { 
            for (const auto& [_, mod] : m_loaded_modules)
                if (mod)
                    WIN_LINUX(FreeLibrary(reinterpret_cast<HMODULE>(mod)), dlclose(mod)); 
        }

    private:
        std::array<std::pair<std::string, void*>, 18> m_loaded_modules = {};
    };
}