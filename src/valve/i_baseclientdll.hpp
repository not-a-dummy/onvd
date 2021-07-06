#include <cstdint>
#include "client_class.hpp"

struct i_baseclientdll
{
    ClientClass* get_all_classes()
    {
        uintptr_t* vtable = *reinterpret_cast<uintptr_t**>(this);
        return reinterpret_cast<ClientClass*(WIN_LINUX(__thiscall,)*)(void* ecx)>(vtable[8])(this);
    }
};