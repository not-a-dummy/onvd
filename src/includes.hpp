#pragma once

#if defined _WIN32
#  include <Windows.h>
#  define WIN_LINUX(win, linux) win
#elif defined __linux__
#  include <dlfcn.h>
#  define WIN_LINUX(win, linux) linux
#else
#  error Unsupported OS
#endif

#include <algorithm>
#include <array>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace fs = std::filesystem;

#include "dumper/dumper.hpp"
#include "modules/modules.hpp"
#include "netvar/netvar.hpp"

/// GCC doesn't have <format> yet, soo...
#define LOG(fmt, ...) std::fprintf(stdout, WIN_LINUX("[>] ", "\033[1m[>] ") fmt WIN_LINUX("\n", "\033[0m\n"), ## __VA_ARGS__)
#define LOG_SUCCESS(fmt, ...) std::fprintf(stdout, WIN_LINUX("[>] ", "\033[1;32m[>] ") fmt WIN_LINUX("\n", "\033[0m\n"), ## __VA_ARGS__)
#define LOG_ERROR(fmt, ...) std::fprintf(stderr, WIN_LINUX("[!] ", "\033[1;31m[!] ") fmt WIN_LINUX("\n", "\033[0m\n"), ## __VA_ARGS__)