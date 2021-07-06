#include "../includes.hpp"
#include "../valve/i_baseclientdll.hpp"

bool csgo::netvar_t::init() const
{
    auto& map = get_map();
    if (!map.empty())
        return true;

    size_t props_count = 0;
    
    std::function<void(RecvTable*, std::string_view, uintptr_t)> store_props =
        [&](RecvTable* table, std::string_view name, uintptr_t offset = 0)
    {
        for (int i = 0; i < table->m_nProps; ++i, ++props_count)
        {
            RecvProp* current = &table->m_pProps[i];

            RecvTable* child = current->m_pDataTable;
            if (child && child->m_nProps > 0)
                store_props(child, name, current->m_Offset + offset);

            map[table->m_pNetTableName][current->m_pVarName] = current->m_Offset + offset;
        }
    };

    i_baseclientdll* client_interface = reinterpret_cast<i_baseclientdll*>(
        m_modules.get().create_interface(WIN_LINUX("client.dll", "client_client.so"), "VClient018")
    );
    if (!client_interface)
        return false;

    ClientClass* cc = client_interface->get_all_classes();
    if (!cc)
        return false;

    size_t tables_count = 0;
    for (; cc; cc = cc->m_pNext, ++tables_count)
        store_props(cc->m_pRecvTable, cc->m_pRecvTable->m_pNetTableName, 0);

    LOG_SUCCESS("Dumped %zu table(s) and %zu prop(s)", tables_count, props_count);

    return true;
}

std::unordered_map<std::string, /// table name
    std::unordered_map<std::string, /// prop name
    uintptr_t /// offset
>>& csgo::netvar_t::get_map() const
{
    static std::unordered_map<std::string,
                std::unordered_map<std::string,
                uintptr_t
            >> map = {};
    return map;
}