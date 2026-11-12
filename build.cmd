@echo off
rem Build with Ninja, configuring the build dir on first run. Ninja needs the
rem MSVC environment (INCLUDE/LIB), which the Visual Studio generator used to
rem set up implicitly. Extra args pass through, e.g.  build.cmd --target modul_app
rem
rem Ninja is single-config, so each build type gets its own directory. Set
rem BUILD_DIR / BUILD_TYPE to override the defaults (build/, Debug); the
rem build-release task in tasks.json does this.
setlocal
if not defined BUILD_DIR set "BUILD_DIR=build"
if not defined BUILD_TYPE set "BUILD_TYPE=Debug"
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo ERROR: vswhere.exe not found - is Visual Studio installed?
    exit /b 1
)
for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -property installationPath`) do set "VSPATH=%%i"
if not defined VSPATH (
    echo ERROR: no Visual Studio installation found.
    exit /b 1
)
call "%VSPATH%\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
if not exist "%BUILD_DIR%\CMakeCache.txt" (
    cmake -B "%BUILD_DIR%" -G Ninja -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    if errorlevel 1 exit /b 1
)
cmake --build "%BUILD_DIR%" %*
