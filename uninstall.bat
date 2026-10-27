@echo off
rem AnyLink Secure Client uninstall script
chcp 65001 >nul
setlocal
title AnyLink Secure Client 卸载
cd /d "%~dp0"

net session >nul 2>&1
if %errorlevel% neq 0 (
    echo 正在请求管理员权限，请在弹出的窗口中点击"是"...
    powershell -NoProfile -Command "Start-Process -FilePath '%~f0' -Verb RunAs"
    exit /b
)

echo.
echo ============================================
echo   AnyLink Secure Client 卸载
echo ============================================
echo.

echo [1/4] 停止客户端程序...
taskkill /f /im fuck.exe >nul 2>&1
taskkill /f /im vpnagent.exe >nul 2>&1

echo [2/4] 停止 VPN 服务 (SSLCon)...
net stop SSLCon >nul 2>&1

echo [3/4] 卸载 VPN 服务...
if exist "fuck123.exe" (
    "fuck123.exe" uninstall
) else (
    echo     ERROR: 未找到 fuck123.exe，尝试直接删除服务...
    sc delete SSLCon >nul 2>&1
)

echo [4/4] 清理完成。
echo.
echo 卸载完成。VPN 服务已移除，你可以手动删除本文件夹。
echo （VC++ 运行库为共享组件，为避免影响其他软件，未卸载。）
pause