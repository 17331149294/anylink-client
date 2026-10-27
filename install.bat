@echo off
rem AnyLink Secure Client one-click install script
chcp 65001 >nul
setlocal
title AnyLink Secure Client 安装
cd /d "%~dp0"

net session >nul 2>&1
if %errorlevel% neq 0 (
    echo 正在请求管理员权限，请在弹出的窗口中点击"是"...
    powershell -NoProfile -Command "Start-Process -FilePath '%~f0' -Verb RunAs"
    exit /b
)

echo.
echo ============================================
echo   AnyLink Secure Client 一键安装
echo ============================================
echo.

if exist "vc_redist.x64.exe" (
    echo [1/3] 安装 VC++ 运行库...
    "vc_redist.x64.exe" /install /quiet /norestart
) else (
    echo [1/3] 未找到 vc_redist.x64.exe，跳过（若程序无法启动请自行安装）。
)

echo [2/3] 注册 VPN 服务 (fuck123.exe)...
if exist "fuck123.exe" (
    "fuck123.exe" uninstall >nul 2>&1
    sc delete SSLCon >nul 2>&1
    "fuck123.exe" install
) else (
    echo     ERROR: 未找到 fuck123.exe
)

echo [3/3] 启动客户端...
if exist "fuck.exe" (
    start "" "fuck.exe"
) else (
    echo     ERROR: 未找到 fuck.exe
)

echo.
echo 安装完成。
pause