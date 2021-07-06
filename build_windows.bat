@echo off
cd /d "%~dp0"

start "" /B /wait "vendor\premake\premake5.exe" vs2019

set msbuild=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe
if not exist "%msbuild%" goto fail

start "" /B /wait "%msbuild%" /m /p:Configuration=Release build/
xcopy /s build\onvd\bin\Release\onvd.exe .
goto end

:fail
echo Error: Unable to find path to MSBuild
goto end

:end