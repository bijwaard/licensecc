@echo off
set PROJECT=equimoves
set BOOST_ROOT=%~dp0\..\boost_1_86_0
set SSL_ROOT=C:\Program Files\OpenSSL-Win64
set INSTALL_DIR=%~dp0\install
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -DBOOST_ROOT="%BOOST_ROOT%" -DCMAKE_PREFIX_PATH="%SSL_ROOT%" -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" -DLCC_PROJECT_NAME=%PROJECT%
rem cmake .. -G "Visual Studio 17 2022" -A x64 -DBOOST_ROOT="C:/projects/equimoves/boost_1_86_0" -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR"
cmake --build . --config Release
cmake --install .
ctest -C Release
