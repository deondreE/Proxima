@echo off
setlocal

set BUILD_DIR=build
set CONFIG=Debug

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
}


cd %BUILD_DIR%

cmake .. -G "Visual Studio 17 2022" -A x64
if errorlevel 1 goto :error

cmake --build . --config %CONFIG%
if errorlevel 1 goto :error

cd %CONFIG%
echo.
echo ===== Running example_app.exe =====
example_app.exe
cd ..

goto :eof

:error
echo.
echo ===== Build failed! =====
exit /b 1