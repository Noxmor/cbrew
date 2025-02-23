@echo off

set SCRIPT_DIR=%~dp0
set INSTALL_DIR=C:\cbrew
set BIN_DIR=%INSTALL_DIR%\bin
set INCLUDE_DIR=%INSTALL_DIR%\include\cbrew

:: Compiling cbrew
echo Compiling cbrew...
gcc -Wall -Wextra -Werror -O3 -I"%SCRIPT_DIR%include" "%SCRIPT_DIR%src\main.c" -o "%SCRIPT_DIR%cbrew"

:: Installing cbrew
echo Installing cbrew...
if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"
move /Y "%SCRIPT_DIR%cbrew.exe" "%BIN_DIR%" >nul

:: Removing old cbrew headers
if exist "%INCLUDE_DIR%" (
    echo Removing old cbrew headers...
    rmdir /s /q "%INCLUDE_DIR%"
)

:: Installing cbrew headers
echo Installing cbrew headers...
mkdir "%INCLUDE_DIR%"
xcopy /Y /Q "%SCRIPT_DIR%include\*" "%INCLUDE_DIR%\" >nul

echo Successfully installed cbrew
