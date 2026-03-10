#include <windows.h>

#include <iostream>



// a function that gets called each time a letter gets pressed

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {

    if (nCode == HC_ACTION && wParam == WM_KEYDOWN) {

        KBDLLHOOKSTRUCT* pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;

        int key = pKeyBoard->vkCode;

        std::cout << "Key Pressed: " << char(key) << std::endl;

        std::cout << "int of it: " << key << std::endl;



    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);

}



int main() {

    HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);



    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {

        TranslateMessage(&msg);

        DispatchMessage(&msg);

    }



    UnhookWindowsHookEx(hhkLowLevelKybd);

    return 0;

}

