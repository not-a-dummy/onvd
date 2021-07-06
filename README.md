# onvd - cross platform offline netvar dumper for csgo

It simply dumps all csgo netvar without needing to have running instance of csgo

![Screenshot](https://i.imgur.com/1oXZJrB.png)

# Build

### Build File

Open up your terminal inside the project's root directory then run `./vendor/premake/premake5(.exe) [action]`
where `[action]` can be anything listed [here](https://premake.github.io/docs/Using-Premake).

The output is inside the `build` directory, located at the project's root directory.

NOTE: I only tested to compile this with `g++ 10.3.0` and latest version of msvc.

### Linux

Prerequisite:

- gcc/g++ 10
- make

If you already have those installed then run `build_linux.sh` file,
then there should be a binary file called `onvd` in project's root directory.
Move said file to your csgo directory then run it, there should be 3 new files called `linux.netvar.<cs,hpp,txt>` after you run the executable.

### Windows

Prerequisite:

- MSVC

Run `build_windows.bat`, then there should be a binary file called `onvd.exe` in project's root directory.
Move said file to your csgo directory then run it, there should be 3 new files called `windows.netvar.<cs,hpp,txt>` after you run the executable.

If the build fails, like it's unable to find path to your MSBuild folder, you can find the `*.sln` file
inside the `build` directory (assuming you've run `build_windows.bat`)
then compile it using visual studio manually.

### Mac

I don't have one, so.. 🤷‍♂️

# Adding different output file

[`csgo::dumper_t`](src/dumper/dumper.hpp) has a member variable called [`m_filtered_netvar`](src/dumper/dumper.hpp#L26-L29),
this variable holds the "cleaned up" version of all netvar, however if you want every netvar without any of it being filtered
you should access it from [`m_netvar.get_map()`](src/dumper/dumper.hpp#L25).

Both of which returns:

```c++
std::unordered_map<std::string, /// (filtered) table name
      std::unordered_map<std::string, /// (filtered) prop name
      uintptr_t /// offset
>>
```

For reference, take a look at [dumper.cpp](src/dumper/dumper.cpp), I've put several different output files there.

# Why?

Stumbled upon the thread I linked in credit section also I've never make a cross platform app before,
so figured I'd try to make one, and I learned quite a lot from this simple project.

# Credit

- [Loading Module client_panorama.dll Externally @ UC](https://www.unknowncheats.me/forum/counterstrike-global-offensive/301870-loading-module-client_panorama-dll-externally.html)
